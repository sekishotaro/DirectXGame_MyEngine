#include "Player.h"
#include "Input.h"
#include "SphereCollider.h"
#include "CollisionManager.h"
#include "MyMath.h"
#include "DebugText.h"
#include "CollisionAttribute.h"
#include "Collision.h"
#include "JsonLoader.h"
#include "OpticalPost.h"

using namespace DirectX;

XMFLOAT3 Player::pos = { 0,0,0 };
XMFLOAT3 Player::moveV = { 0,0,0 };
float Player::moveAdjustmentNum = 1.0f;
bool Player::nowMove = false;
bool Player::onGround = false;
bool Player::adhesionMesh = false;
int Player::crystalNum = 0;
bool Player::goalFlag = false;
bool Player::climbOperation = false;
float Player::timeLimit = 60.0f;
bool Player::staminaBoostFlag = false;
float Player::staminaQuantity = 100.0f;
bool Player::staminaCut = false;
int Player::inputX = 0;
int Player::inputY = 0;
float Player::testRota = 0;

Player* Player::Create(FbxModel* model)
{
	//3Dオブジェクトのインスタンスを生成
	Player* instance = new Player();
	
	if (instance == nullptr)
	{
		return nullptr;
	}

	//初期化
	instance->Initialize();

	//モデルのセット
	if (model)
	{
		instance->SetModel(model);
	}

	return instance;
}

void Player::Initialize()
{
	goalFlag = false;
	timeLimit = 60.0f;
	staminaQuantity = 100.0f;
	staminaCut = false;

	FbxObject3d::Initialize();

	position = JsonLoader::goalObjects[0].get()->GetPosition();
	position.x = -59;
	//position.y = 30;
	position.z = 43;

	scale.x = 0.02f;
	scale.y = 0.02f;
	scale.z = 0.02f;

	//コライダーの追加
	float radius = 0.6f;
	//半径分だけ足元から浮いた座標を球中心にする
	SetCollider(new SphereCollider(XMVECTOR({ 0, radius, 0,0 }), radius));

	collider->SetAttribute(COLLISION_ATTR_ALLIES);

	crystalNum = (int)JsonLoader::crystalObjects.size();

	return;
}

void Player::Update()
{
	//移動量初期化
	XMVECTOR move = { 0,0,0.0f,0 };

	MoveOperation(move);

	StaminaManagement(move);

	if (onObject)
	{
		onGround = true;
	}

	UpdateWorldMatrix();

	//落下処理
	GravityConfirmationProcess();

	//地形との当たり判定(メッシュコライダー)
	TerrainConfirmationProcess();
	
	//障害物(AABB)の衝突処理
	ObstacleConfirmationProcess(move);

	//ゴールとプレイヤーの当たり判定
	GoalConfirmationProcess();

	//クリスタルとの接触
	CrystalConfirmationProcess();

	//制限時間
	TimeManagement();
}

