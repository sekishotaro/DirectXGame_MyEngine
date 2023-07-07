#include "GameCamera.h"
#include "Input.h"
#include "Player.h"
#include "SphereCollider.h"
#include "QueryCallback.h"
#include "CollisionManager.h"
#include "MyMath.h"

using namespace DirectX;
float GameCamera::dx = 0;
float GameCamera::dy = 0;
float GameCamera::dz = 0;
XMFLOAT3 GameCamera::eye;
XMFLOAT3 GameCamera::target;
float GameCamera::dis = 20.0f;
bool GameCamera::hitFlag = false;
std::unique_ptr<Object3d> GameCamera::Object;
Model* GameCamera::Model = nullptr;


GameCamera::GameCamera(int window_width, int window_height) : Camera(window_width, window_height)
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
	rota.x = 180.0f;
}

void GameCamera::Update()
{
	//カメラの位置移動処理(球面上の角度移動)
	XMFLOAT3 cameraPos = MoveUpdate();

	TerrainPushBackProcess(cameraPos);
	//コライダー更新
	Object->UpdateWorldMatrix();
	collider->Update();

	CorrectionProcess();
	cameraPos = cameraPos + correctionVal;
	SetEye(cameraPos);
	
	//常時自機にターゲット
	XMFLOAT3 targetPos = TargetProcess();
	
	targetPos.y += 6.0f;
	
	Camera::SetTarget(targetPos);
	Camera::Update();
}

void GameCamera::UpdateOnly()
{
	XMFLOAT3 cameraPos = {};
	cameraPos = SphereCoordinateSystem();
	SetEye(cameraPos);
	//常時自機にターゲット
	XMFLOAT3 targetPos = TargetProcess();
	targetPos.y += 6.0f;
	Camera::SetTarget(targetPos);
	Camera::Update();
}

GameCamera::XMFLOAT3 GameCamera::SphereCoordinateSystem()
{
	XMFLOAT2 radius = { rota.x * 3.14f / 180.0f, rota.y * 3.14f / 180.0f };
	XMFLOAT3 cameraPos = {};

	cameraPos = TargetProcess();
	
	//球面座標系
	cameraPos.y += (dis) * cos(radius.y);
	cameraPos.x += (dis) * sin(radius.y) * cos(radius.x);
	cameraPos.z += (dis) * sin(radius.y) * sin(radius.x);

	return cameraPos;
}

GameCamera::XMFLOAT3 GameCamera::MoveUpdate()
{
	XMFLOAT3 cameraPos = {};
	//操作
	Operation();
	//坂
	SlopeRotaYProcess();

	//視点正面移動
	NoticeProcess();

	cameraPos = SphereCoordinateSystem();

	oldPosY = cameraPos.y;
	return cameraPos;
}

void GameCamera::Operation()
{
	if (PlayerJumpUp() == true) return;

	//半径
	float disMax = 20.0f;
	//キーボード
	if (Input::GetInstance()->PushKey(DIK_UP)) { rota.y -= 1.0f; }
	else if (Input::GetInstance()->PushKey(DIK_DOWN)) { rota.y += 1.0f; }
	if (Input::GetInstance()->PushKey(DIK_RIGHT)) { rota.x += 1.0f; }
	else if (Input::GetInstance()->PushKey(DIK_LEFT)) { rota.x -= 1.0f; }
	//コントローラー
	if (Input::GetInstance()->RightStickIn(UP) && rota.y < 175)
	{
		rota.y += 1.0f;
		slopeRotaFlag = false;
		if (dis <= disMax && hitFlag == false) { dis += 0.5f; }
	}
	else if (Input::GetInstance()->RightStickIn(DOWN) && rota.y > 5)
	{
		rota.y -= 1.0f;
		slopeRotaFlag = false;
		if (dis <= disMax && hitFlag == false) { dis += 0.5f; }
	}
	if (Input::GetInstance()->RightStickIn(RIGHT))
	{
		rota.x -= 1.0f;
		if (dis <= disMax && hitFlag == false) { dis += 0.5f; }
	}
	else if (Input::GetInstance()->RightStickIn(LEFT))
	{
		rota.x += 1.0f;
		if (dis <= disMax && hitFlag == false) { dis += 0.5f; }
	}

	if (dis <= disMax && hitFlag == false) { dis += 0.5f; }
}

