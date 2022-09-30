#include "Player.h"
#include "Input.h"
#include "SphereCollider.h"
#include "CollisionManager.h"
#include "MyMath.h"
#include "DebugText.h"
#include "CollisionAttribute.h"

using namespace DirectX;

Player* Player::Create(Model* model)
{
	//3Dオブジェクトのインスタンスを生成
	Player* instance = new Player();
	if (instance == nullptr)
	{
		return nullptr;
	}

	//初期化
	if (!instance->Initialize())
	{
		delete instance;
		assert(0);
	}

	//モデルのセット
	if (model)
	{
		instance->SetModel(model);
	}

	return instance;
}

bool Player::Initialize()
{
	if (!Object3d::Initialize())
	{
		return false;
	}

	//コライダーの追加
	float radius = 0.6f;
	//半径分だけ足元から浮いた座標を球中心にする
	SetCollider(new SphereCollider(XMVECTOR({ 0, radius, 0,0 }), radius));

	collider->SetAttribute(COLLISION_ATTR_ALLIES);

	position.y = 10.0f;

	return true;
}

void Player::Update()
{
	Input* input = Input::GetInstance();

	//A,Dで旋回
	if (input->PushKey(DIK_A))
	{
		rotation.y -= 2.0f;
	}
	else if (input->PushKey(DIK_D))
	{
		rotation.y += 2.0f;
	}

	//移動ベクトルをY軸回りの角度で回転
	XMVECTOR move = { 0,0,0.1f,0 };
	XMMATRIX matRot = XMMatrixRotationY(XMConvertToRadians(rotation.y));
	move = XMVector3TransformNormal(move, matRot);

	//向いている方向に移動
	if (input->PushKey(DIK_S))
	{
		position.x -= move.m128_f32[0];
		position.y -= move.m128_f32[1];
		position.z -= move.m128_f32[2];
	}
	else if (input->PushKey(DIK_W))
	{
		position.x += move.m128_f32[0];
		position.y += move.m128_f32[1];
		position.z += move.m128_f32[2];
	}

	//落下処理
	//if (!onGround)
	//{
	//	//下向き加速度
	//	const float fallAcc = -0.01f;
	//	const float fallVYMin = -0.5f;
	//	//加速
	//	fallV.m128_f32[1] = max(fallV.m128_f32[1] + fallAcc, fallVYMin);

	//	//移動
	//	position.x += fallV.m128_f32[0];
	//	position.y += fallV.m128_f32[1];
	//	position.z += fallV.m128_f32[2];
	//}
	//else if (Input::GetInstance()->TriggerKey(DIK_SPACE))//ジャンプ
	//{
	//	onGround = false;
	//	const float jumpVYFist = 0.2f; //ジャンプ時上向き初速
	//	 fallV = { 0, jumpVYFist, 0,0 };
	//}

	//行列の更新など
	Object3d::Update();

	//球コライダーを取得
	SphereCollider* sphereCollider = dynamic_cast<SphereCollider*>(collider);
	assert(sphereCollider);

	//球の上端から球の下端までのレイキャスト用レイを準備
	Ray ray;
	ray.start = sphereCollider->center;
	ray.start.m128_f32[1] += sphereCollider->GetRadius();
	ray.dir = { 0, -1, 0, 0 };
	RaycastHit raycastHit;

	//接地状態
	if (onGround)
	{
		//スムーズに坂を下る為の吸着距離
		const float adsDistance = 0.2f;
		//接地を維持
		if (CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_LANDSHAPE,
			&raycastHit, sphereCollider->GetRadius() * 2.0f + adsDistance))
		{
			onGround = true;
			position.y -= (raycastHit.distance - sphereCollider->GetRadius() * 2.0f);
			//行列の更新など
			Object3d::Update();
		}
		else
		{
			onGround = false;
			fallV = {};
		}
	}
	else if (fallV.m128_f32[1] <= 0.0f)//落下状態
	{
		if (CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_LANDSHAPE,
			&raycastHit, sphereCollider->GetRadius() * 2.0f))
		{
			//着地
			onGround = true;
			position.y -= (raycastHit.distance - sphereCollider->GetRadius() * 2.0f);
			//行列の更新など
			Object3d::Update();
		}
	}
}

void Player::OnCollision(const CollisionInfo& info)
{
	DebugText::GetInstance()->Print(50, 30 * 4, 2, "Hit");
}

