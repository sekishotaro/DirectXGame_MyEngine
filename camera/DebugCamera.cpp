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
	rotaX = 180.0f;
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
	float radiusX = rotaX * 3.14f / 180.0f;
	float radiusY = rotaY * 3.14f / 180.0f;
	XMFLOAT3 cameraPos = {};
	cameraPos.x = Player::GetPos().x;
	cameraPos.z = Player::GetPos().z;
	if (PlayerJumpUp() == true)
	{
		cameraPos.y = oldPosY;
	}
	else
	{
		cameraPos.y = Player::GetPos().y;
		cameraPos.y += dis * cos(radiusY);
	}

	//球面座標系
	
	cameraPos.x += dis * sin(radiusY) * cos(radiusX);
	cameraPos.z += dis * sin(radiusY) * sin(radiusX);

	return cameraPos;
}

DebugCamera::XMFLOAT3 DebugCamera::MoveUpdate()
{
	XMFLOAT3 cameraPos = {};

	if (PlayerJumpUp() != true)
	{
		if (Input::GetInstance()->PushKey(DIK_UP)) { rotaY -= 1.0f; }
		else if (Input::GetInstance()->PushKey(DIK_DOWN)) { rotaY += 1.0f; }
		if (Input::GetInstance()->PushKey(DIK_RIGHT)) { rotaX += 1.0f; }
		else if (Input::GetInstance()->PushKey(DIK_LEFT)) { rotaX -= 1.0f; }
		if (Input::GetInstance()->PushKey(DIK_E) && dis >= 5.0f) { dis -= 1.0f; }
		else if (Input::GetInstance()->PushKey(DIK_Z) && dis <= 20.0f) { dis += 1.0f; }

		if (Input::GetInstance()->RightStickIn(UP) && rotaY < 175)
		{
			rotaY += 1.0f;
			if (dis <= 20.0f && hitFlag == false) { dis += 0.5f; }
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
	}
	
	static float endRota = 0;
	if (Input::GetInstance()->PushPadbutton(GAMEPAD_RIGHT_SHOULDER) && viewpointSwitchFlag == false)
	{
		endRota = 0;
		endRota -= Player::GetRot().y + 90.0f;
		if (endRota >= 360.0f)
		{
			endRota -= 360.0f;
		}
		if (endRota <= 0.0f)
		{
			endRota += 360.0f;
		}
		viewpointSwitchFlag = true;
		viewpointSwitchposParRotX = rotaX;
		viewpointSwitchposParRotY = rotaY;
	}

	if (rotaX >= 360.0f)
	{
		rotaX -= 360.0f;
	}
	if (rotaX <= 0.0f)
	{
		rotaX += 360.0f;
	}

	if (rotaY >= 360.0f)
	{
		rotaY -= 360.0f;
	}
	if (rotaY <= 0.0f)
	{
		rotaY += 360.0f;
	}

	ViewpointSwitch(endRota);

	cameraPos = SphereCoordinateSystem();

	oldPosY = cameraPos.y;
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

void DebugCamera::ViewpointSwitch(float endRota)
{
	static float MoveTime = 0.0f;
	if (viewpointSwitchFlag == false)
	{
		MoveTime = 0.0f;
		return;
	}

	if (endRota >= 360.0f)
	{
		endRota -= 360.0f;
	}
	if (endRota <= 0.0f)
	{
		endRota += 360.0f;
	}

	const float MoveMaxTime = 0.3f; //移動にかかる時間
	float timeRatio = MoveTime / MoveMaxTime;
	if (MoveTime <= MoveMaxTime)
	{
		//エフェクトの時間を進める
		MoveTime += 1.0f / 60.0f;
	}
	else
	{
		viewpointSwitchFlag = false;
		return;
	}

	if (dis <= 20.0f && hitFlag == false) { dis += 1.0f; }
	rotaX = leap(viewpointSwitchposParRotX, endRota, timeRatio);
	rotaY = leap(viewpointSwitchposParRotY, 60.0f, timeRatio);
}

float DebugCamera::leap(float rotaA, float rotaB, float timeRatio)
{
	float result = 0.0f;
	result = rotaA * (1.0f - timeRatio) + rotaB * timeRatio;
	return result;
}

bool DebugCamera::PlayerJumpUp()
{
	if (Player::GetAnimeNum() == 3)
	{
		return true;
	}
	else if (Player::GetAnimeNum() == 13)
	{
		return true;
	}
	else if (Player::GetAnimeNum() == 14)
	{
		return true;
	}
	
	return false;
}