void GameCamera::TerrainPushBackProcess( XMFLOAT3& cameraPos)
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
		rota.y -= 1.0f;
		slopeRotaFlag = false;
		cameraPos = SphereCoordinateSystem();
	}
}

void GameCamera::NoticeProcess()
{
	//視点正面移動
	static float endRota = 0;
	if (Input::GetInstance()->PushPadbutton(GAMEPAD_RIGHT_SHOULDER) && viewpointSwitchFlag == false)
	{
		endRota = GetNoticeRot();
	}

	//崖上がり時視点正面移動
	if (static_cast<int>(Player::GetStatus()) == 8 && static_cast<int>(Player::GetOldStatus()) != 15)
	{
		endRota = GetNoticeRot();
	}

	rota.x = AngleNormalize(rota.x);
	rota.y = AngleNormalize(rota.y);

	static float MoveTime = 0.0f;
	if (viewpointSwitchFlag == false)
	{
		MoveTime = 0.0f;
		return;
	}

	endRota = AngleNormalize(endRota);

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
	rota.x = leap(viewpointSwitchposParRotX, endRota, timeRatio);
	rota.y = leap(viewpointSwitchposParRotY, 60.0f, timeRatio);
}

float GameCamera::GetNoticeRot()
{
	float endRota = 0.0f;
	endRota -= Player::GetRot().y + 90.0f;
	endRota = AngleNormalize(endRota);
	viewpointSwitchFlag = true;
	viewpointSwitchposParRotX = rota.x;
	viewpointSwitchposParRotY = rota.y;

	return endRota;
}

float GameCamera::leap(float rotaA, float rotaB, float timeRatio)
{
	float result = 0.0f;
	result = rotaA * (1.0f - timeRatio) + rotaB * timeRatio;
	return result;
}

float GameCamera::AngleNormalize(const float rot)
{
	float Rot = rot;
	if (rot >= 360.0f)
	{
		Rot -= 360.0f;
	}
	if (rot <= 0.0f)
	{
		Rot += 360.0f;
	}

	return Rot;
}