void Player::OnCollision(const CollisionInfo& info)
{
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
	if (move.m128_f32[0] == 0.0f && move.m128_f32[1] == 0.0f && move.m128_f32[2] == 0.0f) return;

	//壁のぼり用板ポリに当たっているかの判別
	Plate climbWall;
	Box climbWallBox;
	Ray ray;
	ray.start = XMLoadFloat3(&position);
	ray.dir = move;
	bool climbWallHit = false;
	//押し出し方向ベクトル保存用
	XMVECTOR pushvec = {};


	for (int i = 0; i < (int)JsonLoader::climbWallObjects.size(); i++)
	{
		climbWall.position = XMLoadFloat3(&JsonLoader::climbWallObjects[i].get()->GetPosition());
		climbWall.size = JsonLoader::climbWallObjects[i].get()->GetScale();
		
		//面の法線を求める (今は決め打ちでやってる"climbWall.obj")
		climbWall.vert1 = XMLoadFloat3(&JsonLoader::climbWallObjects[i].get()->GetModel()->GetMeshes()[0]->GetVertices()[0].pos);
		climbWall.vert2 = XMLoadFloat3(&JsonLoader::climbWallObjects[i].get()->GetModel()->GetMeshes()[0]->GetVertices()[1].pos);
		climbWall.vert3 = XMLoadFloat3(&JsonLoader::climbWallObjects[i].get()->GetModel()->GetMeshes()[0]->GetVertices()[2].pos);
		climbWall.vert4 = XMLoadFloat3(&JsonLoader::climbWallObjects[i].get()->GetModel()->GetMeshes()[0]->GetVertices()[3].pos);
		climbWall.vert5 = XMLoadFloat3(&JsonLoader::climbWallObjects[i].get()->GetModel()->GetMeshes()[0]->GetVertices()[4].pos);
		climbWall.vert6 = XMLoadFloat3(&JsonLoader::climbWallObjects[i].get()->GetModel()->GetMeshes()[0]->GetVertices()[5].pos);

		XMMATRIX matWorld = JsonLoader::climbWallObjects[i].get()->GetMatWorld();

		climbWall.vert1 = XMVector3TransformNormal(climbWall.vert1, matWorld);
		climbWall.vert2 = XMVector3TransformNormal(climbWall.vert2, matWorld);
		climbWall.vert3 = XMVector3TransformNormal(climbWall.vert3, matWorld);
		climbWall.vert4 = XMVector3TransformNormal(climbWall.vert4, matWorld);
		climbWall.vert5 = XMVector3TransformNormal(climbWall.vert5, matWorld);
		climbWall.vert6 = XMVector3TransformNormal(climbWall.vert6, matWorld);


		//法線を求める、求めるのは一つの三角ポリゴンで十分
		XMVECTOR p0_p1 = climbWall.vert2 - climbWall.vert1;
		XMVECTOR p0_p2 = climbWall.vert3 - climbWall.vert1;

		//外積により、 2辺に垂直なベクトルを算出する
		climbWall.normal = XMVector3Cross(p0_p1, p0_p2);
		climbWall.normal = XMVector3Normalize(climbWall.normal);


		climbWallBox.centerPos = JsonLoader::climbWallObjects[i].get()->GetPosition();
		climbWallBox.size = JsonLoader::climbWallObjects[i].get()->GetScale();

		XMVECTOR size = XMLoadFloat3(&JsonLoader::climbWallObjects[i].get()->GetScale());
		XMMATRIX matRot;
		matRot = XMMatrixIdentity();
		matRot *= XMMatrixRotationZ(XMConvertToRadians(JsonLoader::climbWallObjects[i].get()->GetRotation().z));
		matRot *= XMMatrixRotationX(XMConvertToRadians(JsonLoader::climbWallObjects[i].get()->GetRotation().x));
		matRot *= XMMatrixRotationY(XMConvertToRadians(JsonLoader::climbWallObjects[i].get()->GetRotation().y));
		size = XMVector3TransformNormal(size, matRot);
		size = XMVectorAbs(size);

		climbWallBox.size.x = XMVectorGetX(size);
		climbWallBox.size.y = XMVectorGetY(size);
		climbWallBox.size.z = XMVectorGetZ(size);

		climbWallBox.LeastPos = XMFLOAT3(climbWallBox.centerPos.x - climbWallBox.size.x, climbWallBox.centerPos.y - climbWallBox.size.y, climbWallBox.centerPos.z - climbWallBox.size.z);
		climbWallBox.MaxPos = XMFLOAT3(climbWallBox.centerPos.x + climbWallBox.size.x, climbWallBox.centerPos.y + climbWallBox.size.y, climbWallBox.centerPos.z + climbWallBox.size.z);

		if (Collision::CheckBoxDot(climbWallBox, position) == true)
		{
			climbWallHit = true;

			pushvec = -climbWall.normal;

			break;
		}
		else
		{
			climbWallHit = false;
		}
	}

	if (climbWallHit == false && climbWallHitPar == true)
	{
		position.x -= moveV.x;
		position.y -= moveV.y;
		position.z -= moveV.z;
		climbWallHit = true;
	}
	else if (climbWallHit == false && climbWallHitPar == false)
	{
		
	}

	//当たり判定保存
	climbWallHitPar = climbWallHit;

	//壁にめり込んでいる
	if (climbWallHit == true)
	{
		if (move.m128_f32[0] != 0.0f || move.m128_f32[2] != 0.0f)
		{
			wallCount++;
		}
		else
		{
			wallCount = 0;
		}
	}

	if (wallCount == 60)
	{
		climbNormal =  pushvec;
		climbOperation = true;
		wallCount = 0;
	}
}

