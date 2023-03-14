#include "DebugCamera.h"
#include "Input.h"
#include "Player.h"
#include "Enemy.h"
#include "SphereCollider.h"
#include "QueryCallback.h"
#include "CollisionManager.h"

using namespace DirectX;
float DebugCamera::dx = 0;
float DebugCamera::dy = 0;
float DebugCamera::dz = 0;
XMFLOAT3 DebugCamera::eye;
XMFLOAT3 DebugCamera::target;
XMFLOAT3 DebugCamera::distance = { 0, 5.0f, -10.0 };
float DebugCamera::rotaX = 270.0f;
float DebugCamera::rotaY = 70.0f;
float DebugCamera::dis = 20.0f;
bool DebugCamera::hitFlag = false;
std::unique_ptr<Object3d> DebugCamera::Object;
Model* DebugCamera::Model = nullptr;


DebugCamera::DebugCamera(int window_width, int window_height) : Camera(window_width, window_height)
{
	Model = Model::LoadFromOBJ("skydome");
	Object = Object3d::Create();
	Object->SetModel(Model);

	//コライダーの追加
	float radius = 0.6f;
	//半径分だけ足元から浮いた座標を球中心にする
	Object->SetCollider(new SphereCollider(XMVECTOR({ 0, radius, 0,0 }), radius));
	collider = Object->GetBaseCollider();
	collider->SetAttribute(COLLISION_ATTR_ALLIES);
	Object->SetBaseCollider(collider);
}

void DebugCamera::Update()
{
	Input::MousePos mpos = Input::GetInstance()->MousePosLoad();

	XMFLOAT3 cameraPos = MoveUpdate();

	UpdateProcess(cameraPos);
	RaidCameraCount();
	//コライダー更新
	Object->UpdateWorldMatrix();
	collider->Update();

	SetEye(cameraPos);
	XMFLOAT3 targetPos = Player::GetPos();
	if (RaidTargetCameraFlag == true)
	{
		targetPos = Enemy::GetPos();
	}
	
	targetPos.y += 6.0f;
	oldRaidFlag = Enemy::GetRaidFlag();
	Camera::SetTarget(targetPos);
	Camera::Update();
}

DebugCamera::XMFLOAT3 DebugCamera::SphereCoordinateSystem()
{
	XMFLOAT3 cameraPos = Player::GetPos();
	float radiusX = rotaX * 3.14f / 180.0f;
	float radiusY = rotaY * 3.14f / 180.0f;

	//球面座標系
	cameraPos.y += dis * cos(radiusY);
	cameraPos.x += dis * sin(radiusY) * cos(radiusX);
	cameraPos.z += dis * sin(radiusY) * sin(radiusX);

	return cameraPos;
}

DebugCamera::XMFLOAT3 DebugCamera::MoveUpdate()
{
	XMFLOAT3 cameraPos = Player::GetPos();

	if (Input::GetInstance()->PushKey(DIK_UP)) { rotaY -= 1.0f; }
	else if (Input::GetInstance()->PushKey(DIK_DOWN)) { rotaY += 1.0f; }
	if (Input::GetInstance()->PushKey(DIK_RIGHT)) { rotaX += 1.0f; }
	else if (Input::GetInstance()->PushKey(DIK_LEFT)) { rotaX -= 1.0f; }
	if (Input::GetInstance()->PushKey(DIK_E) && dis >= 5.0f) { dis -= 1.0f; }
	else if (Input::GetInstance()->PushKey(DIK_Z) && dis <= 20.0f) { dis += 1.0f; }

	if (Input::GetInstance()->RightStickIn(UP) && rotaY < 175) 
	{ 
		rotaY += 1.0f; 
		if (dis <= 20.0f && hitFlag == false) { dis += 0.5f;}
	}
	else if (Input::GetInstance()->RightStickIn(DOWN) && rotaY > 5) 
	{ 
		rotaY -= 1.0f; 
		if (dis <= 20.0f && hitFlag == false) { dis += 0.5f; }
	}
	if (Input::GetInstance()->RightStickIn(RIGHT)) 
	{ 
		rotaX -= 1.0f; 
		if (dis <= 20.0f && hitFlag == false) { dis += 0.5f; }
	}
	else if (Input::GetInstance()->RightStickIn(LEFT)) 
	{ 
		rotaX += 1.0f; 
		if (dis <= 20.0f && hitFlag == false) { dis += 0.5f; }
	}
	//if (Input::GetInstance()->PushPadbutton(GAMEPAD_LEFT_TRIGGER) && dis >= 5.0f) { dis -= 1.0f; }
	//else if (Input::GetInstance()->PushPadbutton(GAMEPAD_RIGHT_TRIGGER) && dis <= 20.0f) { dis += 1.0f; }

	cameraPos = SphereCoordinateSystem();

	return cameraPos;
}

