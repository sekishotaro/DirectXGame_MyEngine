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
bool Player::adhesionMesh = false;
int Player::crystalNum = 0;
bool Player::goalFlag = false;

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

	position.x = JsonLoader::goalObjects[0].get()->GetPosition().x;
	position.y = JsonLoader::goalObjects[0].get()->GetPosition().y +5.0f;
	position.z = JsonLoader::goalObjects[0].get()->GetPosition().z;

	//コライダーの追加
	float radius = 0.6f;
	//半径分だけ足元から浮いた座標を球中心にする
	SetCollider(new SphereCollider(XMVECTOR({ 0, radius, 0,0 }), radius));

	collider->SetAttribute(COLLISION_ATTR_ALLIES);

	pos = position;

	crystalNum = (int)JsonLoader::crystalObjects.size();

	return true;
}

void Player::Update()
{
	Input* input = Input::GetInstance();

	XMVECTOR move = { 0,0,0.1f,0 };

	if (climbOperation == false)
	{
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
		
		XMMATRIX matRot = XMMatrixRotationY(XMConvertToRadians(rotation.y));
		move = XMVector3TransformNormal(move, matRot);
		float power = 1.0f;
		//向いている方向に移動
		if (input->PushKey(DIK_V))
		{
			power = 3.0f;
		}

		if (input->PushKey(DIK_S))
		{
			position.x -= move.m128_f32[0] * power;
			position.y -= move.m128_f32[1] * power;
			position.z -= move.m128_f32[2] * power;
			nowMove = true;
		}
		else if (input->PushKey(DIK_W))
		{
			position.x += move.m128_f32[0] * power;
			position.y += move.m128_f32[1] * power;
			position.z += move.m128_f32[2] * power;
			nowMove = true;
		}
		else
		{
			nowMove = false;
		}
	}
	else
	{
		if (input->PushKey(DIK_A))
		{

		}
		else if (input->PushKey(DIK_D))
		{

		}
		else if (input->PushKey(DIK_S))
		{
			position.y -= 1.0f;
		}
		else if (input->PushKey(DIK_W))
		{
			position.y += 1.0f;
		}
		else if (input->PushKey(DIK_P))
		{
			climbOperation = false;
			position.x += climbNormal.m128_f32[0];
			position.z += climbNormal.m128_f32[2];
		}
	}


	if (onObject)
	{
		onGround = true;
	}

	UpdateWorldMatrix();

	//落下処理
	if (!onGround && climbOperation == false)
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
	else if (Input::GetInstance()->TriggerKey(DIK_SPACE) && climbOperation == false)//ジャンプ
	{
		onGround = false;
		nowMove = true;
		const float jumpVYFist = 0.5f; //ジャンプ時上向き初速
		 fallV = { 0, jumpVYFist, 0,0 };
	}
	// ワールド行列更新
	UpdateWorldMatrix();
	collider->Update();


	//球コライダーを取得
	SphereCollider* sphereCollider = dynamic_cast<SphereCollider*>(collider);
	assert(sphereCollider);

	//クエリ―コールバッククラス
	class PlayerQueryCallback :public QueryCallback
	{
	public:
		PlayerQueryCallback(Sphere* sphere) : sphere(sphere) {};

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
	PlayerQueryCallback callback(sphereCollider);

	//球と地形の交差を全検索
	CollisionManager::GetInstance()->QuerySphere(*sphereCollider, &callback, COLLISION_ATTR_LANDSHAPE);
	//交差による排斥分動かす
	position.x += callback.move.m128_f32[0];
	position.y += callback.move.m128_f32[1];
	position.z += callback.move.m128_f32[2];
	//コライダー更新
	UpdateWorldMatrix();
	collider->Update();


	//壁のぼり判定
	ClimbWallJudge(callback.move);


	//球の上端から球の下端までのレイキャスト用レイを準備
	Ray ray;
	ray.start = sphereCollider->center;
	//ray.start.m128_f32[1] += sphereCollider->GetRadius();
	ray.dir = { 0, -1, 0, 0 };
	RaycastHit raycastHit;

	pos = position;
	moveV.x = move.m128_f32[0];
	moveV.y = move.m128_f32[1];
	moveV.z = move.m128_f32[2];

	//地面接地状態
	//メッシュコライダー
	//Y軸
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
	playerBox.size = { 0.5f, 1.0f, 0.5f };
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

	Object3d::Update();

	//クリスタルとの接触
	std::vector<Box> crystal;
	onObject = false;

	//自機からXZ軸で一定の距離の中に障害物オブジェクトの中心座標があるものだけ当たり判定用のコンテナに格納する
	for (int i = 0; i < JsonLoader::crystalColliderObjects.size(); i++)
	{
		box.centerPos = JsonLoader::crystalColliderObjects[i].get()->GetPosition();
		box.size = JsonLoader::crystalColliderObjects[i].get()->GetScale();
		box.LeastPos = XMFLOAT3(box.centerPos.x - (box.size.x / 2), box.centerPos.y - (box.size.y / 2), box.centerPos.z - (box.size.z / 2));
		box.MaxPos = XMFLOAT3(box.centerPos.x + (box.size.x / 2), box.centerPos.y + (box.size.y / 2), box.centerPos.z + (box.size.z / 2));
		crystal.push_back(box);

		if (Collision::Check2Box(playerBox, crystal[i], distance) == true)
		{
			JsonLoader::crystalColliderObjects.erase(JsonLoader::crystalColliderObjects.begin() + i);
			JsonLoader::crystalObjects.erase(JsonLoader::crystalObjects.begin() + i);

			crystalNum--;
		}
	}

	//ゴールとプレイヤーの当たり判定
	Sphere goal;
	goal.center = XMLoadFloat3(&JsonLoader::goalObjects[0].get()->GetPosition());
	goal.radius = 2.0f;

	Sphere player;
	player.center = XMLoadFloat3(&position);
	player.radius = 2.0f;


	if (Collision::CheckSphere2Sphere(player, goal))
	{
		goalFlag = true;
	}
	else
	{
		goalFlag = false;
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

void Player::ClimbWallJudge(XMVECTOR move)
{
	//壁のぼり用板ポリに当たっているかの判別
	Plate climbWall;
	
	climbWall.position = XMLoadFloat3(&JsonLoader::climbWallObjects[0].get()->GetPosition());
	climbWall.size = JsonLoader::climbWallObjects[0].get()->GetScale();
	XMFLOAT3 rota = JsonLoader::climbWallObjects[0].get()->GetRotation();

	//面の法線を求める
	XMVECTOR vertPos1 = XMLoadFloat3(&JsonLoader::climbWallObjects[0].get()->GetModel()->GetMeshes()[0]->GetVertices()[0].pos);
	XMVECTOR vertPos2 = XMLoadFloat3(&JsonLoader::climbWallObjects[0].get()->GetModel()->GetMeshes()[0]->GetVertices()[1].pos);
	XMVECTOR vertPos3 = XMLoadFloat3(&JsonLoader::climbWallObjects[0].get()->GetModel()->GetMeshes()[0]->GetVertices()[2].pos);

	XMMATRIX matWorld = JsonLoader::climbWallObjects[0].get()->GetMatWorld();

	vertPos1 = XMVector3TransformNormal(vertPos1, matWorld);
	vertPos2 = XMVector3TransformNormal(vertPos2, matWorld);
	vertPos3 = XMVector3TransformNormal(vertPos3, matWorld);



	//行列の計算
	//XMVECTOR transform(const XMVECTOR & v, const XMMATRIX & m)
	//{
	//	float w = v.x * m.m[0][3] + v.y * m.m[1][3] + v.z * m.m[2][3] + m.m[3][3];

	//	Vector3 result
	//	{
	//		 (v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0] + m.m[3][0]) / w,
	//		 (v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1] + m.m[3][1]) / w,
	//		 (v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2] + m.m[3][2]) / w
	//	};

	//	return result;
	//}


	XMVECTOR p0_p1 = vertPos2 - vertPos1;
	XMVECTOR p0_p2 = vertPos3 - vertPos1;

	//外積により、 2辺に垂直なベクトルを算出する
	climbWall.normal = XMVector3Cross(p0_p1, p0_p2);
	climbWall.normal = XMVector3Normalize(climbWall.normal);



	Sphere player;
	player.center = XMLoadFloat3(&position);
	player.radius = 1.0f;

	//壁にめり込んでいる
	if (climbOperation == false)
	{
		if (move.m128_f32[0] != 0.0f || move.m128_f32[2] != 0.0f)
		{
			count++;
		}
		else
		{
			count = 0;
		}
	}


	if (count == 60)
	{
		climbNormal = move;
		climbOperation = true;
		count = 0;
	}

}

