#include "Player.h"
#include "Input.h"
#include "SphereCollider.h"
#include "CollisionManager.h"
#include "MyMath.h"
#include "DebugText.h"
#include "CollisionAttribute.h"
#include "Collision.h"
#include "JsonLoader.h"

using namespace DirectX;

XMFLOAT3 Player::pos = { 0,0,0 };
XMFLOAT3 Player::moveV = { 0,0,0 };
bool Player::nowMove = false;
bool Player::onGround = false;

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

	position.x = 0.0f;
	position.y = 5.0f;
	position.z = -10.0f;

	//コライダーの追加
	float radius = 0.6f;
	//半径分だけ足元から浮いた座標を球中心にする
	SetCollider(new SphereCollider(XMVECTOR({ 0, radius, 0,0 }), radius));

	collider->SetAttribute(COLLISION_ATTR_ALLIES);

	pos = position;

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
		nowMove = true;
	}
	else if (input->PushKey(DIK_W))
	{
		position.x += move.m128_f32[0];
		position.y += move.m128_f32[1];
		position.z += move.m128_f32[2];
		nowMove = true;
	}
	else
	{
		nowMove = false;
	}

	if (onObject)
	{
		onGround = true;
	}
	

	//落下処理
	if (!onGround)
	{
		//下向き加速度
		const float fallAcc = -0.01f;
		const float fallVYMin = -0.5f;
		//加速
		fallV.m128_f32[1] = max(fallV.m128_f32[1] + fallAcc, fallVYMin);

		//移動
		position.x += fallV.m128_f32[0];
		position.y += fallV.m128_f32[1];
		position.z += fallV.m128_f32[2];
	}
	else if (Input::GetInstance()->TriggerKey(DIK_SPACE))//ジャンプ
	{
		onGround = false;
		nowMove = true;
		const float jumpVYFist = 0.5f; //ジャンプ時上向き初速
		 fallV = { 0, jumpVYFist, 0,0 };
	}

	//球コライダーを取得
	SphereCollider* sphereCollider = dynamic_cast<SphereCollider*>(collider);
	assert(sphereCollider);

	//球の上端から球の下端までのレイキャスト用レイを準備
	Ray ray;
	ray.start = sphereCollider->center;
	ray.start.m128_f32[1] += sphereCollider->GetRadius();
	ray.dir = { 0, -1, 0, 0 };
	RaycastHit raycastHit;

	pos = position;
	moveV.x = move.m128_f32[0];
	moveV.y = move.m128_f32[1];
	moveV.z = move.m128_f32[2];


	//行列の更新など
	Object3d::Update();

	//地面接地状態
	if (onGround && onObject == false)
	{
		//スムーズに坂を下る為の吸着距離
		const float adsDistance = 0.2f;

		//接地を維持
		if (CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_LANDSHAPE,
			&raycastHit, sphereCollider->GetRadius() * 2.0f + adsDistance) == true)
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
		nowMove = true;
		if (CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_LANDSHAPE,
			&raycastHit, sphereCollider->GetRadius() * 2.0f) == true)
		{
			//着地
			onGround = true;
			position.y -= (raycastHit.distance - sphereCollider->GetRadius() * 2.0f);
			//行列の更新など
			Object3d::Update();
		}
	}

	//自機の一定の距離内の障害物を抽出し、その障害物とだけ当たり判定を取る。
	Box playerBox;
	playerBox.centerPos = position;
	playerBox.size = { 0.5f, 0.5f, 0.5f };
	playerBox.LeastPos = XMFLOAT3(playerBox.centerPos.x - playerBox.size.x, playerBox.centerPos.y - playerBox.size.y, playerBox.centerPos.z - playerBox.size.z);
	playerBox.MaxPos = XMFLOAT3(playerBox.centerPos.x + playerBox.size.x, playerBox.centerPos.y + playerBox.size.y, playerBox.centerPos.z + playerBox.size.z);

	ray;
	ray.start = sphereCollider->center;
	ray.start.m128_f32[0];
	ray.start.m128_f32[1];
	ray.start.m128_f32[2];
	ray.dir = { move.m128_f32[0], move.m128_f32[1], move.m128_f32[2] };
	ray.dir.m128_f32[1] += fallV.m128_f32[1];

	XMFLOAT3 distance = { 0, 0, 0 };

	Circle circle;
	circle.center = { position.x, position.z };
	circle.radius = 50.0f;

	std::vector<Box> boxs;
	Box box;
	XMVECTOR normal = { 0,0,0,0 };
	onObject = false;

	//自機からXZ軸で一定の距離の中に障害物オブジェクトの中心座標があるものだけ当たり判定用のコンテナに格納する
	for (int i = 0; i < JsonLoader::colliderObjects.size(); i++)
	{
		if (Collision::CheckCircleDot(circle, XMFLOAT2(JsonLoader::colliderObjects[i].get()->GetPosition().x, JsonLoader::colliderObjects[i].get()->GetPosition().z)) == true)
		{
			box.centerPos = JsonLoader::colliderObjects[i].get()->GetPosition();
			box.size = JsonLoader::colliderObjects[i].get()->GetScale();
			box.LeastPos = XMFLOAT3(box.centerPos.x - (box.size.x / 2), box.centerPos.y - (box.size.y / 2), box.centerPos.z - (box.size.z / 2));
			box.MaxPos = XMFLOAT3(box.centerPos.x + (box.size.x / 2), box.centerPos.y + (box.size.y / 2), box.centerPos.z + (box.size.z / 2));
			boxs.push_back(box);
		}
	}


	for (int i = 0; i < boxs.size(); i++)
	{
		//障害物
		if (adhesion)
		{
			//めり込まないようにする接着状態の維持
			if ( Collision::Check2Box(playerBox, boxs[i], distance) == true)
			{
				normal = Collision::CheckRayBoxforPlane(ray, boxs[i]);
				adhesion = true;
				PushBack(normal, distance);
				//行列の更新など
				Object3d::Update();
			}
			else
			{
				adhesion = false;
			}
		}
		else if (nowMove == true) //移動状態
		{
			//当たっているかの確認
			if (Collision::Check2Box(playerBox, boxs[i], distance) == true)
			{
				normal = Collision::CheckRayBoxforPlane(ray, box);
				adhesion = true;
				//めり込まないように押し戻し処理
				PushBack(normal, distance);
				//行列の更新など
				Object3d::Update();
			}
		}
	}
}

void Player::OnCollision(const CollisionInfo& info)
{
	//DebugText::GetInstance()->Print(50, 30 * 8, 2, "Hit");
}

void Player::PushBack(const DirectX::XMVECTOR& normal, const XMFLOAT3& distance)
{
	if (normal.m128_f32[0] != 0)
	{
		position.x += distance.x;
	}
	else if (normal.m128_f32[1] != 0)
	{
		//着地
		onObject = true;
		position.y += distance.y - 0.5f;
	}
	else if (normal.m128_f32[2] != 0)
	{
		position.z += distance.z;
	}
}