void Player::MoveOperation(XMVECTOR& move)
{
	inputX = Input::GetInstance()->LeftStickInXNum();
	inputY = Input::GetInstance()->LeftStickInYNum();
	AnimationFlag = false;
	//移動量初期化
	move = { 0,0,0.1f,0 };

	if (staminaCut == true)
	{
		moveAdjustmentNum = 0.5f;
	}
	else
	{
		moveAdjustmentNum = 1.0f;
	}

	if (climbOperation == false) //通常移動
	{
		if (Input::GetInstance()->PushKey(DIK_A))
		{
			rotation.y -= 2.0f;
		}
		else if (Input::GetInstance()->PushKey(DIK_D))
		{
			rotation.y += 2.0f;
		}

		const int MAX = 24918;
		int x = 0;
		int y = 0;
		if (inputX != 0)
		{
			float xX = (float)inputX / MAX;
			inputX = (int)(xX * 100);
		}
		if (inputY != 0)
		{
			float xX = (float)inputY / MAX;
			inputY = (int)(xX * 100);
		}

		float rot1 = 0;
		float rot2 = 0;
		if (inputX != 0 || inputY != 0)
		{
			XMFLOAT2 vec1 = { 0, 100 };
			XMFLOAT2 vec2 = { (float)inputX,(float)inputY };
			XMFLOAT2 vec3 = { position.x - cameraPos2d.x, position.z - cameraPos2d.y };

			float inner = vec1.x * vec2.x + vec1.y * vec2.y;
			float veclong = sqrtf((vec1.x * vec1.x) + (vec1.y * vec1.y)) * sqrtf((vec2.x * vec2.x) + (vec2.y * vec2.y));
			float cos = inner / veclong;
			rot1 = acosf(cos);
			rot1 = rot1 * 180.0f / 3.1415f;
			if (inputX <= 0)
			{
				rot1 = (360.0f - rot1);
			}

			inner = vec1.x * vec3.x + vec1.y * vec3.y;
			veclong = sqrtf((vec1.x * vec1.x) + (vec1.y * vec1.y)) * sqrtf((vec3.x * vec3.x) + (vec3.y * vec3.y));
			cos = inner / veclong;
			rot2 = acosf(cos);
			rot2 = rot2 * 180.0f / 3.1415f;
			if (vec3.x <= 0)
			{
				rot2 = (360.0f - rot2);
			}
			rotation.y = rot1 + rot2;
		}

		//移動ベクトルをY軸回りの角度で回転

		XMMATRIX matRot = XMMatrixRotationY(XMConvertToRadians(rotation.y));
		testRota = rotation.y;
		move = XMVector3TransformNormal(move, matRot);
		float power = 1.0f;

		//向いている方向に移動
		if (Input::GetInstance()->PushKey(DIK_V) && staminaCut == false)
		{
			staminaBoostFlag = true;
			power = 3.0f;
		}
		else
		{
			staminaBoostFlag = false;
		}

		if (Input::GetInstance()->PushPadbutton(Button_A) && staminaCut == false)
		{
			if (inputX == 0 && inputY == 0)
			{
				staminaBoostFlag = false;
			}
			else
			{
				staminaBoostFlag = true;
				power = 3.0f;
			}
			
		}
		else
		{
			staminaBoostFlag = false;
		}

		if (Input::GetInstance()->PushKey(DIK_S))
		{
			position.x -= move.m128_f32[0] * power * moveAdjustmentNum;
			position.y -= move.m128_f32[1] * power * moveAdjustmentNum;
			position.z -= move.m128_f32[2] * power * moveAdjustmentNum;
			nowMove = true;
			
			if (AnimationFlag == false)
			{
				this->AnimationNum = 2;
				this->AnimationFlag = true;
			}
		}
		else if (Input::GetInstance()->PushKey(DIK_W))
		{
			position.x += move.m128_f32[0] * power * moveAdjustmentNum;
			position.y += move.m128_f32[1] * power * moveAdjustmentNum;
			position.z += move.m128_f32[2] * power * moveAdjustmentNum;
			nowMove = true;
			
			if (AnimationFlag == false)
			{
				this->AnimationNum = 2;
				this->AnimationFlag = true;
			}
			
		}
		else if (Input::GetInstance()->LeftStickIn(LEFT) || Input::GetInstance()->LeftStickIn(RIGHT))
		{
			position.x += move.m128_f32[0] * power;
			position.y += move.m128_f32[1] * power;
			position.z += move.m128_f32[2] * power;
			nowMove = true;

			if (AnimationFlag == false)
			{
				this->AnimationNum = 2;
				this->AnimationFlag = true;
			}
		}
		else if (Input::GetInstance()->LeftStickIn(UP) || Input::GetInstance()->LeftStickIn(DOWN))
		{
			position.x += move.m128_f32[0] * power;
			position.y += move.m128_f32[1] * power;
			position.z += move.m128_f32[2] * power;
			nowMove = true;

			if (AnimationFlag == false)
			{
				this->AnimationNum = 2;
				this->AnimationFlag = true;
			}
		}
		else
		{
			nowMove = false;
			this->AnimationFlag = false;
		}

	}
	else if (climbOperation == true) //壁のぼり移動
	{
		moveV = { 0,0,0 };

		if (Input::GetInstance()->PushKey(DIK_V))
		{
			staminaBoostFlag = true;
		}
		else
		{
			staminaBoostFlag = false;
		}

		if (Input::GetInstance()->PushPadbutton(Button_A))
		{
			staminaBoostFlag = true;
		}
		else
		{
			staminaBoostFlag = false;
		}

		//キーボード
		if (Input::GetInstance()->PushKey(DIK_A))
		{
			if (climbNormal.m128_f32[2] == 1.0f)
			{
				moveV.x += 0.5f;
			}
			else if (climbNormal.m128_f32[2] == -1.0f)
			{
				moveV.x -= 0.5f;
			}

			if (climbNormal.m128_f32[0] == 1.0f)
			{
				moveV.x -= 0.5f;
			}
			else if (climbNormal.m128_f32[0] == -1.0f)
			{
				moveV.z += 0.5f;
			}

			nowMove = true;
		}
		else if (Input::GetInstance()->PushKey(DIK_D))
		{
			if (climbNormal.m128_f32[2] == 1.0f)
			{
				moveV.x -= 0.5f;
			}
			else if (climbNormal.m128_f32[2] == -1.0f)
			{
				moveV.x += 0.5f;
			}

			if (climbNormal.m128_f32[0] == 1.0f)
			{
				moveV.x += 0.5f;
			}
			else if (climbNormal.m128_f32[0] == -1.0f)
			{
				moveV.z -= 0.5f;
			}

			nowMove = true;
		}
		else
		{
			nowMove = false;
		}

		//コントローラー
		if (Input::GetInstance()->LeftStickIn(LEFT))
		{
			if (climbNormal.m128_f32[2] == 1.0f)
			{
				moveV.x += 0.5f;
			}
			else if (climbNormal.m128_f32[2] == -1.0f)
			{
				moveV.x -= 0.5f;
			}

			if (climbNormal.m128_f32[0] == 1.0f)
			{
				moveV.x -= 0.5f;
			}
			else if (climbNormal.m128_f32[0] == -1.0f)
			{
				moveV.z += 0.5f;
			}

			nowMove = true;

			if (AnimationFlag == false)
			{
				this->AnimationNum = 1;
				this->AnimationFlag = true;
			}
		}
		else if (Input::GetInstance()->LeftStickIn(RIGHT))
		{
			if (climbNormal.m128_f32[2] == 1.0f)
			{
				moveV.x -= 0.5f;
			}
			else if (climbNormal.m128_f32[2] == -1.0f)
			{
				moveV.x += 0.5f;
			}

			if (climbNormal.m128_f32[0] == 1.0f)
			{
				moveV.x += 0.5f;
			}
			else if (climbNormal.m128_f32[0] == -1.0f)
			{
				moveV.z -= 0.5f;
			}

			nowMove = true;

			if (AnimationFlag == false)
			{
				this->AnimationNum = 1;
				this->AnimationFlag = true;
			}
		}
		else
		{
			nowMove = false;
		}
		
		if (Input::GetInstance()->LeftStickIn(DOWN))
		{
			moveV.y -= 0.5f;
			nowMove = true;

			if (AnimationFlag == false)
			{
				this->AnimationNum = 1;
				this->AnimationFlag = true;
			}
		}
		else if (Input::GetInstance()->LeftStickIn(UP))
		{
			moveV.y += 0.5f;
			nowMove = true;

			if (AnimationFlag == false)
			{
				this->AnimationNum = 1;
				this->AnimationFlag = true;
			}
		}

		if (Input::GetInstance()->PushKey(DIK_S))
		{
			moveV.y -= 0.5f;
			nowMove = true;
		}
		else if (Input::GetInstance()->PushKey(DIK_W))
		{
			moveV.y += 0.5f;
			nowMove = true;
		}

		if (Input::GetInstance()->PushKey(DIK_P) || Input::GetInstance()->PushPadbutton(Button_B))
		{
			climbOperation = false;
			position.x += climbNormal.m128_f32[0];
			position.z += climbNormal.m128_f32[2];
		}

		position.x += moveV.x;
		position.y += moveV.y;
		position.z += moveV.z;
	}
}