bool GameCamera::PlayerJumpUp()
{
	if (Player::GetStatus() == 3 && Player::GetStatus() == 4)
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

GameCamera::XMFLOAT3 GameCamera::TargetProcess()
{
	XMFLOAT3 result = {};
	//平面移動
	result = { Player::GetPos().x, 0.0f,Player::GetPos().z };

	CliffFlagUpdate();

	//Y座標移動
	if (static_cast<int>(Player::GetStatus()) == 3)			//通常ジャンプ上昇中※△
	{
		//ジャンプ前座標
		result.y = oldTargetPos.y;
	}
	else if (static_cast<int>(Player::GetStatus()) == 4)	//通常ジャンプ下降中※△
	{
		//ジャンプ前座標
		result.y = oldTargetPos.y;
	}
	else if (static_cast<int>(Player::GetStatus()) == 13)	// 壁蹴りジャンプ上昇中※〇
	{
		//崖上がり前座標
		result.y = oldTargetPos.y;
	}
	else if (static_cast<int>(Player::GetStatus()) == 14)	// 壁蹴りジャンプ下降中※〇
	{
		//崖上がり前座標
		result.y = oldTargetPos.y;
	}
	else if (static_cast<int>(Player::GetStatus()) == 8)	// 崖つかみ中
	{
		//崖上がり前座標
		result.y = oldTargetPos.y;
	}
	else if (cliffTargetFlag == true)	// 崖上がりタイミングからのカメラ移動
	{
		result.y = CliffMoveTargetState();
	}
	else //その他移動は自機の位置にマーク
	{
		result.y = Player::GetPos().y;
	}

	oldTargetPos = result;
	return result;
}

void GameCamera::CliffFlagUpdate()
{
	//崖上りフラグが立っていたら早期リターン
	if (cliffTargetFlag == true) return;

	//崖上がりをした瞬間
	if (static_cast<int>(Player::GetStatus()) == 15 && static_cast<int>(Player::GetOldStatus()) == 8)
	{
		cliffTargetFlag = true;
		moveAftaerPosY = Player::GetPos().y;
		movePreviousPosY = oldTargetPos.y;
	}
}

void GameCamera::CorrectionProcess()
{
	static XMFLOAT3 moveVal = {};

	if (CorrectionCheck() == true) //移動
	{
		if (moveVal.x < 1.0f && moveVal.x > -1.0f)
		{
			moveVal.x += (-1.0f) * Player::GetMove().x;
		}

		if (moveVal.z < 1.0f && moveVal.z > -1.0f)
		{
			moveVal.z += (-1.0f) * Player::GetMove().z;
		}
	}
	else
	{

		if (moveVal.x < 0.0f)
		{
			moveVal.x += 0.1f;
		}
		else if (moveVal.x > 0.0f)
		{
			moveVal.x -= 0.1f;
		}

		
		if (moveVal.z < 0.0f)
		{
			moveVal.z += 0.1f;
		}
		else if (moveVal.z > 0.0f)
		{
			moveVal.z -= 0.1f;
		}

		if (0.1f >= moveVal.x && moveVal.x >= -0.1f)
		{
			moveVal.x = 0.0f;
		}
		if (0.1f >= moveVal.z && moveVal.z >= -0.1f)
		{
			moveVal.z = 0.0f;
		}
	}
	correctionVal = moveVal;
}

bool GameCamera::CorrectionCheck()
{
	if (static_cast<int>(Player::GetStatus()) == 1) return true;
	if (static_cast<int>(Player::GetStatus()) == 2) return true;
	if (static_cast<int>(Player::GetStatus()) == 3) return true;
	if (static_cast<int>(Player::GetStatus()) == 4) return true;
	return false;
}

void GameCamera::SlopeRotaYProcess()
{
	if (Player::GetSlopeFlag() == true)	//坂の上にいる
	{
		OnSlopeProcess();
	}
	else
	{
		UnSlopeProcess();
	}
}

void GameCamera::OnSlopeProcess()
{
	if (slopeRotaFlag == false)
	{
		slopeRotaFlag = true;
	}
	//		坂の上下角度		右		左
	const XMFLOAT2 slopeUp	= {  75.0f, 105.0f };
	const XMFLOAT2 slopeDown	= { 255.0f, 285.0f };
	//坂上下Y角度			上		下
	XMFLOAT2 slopeRotY = { 100.0f, 50.0f };

	if (Player::GetRot().y >= slopeUp.x && Player::GetRot().y <= slopeUp.y)
	{
		if (rota.y < slopeRotY.x)
		{
			rota.y += 0.5f;
		}
	}
	else if (Player::GetRot().y >= slopeDown.x && Player::GetRot().y <= slopeDown.y)
	{
		if (rota.y > slopeRotY.y)
		{
			rota.y -= 0.5f;
		}

		if (rota.x < 1.0f || rota.x > 359.0f)
		{
			rota.x = 0;
		}
		else if (rota.x <= 180 && rota.x > 0)
		{
			rota.x -= 1.0f;
		}
		else if (rota.x > 180 && rota.x < 360)
		{
			rota.x += 1.0f;
		}
	}
}

void GameCamera::UnSlopeProcess()
{
	if (slopeRotaFlag == false) return;
	
	if (rota.y > 70)
	{
		rota.y -= 0.5f;
	}
	
	if (rota.y < 70)
	{
		rota.y += 0.5f;
	}
}

float GameCamera::CliffMoveTargetState()
{
	const float timeMax = 1.0f;							//最大時間
	float time = timeMax - cliffTargetCount;			//加算時間に変化
	float timeRate = min(time / timeMax, 1.0f);			//タイムレート 0.0f->1.0f

	//移動完了時間の初期化
	if (cliffTargetCount <= 0.0f)
	{
		cliffTargetFlag = false;
		cliffTargetCount = timeMax;
		return oldTargetPos.y;
	}

	XMFLOAT3 pos = MyMath::lerp({0.0f, movePreviousPosY, 0.0f }, { 0.0f, moveAftaerPosY, 0.0f }, timeRate);

	cliffTargetCount -= 1.0f / 60.0f;

	return pos.y;
}