void DebugCamera::UpdateProcess( XMFLOAT3& cameraPos)
{
	//球コライダーを取得
	Object->SetPosition(cameraPos);
	Object->Update();

	SphereCollider* sphereCollider = dynamic_cast<SphereCollider*>(collider);
	assert(sphereCollider);

	//クエリ―コールバッククラス
	class CameraQueryCallback :public QueryCallback
	{
	public:
		CameraQueryCallback(Sphere* sphere) : sphere(sphere) {};

		//衝突時コールバック関数
		bool OnQueryHit(const QueryHit& info)
		{
			//ワールドの上方向
			const XMVECTOR up = { 0,1,0,0 };
			//排斥方向
			XMVECTOR rejectDir = XMVector3Normalize(info.reject);
			//上方向と排斥方向の角度差のcos値
			float cos = XMVector3Dot(rejectDir, up).m128_f32[0];

			//地面判定しきい値角度
			const float threshold = cosf(XMConvertToRadians(45.0f));

			//角度差によって天井または地面と判定されるものを除いて
			if (-threshold < cos && cos < threshold)
			{
				//球を排斥 (押し出す)
				sphere->center += info.reject;
				move += info.reject;
			}

			return true;
		}

		//クエリ―に使用する球
		Sphere* sphere = nullptr;
		//排斥による移動量(累積値)
		DirectX::XMVECTOR move = {};
	};

	//クエリ―コールバックの関数オブジェクト
	CameraQueryCallback callback(sphereCollider);

	//球と地形の交差を全検索
	CollisionManager::GetInstance()->QuerySphere(*sphereCollider, &callback, COLLISION_ATTR_LANDSHAPE);
	//交差による排斥分動かす
	cameraPos.x += callback.move.m128_f32[0];
	cameraPos.y += callback.move.m128_f32[1];
	cameraPos.z += callback.move.m128_f32[2];

	if (callback.move.m128_f32[0] != 0)
	{
		hitFlag = true;
		if (dis > 5.0f) { dis -= 0.5f; }
		cameraPos = SphereCoordinateSystem();
	} 
	else if (callback.move.m128_f32[1] != 0)
	{
		hitFlag = true;
		if (dis > 5.0f) { dis -= 0.5f; }
		cameraPos = SphereCoordinateSystem();
	}
	else if (callback.move.m128_f32[2] != 0)
	{
		hitFlag = true;
		if (dis > 5.0f) { dis -= 0.5f; }
		cameraPos = SphereCoordinateSystem();
	}
	else
	{
		hitFlag = false;
	}

	//球の上端から球の下端までのレイキャスト用レイを準備
	Ray ray;
	ray.start = sphereCollider->center;
	ray.dir = { 0, -1, 0, 0 };
	RaycastHit raycastHit;

	if (CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_LANDSHAPE,
		&raycastHit, sphereCollider->GetRadius() * 2.0f) == true)
	{
		if (dis > 5.0f) { dis -= 1.0f; }
		hitFlag = true;
		rotaY -= 1.0f;
		cameraPos = SphereCoordinateSystem();
	}
}

void DebugCamera::UpdateOnly()
{
	Camera::Update();
}

void DebugCamera::RaidCameraCount()
{
	if (oldRaidFlag == false && Enemy::GetRaidFlag() == true)
	{
		RaidTargetCameraFlag = true;
	}

	if (RaidTargetCameraFlag == false) return;

	if (count >= 300)
	{
		RaidTargetCameraFlag = false;
		count = 0;
		return;
	}
	count++;
}