void Player::CrystalConfirmationProcess()
{
	//クリスタルとの接触
	Box playerBox;
	playerBox.centerPos = position;
	playerBox.size = { 0.5f, 1.0f, 0.5f };
	playerBox.LeastPos = XMFLOAT3(playerBox.centerPos.x - playerBox.size.x, playerBox.centerPos.y - playerBox.size.y, playerBox.centerPos.z - playerBox.size.z);
	playerBox.MaxPos = XMFLOAT3(playerBox.centerPos.x + playerBox.size.x, playerBox.centerPos.y + playerBox.size.y, playerBox.centerPos.z + playerBox.size.z);


	XMFLOAT3 distance = { 0, 0, 0 };
	Box box;
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

			OpticalPost::Erase(i);

			crystalNum--;

			//制限時間の初期化
			timeLimit = 60.0f;
		}
	}
}

void Player::GoalConfirmationProcess()
{
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

void Player::ObstacleConfirmationProcess(const XMVECTOR &move)
{
	//自機の一定の距離内の障害物を抽出し、その障害物とだけ当たり判定を取る。
	Box playerBox;
	playerBox.centerPos = position;
	playerBox.size = { 0.5f, 1.0f, 0.5f };
	playerBox.LeastPos = XMFLOAT3(playerBox.centerPos.x - playerBox.size.x, playerBox.centerPos.y - playerBox.size.y, playerBox.centerPos.z - playerBox.size.z);
	playerBox.MaxPos = XMFLOAT3(playerBox.centerPos.x + playerBox.size.x, playerBox.centerPos.y + playerBox.size.y, playerBox.centerPos.z + playerBox.size.z);

	//球コライダーを取得
	SphereCollider* sphereCollider = dynamic_cast<SphereCollider*>(collider);
	assert(sphereCollider);

	Ray ray;
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
			if (Collision::Check2Box(playerBox, boxs[i], distance) == true)
			{
				normal = Collision::CheckRayBoxforPlane(ray, boxs[i]);
				adhesion = true;
				PushBack(normal, distance);
				//行列の更新など
				FbxObject3d::Update();
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
				FbxObject3d::Update();
			}
		}
	}

	pos = position;
	FbxObject3d::Update();
}

void Player::GravityConfirmationProcess()
{
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
	else if (Input::GetInstance()->PushKey(DIK_SPACE) && climbOperation == false)//ジャンプ
	{
		onGround = false;
		nowMove = true;
		const float jumpVYFist = 0.3f; //ジャンプ時上向き初速
		fallV = { 0, jumpVYFist, 0,0 };
	}
	else if (Input::GetInstance()->PushPadbutton(Button_Y) && climbOperation == false)
	{
		onGround = false;
		nowMove = true;
		const float jumpVYFist = 0.3f; //ジャンプ時上向き初速
		fallV = { 0, jumpVYFist, 0,0 };
	}
	// ワールド行列更新
	UpdateWorldMatrix();
	collider->UpdateF();
}

void Player::TerrainConfirmationProcess()
{
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
	collider->UpdateF();


	//壁のぼり判定
	ClimbWallJudge(callback.move);


	//球の上端から球の下端までのレイキャスト用レイを準備
	Ray ray;
	ray.start = sphereCollider->center;
	//ray.start.m128_f32[1] += sphereCollider->GetRadius();
	ray.dir = { 0, -1, 0, 0 };
	RaycastHit raycastHit;

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
			FbxObject3d::Update();
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
			FbxObject3d::Update();
		}
	}


}

void Player::StaminaManagement(const DirectX::XMVECTOR& move)
{
	const float staminaQuantityMax = 100.0f;

	if (staminaCut == false)	//スタミナがある状態
	{
		//スタミナ残量がないかの確認
		if (staminaQuantity <= 0.0f)
		{
			staminaCut = true;
			return;
		}

		//	スタミナが使われていない または 今移動していない場合
		if (staminaBoostFlag != true || nowMove != true)
		{
			if (staminaQuantity >= staminaQuantityMax) return;
			staminaQuantity += staminaQuantityMax * (1.0f / 300.0f);
		}
		else //スタミナを消費している場合
		{
			if (staminaQuantity <= 0.0f) return;
			staminaQuantity -= staminaQuantityMax * (1.0f/ 300.0f);
		}
	}
	else						//スタミナがない場合
	{
		if (staminaQuantity >= staminaQuantityMax)
		{
			staminaCut = false;

			int stamina = (int)staminaQuantity;
			staminaQuantity = (float)stamina;


			return;
		}

		staminaQuantity += staminaQuantityMax * (2.0f / 300.0f);
	}
}

void Player::TimeManagement()
{
	//if (timeLimit <= 0)return;
	//
	//flame += 1.0f;
	//
	//if (flame != 60.0) return;
	//
	//timeLimit -= 1.0f;
	//flame = 0.0f;
}