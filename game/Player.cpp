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
#include "SafeDelete.h"

using namespace DirectX;

bool Player::slopeFlag = false;

//モデル
FbxModel* Player::fbxModels[] = {nullptr,nullptr, nullptr, nullptr, nullptr, 
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr};

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
	timeLimit = timeLimitMax;
	staminaQuantity = 100.0f;
	staminaCut = false;

	FbxObject3d::Initialize();

	position = JsonLoader::goalObjects[0].get()->GetPosition();

	rotation.y = 90.0f;

	scale = { 0.01f ,0.01f ,0.01f };

	//コライダーの追加
	float radius = 0.6f;
	//半径分だけ足元から浮いた座標を球中心にする
	SetCollider(new SphereCollider(XMVECTOR({ 0, radius * 1.8f, 0, 0 }), radius));

	collider->SetAttribute(COLLISION_ATTR_ALLIES);
	crystalNum = (int)JsonLoader::crystalObjects.size();
	crystalMaxNum = crystalNum;
	return;
}

void Player::Update()
{
	//過去ステータスを保存
	oldPlayerStatus = playerStatus;

	//デバック用初期位置テレポート
	if (teleportFlag == true)
	{
		position = {274.0f, 106.0f, 107.0f};
		position.y += 1.0f;
	}

	//移動量初期化
	XMVECTOR move = { 0.0f, 0.0f, 0.0f, 0.0f};
	
	//スタミナによる速度変数
	float power = 1.0f;
	
	//移動処理
	MoveOperation(move, power);

	//スタミナ処理
	StaminaManagement();

	//坂処理
	SlopeDownhill(move, power);

	//落下処理
	GravityConfirmationProcess();

	//移動確定
	MoveAddDetermination(move, power);

	//箱移動処理
	MoveBoxProcess(move, power);

	//OBJのアップデート
	UpdateWorldMatrix();

	////障害物(AABB)の衝突処理
	ObstacleConfirmationProcess(move);

	//地形との当たり判定(メッシュコライダー)
	TerrainConfirmationProcess();

	//崖上がり
	climbingCliff();

	if (playerStatus == STATE_CLIFF_IDLING)
	{
		animeFlag = true;
		animeNum = hangingCliff;
		playerStatus = STATE_CLIFF_IDLING;
	}
	else if (playerStatus == STATE_CLIFFUP)
	{
		animeFlag = true;
		playerStatus = STATE_CLIFFUP;
		animeNum = hangingCliffUp;
	}
	else if (playerStatus == STATE_JUMP_UP || playerStatus == STATE_JUMP_DOWN)
	{
		animeFlag = true;
		animeNum = jump;
	}
	else if (playerStatus == STATE_WALLKICK_UP)
	{
		animeFlag = true;
		animeNum = wallKickUp;
	}
	else if (playerStatus == STATE_FALL)
	{
		animeFlag = true;
		animeNum = fall;
	}
	else if (playerStatus == STATE_JUMP_DOWN)
	{
		animeFlag = true;
		animeNum = fall;
	}
	else if (playerStatus == STATE_WALLKICK_DOWN)
	{
		animeFlag = true;
		animeNum = fall;
	}
	
	if (oldPlayerStatus == STATE_LANDING)
	{
		if (playerStatus == STATE_IDLING)
		{
			playerStatus = STATE_LANDING;
			animeFlag = true;
			animeNum = landing;
		}	
	}

	StatusProsecc();
	
	//アニメーション処理
	AnimetionProcess();

	//ゴールとプレイヤーの当たり判定
	GoalConfirmationProcess();

	//クリスタルとの接触
	CrystalConfirmationProcess();
	
	//制限時間
	TimeManagement();

	parPos = position;


}

void Player::OnCollision(const CollisionInfo& info)
{
}

void Player::ObjectUpdate()
{
	FbxObject3d::Update();
}

void Player::Finalize()
{
}

void Player::ReStart()
{
	goalFlag = false;
	timeLimit = timeLimitMax;
	staminaQuantity = 100.0f;
	staminaCut = false;

	crystalNum = (int)JsonLoader::crystalObjects.size();

	FbxObject3d::Initialize();

	position = JsonLoader::goalObjects[0].get()->GetPosition();
	rotation.y = 90.0f;
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
	if (playerStatus == STATE_FALL) return;

	//移動してない場合return
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
		position = position - moveV;
		climbWallHit = true;
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
		playerStatus = STATE_CLIMBING;
		wallCount = 0;
	}
}

void Player::MoveOperation(XMVECTOR& move, float& power)
{
	//左スティック
	inputX = Input::GetInstance()->LeftStickInXNum();
	inputY = Input::GetInstance()->LeftStickInYNum();
	
	//移動量初期化
	move = { 0.0f,0.0f,0.0f,0 };

	//スタミナ切れ確認
	if (staminaCut == true)
	{
		moveAdjustmentNum = 0.5f;
	}
	else
	{
		moveAdjustmentNum = 1.0f;
	}


	if (oldPlayerStatus == STATE_CLIFF_IDLING)													//崖上がり
	{
		MoveClimbingCliff(move, power);
		slopeRising = false;
	}
	else if (oldPlayerStatus == STATE_CLIMBING)												//壁のぼり移動
	{
		MoveClimb(move, power);
	}
	else if (oldPlayerStatus != STATE_CLIFFUP)				//通常移動
	{
		if (oldPlayerState == STATE_CLIFF_IDLING) return;
		if (SlopeRisingFlag() == true) return;
		if (moveLimitFlag == true) return;
		MoveNormal(move, power);
	}
}

void Player::MoveClimbingCliff(DirectX::XMVECTOR& move, float& power)
{
	if (oldPlayerStatus == STATE_CLIFFUP) return;

	if (Input::GetInstance()->TriggerPadbutton(Button_Y))
	{
		move = { 0.0f,0.0f,0.1f,0 };
		XMMATRIX matRot = XMMatrixRotationY(XMConvertToRadians(rotation.y));
		move = XMVector3TransformNormal(move, matRot);
		move.m128_f32[0] = move.m128_f32[0] * 15.0f;
		move.m128_f32[2] = move.m128_f32[2] * 15.0f;

		playerStatus = STATE_CLIFFUP;
	}
	else if (Input::GetInstance()->TriggerPadbutton(Button_A))
	{
		move.m128_f32[1] -= 3.0f;

		playerStatus = STATE_FALL;
	}
	else
	{
		playerStatus = STATE_CLIFF_IDLING;
	}
}

void Player::MoveNormal(DirectX::XMVECTOR& move, float& power)
{
	//コントローラー左スティックによるプレイヤーの回転
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

		if (rotation.y >= 360.0f)
		{
			rotation.y -= 360.0f;
		}
		if (rotation.y <= 0.0f)
		{
			rotation.y += 360.0f;
		}

	}

	//移動ベクトルをY軸回りの角度で回転

	XMMATRIX matRot = XMMatrixRotationY(XMConvertToRadians(rotation.y));
	move = { 0.0f,0.0f,0.1f,0 };
	move = XMVector3TransformNormal(move, matRot);

	//スタミナ消費
	if (Input::GetInstance()->PushPadbutton(Button_A) && StaminaUnusable() == false)
	{
		if (inputX == 0 && inputY == 0)	//スティック移動させていないときはスタミナを消費しない
		{
			staminaBoostFlag = false;
		}
		else
		{
			staminaBoostFlag = true;
			power = 2.7f * moveAdjustmentNum;
		}

	}
	else
	{
		staminaBoostFlag = false;
		power = 0.8f * moveAdjustmentNum;
	}

	if (MoveStickCheck())
	{
		animeFlag = true;
		if (staminaBoostFlag == false)
		{
			playerStatus = STATE_WALKING;
			animeNum = walking;
		}
		else
		{
			playerStatus = STATE_RUNNING;
			animeNum = running;
		}
	}
	else
	{
		move = { 0.0f,0.0f,0.0f,0 };
		playerStatus = STATE_IDLING;
		animeFlag = true;
		animeNum = idling;
	}
}

void Player::MoveClimb(DirectX::XMVECTOR& move, float& power)
{
	moveV = { 0,0,0 };

	float moveAdjustment = 0.07f;

	if (staminaQuantity <= 0.0f)
	{
		playerStatus = STATE_FALL;
		position.x += climbNormal.m128_f32[0];
		position.z += climbNormal.m128_f32[2];
		return;
	}


	//コントローラー
	if (Input::GetInstance()->LeftStickIn(LEFT))
	{
		if (climbNormal.m128_f32[2] == 1.0f)
		{
			moveV.x += moveAdjustment * power;
		}
		else if (climbNormal.m128_f32[2] == -1.0f)
		{
			moveV.x -= moveAdjustment * power;
		}

		if (climbNormal.m128_f32[0] == 1.0f)
		{
			moveV.x -= moveAdjustment * power;
		}
		else if (climbNormal.m128_f32[0] == -1.0f)
		{
			moveV.z += moveAdjustment * power;
		}

		animeFlag = true;
		if (staminaBoostFlag == true)
		{
			animeNum = climbing;
		}
		else				//ダッシュ
		{
			animeNum = climbing;
		}
	}
	else if (Input::GetInstance()->LeftStickIn(RIGHT))
	{
		if (climbNormal.m128_f32[2] == 1.0f)
		{
			moveV.x -= moveAdjustment * power;
		}
		else if (climbNormal.m128_f32[2] == -1.0f)
		{
			moveV.x += moveAdjustment * power;
		}

		if (climbNormal.m128_f32[0] == 1.0f)
		{
			moveV.x += moveAdjustment * power;
		}
		else if (climbNormal.m128_f32[0] == -1.0f)
		{
			moveV.z -= moveAdjustment * power;
		}

		animeFlag = true;
		if (staminaBoostFlag == true)
		{
			animeNum = climbing;
		}
		else				//ダッシュ
		{
			animeNum = climbing;
		}
	}
	else if (Input::GetInstance()->LeftStickIn(DOWN))
	{
		moveV.y -= moveAdjustment * power;

		animeFlag = true;
		if (staminaBoostFlag == true)
		{
			animeNum = climbing;
		}
		else				//ダッシュ
		{
			animeNum = climbing;
		}
	}
	else if (Input::GetInstance()->LeftStickIn(UP))
	{
		moveV.y += moveAdjustment * power;

		animeFlag = true;
		if (staminaBoostFlag == true)
		{
			animeNum = climbing;
		}
		else				//ダッシュ
		{
			animeNum = climbing;
		}
	}
	else
	{
		animeFlag = false;
		animeNum = climbing;
	}

	playerStatus = STATE_CLIMBING;

	//落下
	if (Input::GetInstance()->PushPadbutton(Button_A))
	{
		position.x += climbNormal.m128_f32[0];
		position.z += climbNormal.m128_f32[2];
		playerStatus = STATE_FALL;
	}

	position = position + moveV;
}

void Player::CrystalConfirmationProcess()
{
	if (playerStatus == STATE_CLIFF_IDLING || playerStatus == STATE_CLIFFUP) return;


	//クリスタルとの接触
	Box playerBox;
	playerBox.centerPos = position;
	playerBox.centerPos.y += 2.0f;
	playerBox.size = { 0.5f, 1.0f, 0.5f };
	playerBox.LeastPos = XMFLOAT3(playerBox.centerPos.x - playerBox.size.x, playerBox.centerPos.y - playerBox.size.y, playerBox.centerPos.z - playerBox.size.z);
	playerBox.MaxPos = XMFLOAT3(playerBox.centerPos.x + playerBox.size.x, playerBox.centerPos.y + playerBox.size.y, playerBox.centerPos.z + playerBox.size.z);


	XMFLOAT3 distance = { 0, 0, 0 };
	Box box;
	std::vector<Box> crystal;


	crystalGetFlag = false;
	//自機からXZ軸で一定の距離の中に障害物オブジェクトの中心座標があるものだけ当たり判定用のコンテナに格納する
	for (int i = 0; i < JsonLoader::crystalObjects.size(); i++)
	{
		box.centerPos = JsonLoader::crystalObjects[i].get()->GetPosition();
		box.size = JsonLoader::crystalColliderObjects[i].get()->GetScale();
		box.LeastPos = XMFLOAT3(box.centerPos.x - (box.size.x / 2), box.centerPos.y - (box.size.y / 2), box.centerPos.z - (box.size.z / 2));
		box.MaxPos = XMFLOAT3(box.centerPos.x + (box.size.x / 2), box.centerPos.y + (box.size.y / 2), box.centerPos.z + (box.size.z / 2));
		crystal.push_back(box);

		if (Collision::Check2Box(playerBox, crystal[i], distance) == true)
		{
			crystalGetFlag = true;
			crystalGetNum = i;

			JsonLoader::crystalObjects.erase(JsonLoader::crystalObjects.begin() + i);

			//OpticalPost::Erase(i);

			crystalNum--;

			staminaQuantity = 100.0f;

			//制限時間の初期化
			timeLimit = timeLimitMax;
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
	if (playerStatus == STATE_CLIMBING) return;

	//球コライダーを取得
	SphereCollider* sphereCollider = dynamic_cast<SphereCollider*>(collider);
	assert(sphereCollider);

	//プレイヤーの下方向レイ
	Ray ray;
	ray.start = sphereCollider->center;
	ray.dir = { 0, -1.3f, 0, 0 };
	RaycastHit raycastHit;
	//動かせる箱
	Box box;
	for (int i = 0; i < JsonLoader::moveBoxObjects.size(); i++)
	{
		box.centerPos = JsonLoader::moveBoxObjects[i].get()->GetPosition();
		box.size = JsonLoader::moveBoxObjects[i].get()->GetScale();
		box.LeastPos = XMFLOAT3{ box.centerPos.x - box.size.x, box.centerPos.y - box.size.y, box.centerPos.z - box.size.z };
		box.MaxPos = XMFLOAT3{ box.centerPos.x + box.size.x, box.centerPos.y + box.size.y, box.centerPos.z + box.size.z };

		//移動箱の上部による当たり判定と排斥
		if (fallV.m128_f32[1] >= 0.0f) return;
		if (onGround == true && onObject == true)
		{
			//スムーズに坂を下る為の吸着距離u
			const float adsDistance = 0.6f;

			//接地を維持
			if (Collision::CheckRayBox(ray, box) == true)
			{
				onGround = true;
				position.y = box.MaxPos.y;
				onObject = true;
				return;
			}
		}									//↓坂でダッシュジャンプした時の当たり判定用に追加
		else if (fallV.m128_f32[1] <= 0.0f)//落下状態
		{
			if (Collision::CheckRayBox(ray, box) == true)
			{
				//着地
				onGround = true;
				onObject = true;
				playerStatus = STATE_LANDING;
				position.y = box.MaxPos.y;
				return;
			}
		}
	}

	for (int i = 0; i < JsonLoader::terrainObjects.size(); i++)
	{
		box.centerPos = JsonLoader::terrainObjects[i].get()->GetPosition();
		box.size = JsonLoader::terrainObjects[i].get()->GetScale();
		box.LeastPos = XMFLOAT3{ box.centerPos.x - box.size.x, box.centerPos.y - box.size.y, box.centerPos.z - box.size.z };
		box.MaxPos = XMFLOAT3{ box.centerPos.x + box.size.x, box.centerPos.y + box.size.y, box.centerPos.z + box.size.z };

		//移動箱の上部による当たり判定と排斥
		if (fallV.m128_f32[1] >= 0.0f) return;
		if (onGround == true && onObject == true)
		{
			//スムーズに坂を下る為の吸着距離
			const float adsDistance = 0.6f;

			//接地を維持
			if (Collision::CheckRayBox(ray, box) == true)
			{
				onGround = true;
				position.y = box.MaxPos.y;
				onObject = true;
				return;
			}
			else
			{
				onGround = false;
				onObject = false;
			}
		}									//↓坂でダッシュジャンプした時の当たり判定用に追加
		else if (fallV.m128_f32[1] <= 0.0f)//落下状態
		{
			if (Collision::CheckRayBox(ray, box) == true)
			{
				//着地
				onGround = true;
				onObject = true;
				playerStatus = STATE_LANDING;
				position.y = box.MaxPos.y;
				return;
			}
		}
	}
}

void Player::GravityConfirmationProcess()
{
	if(playerStatus == STATE_CLIFF_IDLING || playerStatus == STATE_CLIFFUP)
	{
		fallV = { 0, 0.0f, 0,0 };
	}
	else if (onGround == false && playerStatus != STATE_CLIMBING)
	{
		//下向き加速度
		const float fallAcc = -0.01f;
		const float fallVYMin = -0.5f;
		//加速
		fallV.m128_f32[1] = max(fallV.m128_f32[1] + fallAcc, fallVYMin);
		//移動
		position = MyMath::addVector(position, fallV);

		if (fallV.m128_f32[1] < -0.2f)
		{
			playerStatus = STATE_FALL;
			return;
		}

		if (fallV.m128_f32[1] >= 0.0f && oldPlayerStatus == STATE_JUMP_UP)
		{
			playerStatus = STATE_JUMP_UP;
		}
		else if (fallV.m128_f32[1] < 0.0f && oldPlayerStatus == STATE_JUMP_UP)
		{
			playerStatus = STATE_JUMP_DOWN;
		}
		else if (fallV.m128_f32[1] < 0.0f && oldPlayerStatus == STATE_JUMP_DOWN)
		{
			playerStatus = STATE_JUMP_DOWN;
		}
		else if (fallV.m128_f32[1] >= 0.0f && oldPlayerStatus == STATE_WALLKICK_UP)
		{
			playerStatus = STATE_WALLKICK_UP;
		}
		else if (fallV.m128_f32[1] < 0.0f && oldPlayerStatus == STATE_WALLKICK_UP)
		{
			playerStatus = STATE_WALLKICK_DOWN;
		}
		else if (fallV.m128_f32[1] < 0.0f && oldPlayerStatus == STATE_WALLKICK_DOWN)
		{
			playerStatus = STATE_WALLKICK_DOWN;
		}
		else
		{
			playerStatus = STATE_FALL;
		}

	}
	else if (Input::GetInstance()->TriggerPadbutton(Button_Y) && playerStatus != STATE_CLIMBING)
	{
		playerStatus = STATE_JUMP_UP;
		onGround = false;
		onObject = false;
		//nowMove = true;
		const float jumpVYFist = 0.2f; //ジャンプ時上向き初速
		fallV = { 0.0f, jumpVYFist, 0.0f };
	}
	else if (climbingKickJump()) //壁蹴り上り
	{
		playerStatus = STATE_WALLKICK_UP;

		onGround = false;
		onObject = false;
		const float jumpVYFist = 0.35f; //ジャンプ時上向き初速
		fallV = { 0.0f, jumpVYFist, 0.0f };
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
	sphereCollider->center.m128_f32[0] += 1.0f;
	oldSlopeFlag = slopeFlag;
	slopeFlag = false;

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
			const float threshold = cosf(XMConvertToRadians(70.0f));
			float ccos = acosf(cos);
			ccos = ccos * 180.0f / 3.1415f;

			if (10.0f <= ccos && ccos < 60.0f)
			{
				slopeFlag = true;
			}

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

	class PlayerQueryCallbackSlop :public QueryCallback
	{
	public:
		PlayerQueryCallbackSlop(Sphere* sphere) : sphere(sphere) {};

		//衝突時コールバック関数
		bool OnQueryHit(const QueryHit& info)
		{
			//ワールドの上方向
			const XMVECTOR up = { 0,1,0,0 };
			//排斥方向
			XMVECTOR rejectDir = XMVector3Normalize(info.reject);
			//上方向と排斥方向の角度差のcos値
			float cos = XMVector3Dot(rejectDir, up).m128_f32[0];

			float ccos = acosf(cos);
			ccos = ccos * 180.0f / 3.1415f;
			
			if (10.0f <= ccos && ccos < 60.0f)
			{
				slopeFlag = true;
			}
			return true;
		}

		//クエリ―に使用する球
		Sphere* sphere = nullptr;
		//排斥による移動量(累積値)
		DirectX::XMVECTOR move = {};
	};

	//クエリ―コールバックの関数オブジェクト
	PlayerQueryCallbackSlop callbackSlop(sphereCollider);
	//球と地形の交差を全検索
	CollisionManager::GetInstance()->QuerySphere(*sphereCollider, &callbackSlop, COLLISION_ATTR_LANDSHAPE);
	
	sphereCollider->center.m128_f32[0] -= 1.0f;
	//クエリ―コールバックの関数オブジェクト
	PlayerQueryCallback callback(sphereCollider);
	//球と地形の交差を全検索
	CollisionManager::GetInstance()->QuerySphere(*sphereCollider, &callback, COLLISION_ATTR_LANDSHAPE);
	//交差による排斥分動かす
	position.x += callback.move.m128_f32[0];
	position.y += callback.move.m128_f32[1];
	position.z += callback.move.m128_f32[2];
	oldWallHittingFlag = wallHittingFlag;
	if (callback.move.m128_f32[0] != 0.0f)
	{
		wallHittingFlag = true;
	}
	else if (callback.move.m128_f32[2] != 0.0f)
	{
		wallHittingFlag = true;
	}
	else
	{
		wallHittingFlag = false;
	}

	//ジャンプした上が壁に当たるかチェック
	SphereCollider* sphereColliderJ = dynamic_cast<SphereCollider*>(collider);
	sphereColliderJ->center.m128_f32[1] += 5.0f;
	assert(sphereColliderJ);
	PlayerQueryCallback callbackJ(sphereColliderJ);
	
	//球と地形の交差を全検索
	CollisionManager::GetInstance()->QuerySphere(*sphereColliderJ, &callbackJ, COLLISION_ATTR_LANDSHAPE);
	bool oldJumpWallHittingFlag = jumpWallHittingFlag;
	if (callbackJ.move.m128_f32[0] != 0.0f)
	{
		jumpWallHittingFlag = true;
	}
	else if (callbackJ.move.m128_f32[0] != 0.0f)
	{
		jumpWallHittingFlag = true;
	}
	else if (callbackJ.move.m128_f32[0] != 0.0f)
	{
		jumpWallHittingFlag = true;
	}
	else
	{
		jumpWallHittingFlag = false;
	}

	if (wallHittingFlag == true && jumpWallHittingFlag == false)
	{
		if (oldJumpWallHittingFlag == true)
		{
			jumpHeightPosY = sphereColliderJ->center.m128_f32[1];
		}
	}


	//コライダー更新
	UpdateWorldMatrix();
	collider->UpdateF();

	//壁のぼり判定
	if (FallStateCheck() == false)
	{
		ClimbWallJudge(callback.move);
	}

	//壁のぼり状態なら床の判定を飛ばす
	if (playerStatus == STATE_CLIMBING)
	{
		return;
	}

	//球の上端から球の下端までのレイキャスト用レイを準備
	Ray ray;
	ray.start = sphereCollider->center;
	ray.dir = { 0, -1, 0, 0 };
	RaycastHit raycastHit;

	//地面接地状態
	//メッシュコライダー
	//Y軸
	if (onGround && onObject == false)
	{
		//スムーズに坂を下る為の吸着距離
		const float adsDistance = 0.6f;

		//接地を維持
		if (CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_LANDSHAPE,
			&raycastHit, sphereCollider->GetRadius() * 2.0f + adsDistance) == true)
		{
			onGround = true;
			position.y -= (raycastHit.distance - sphereCollider->GetRadius() * 2.0f);
		}
		else
		{
			onGround = false;
			fallV = {};
		}
	}
	else if (onObject == false && slopeFlag == true)//落下状態
	{
		if (CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_LANDSHAPE,
			&raycastHit, sphereCollider->GetRadius() * 2.0f) == true)
		{
			//着地
			onGround = true;
			playerStatus = STATE_LANDING;
			position.y -= (raycastHit.distance - sphereCollider->GetRadius() * 2.0f);
		}
	}
	else if (onObject == false && fallV.m128_f32[1] <= 0.0f )//落下状態
	{
		if (CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_LANDSHAPE,
			&raycastHit, sphereCollider->GetRadius() * 2.0f) == true)
		{
			//着地
			onGround = true;
			playerStatus = STATE_LANDING;
			position.y -= (raycastHit.distance - sphereCollider->GetRadius() * 2.0f);
		}
	}

}

void Player::StaminaManagement()
{
	const float staminaQuantityMax = 100.0f;

	//スタミナ消費量
	float staminaDecreaseAmount = 0.0f;
	if (playerStatus == STATE_CLIMBING)
	{
		staminaDecreaseAmount = staminaQuantityMax * (1.0f / 700.0f);
	}

	else if (moveBoxFlag == true)
	{
		staminaDecreaseAmount = staminaQuantityMax * (1.0f / 400.0f);
	}
	else
	{
		staminaDecreaseAmount = staminaQuantityMax * (1.0f / 300.0f);
	}


	if (staminaCut == false)	//スタミナがある状態
	{
		//スタミナ残量がないかの確認
		if (staminaQuantity <= 0.0f)
		{
			staminaCut = true;
			return;
		}

		//回復
		if (StaminaConsumptionFlag() == true)
		{
			if (staminaRecoveryTime >= 0.0f)
			{
				staminaRecoveryTime -= 1.0f / 60.0f;
				return;
			}

			if (staminaQuantity >= staminaQuantityMax) return;
			staminaQuantity += staminaQuantityMax * (1.0f / 300.0f);
		}
		else if (playerStatus == STATE_CLIFF_IDLING || playerStatus == STATE_CLIFFUP)
		{
			staminaRecoveryTime = 2.0f;
		}
		else //スタミナを消費している場合
		{
			staminaRecoveryTime = 2.0f;

			if (staminaQuantity <= 0.0f) return;
			staminaQuantity -= staminaDecreaseAmount;
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
		if (playerStatus == STATE_WALKING || playerStatus == STATE_RUNNING)
		{
			playerStatus = STATE_TIRED_WALKING;
			animeNum = tiredWalking;
		}
		else if(playerStatus == STATE_IDLING)
		{
			playerStatus = STATE_TIRED_IDLING;
			animeNum = tiredIdling;
		}
		
	}
}

void Player::MoveBoxProcess(DirectX::XMVECTOR& move, float& power)
{
	if (playerStatus == STATE_CLIFF_IDLING || playerStatus == STATE_CLIFFUP) return;
	
	//※前提条件移動後
	
	//初期化
	moveBoxFlag = false;
	moveBoxHitFlag = false;
	//そもそも押せる箱がないなら返す
	if (JsonLoader::moveBoxObjects.size() == 0) return;

	SphereF playerSphere;
	playerSphere.center = position;
	playerSphere.radius = 1;
	for (int i = 0; i < JsonLoader::moveBoxObjects.size(); i++)
	{
		//移動後移動させる箱にめり込んでいるか
		Box box;
		box.centerPos = JsonLoader::moveBoxObjects[i].get()->GetPosition();
		box.size = JsonLoader::moveBoxObjects[i].get()->GetScale();
		box.LeastPos = XMFLOAT3(box.centerPos.x - box.size.x, box.centerPos.y - box.size.y, box.centerPos.z - box.size.z);
		box.MaxPos = XMFLOAT3(box.centerPos.x + box.size.x, box.centerPos.y + box.size.y, box.centerPos.z + box.size.z);
		if (Collision::CheckBoxDot(box, position) == false) return;
		moveBoxHitFlag = true;
		moveBoxHitNum = i;
		//方向を検索する
		int xyz = 0;
		float moveValue = 0.0f;
		float moveValueP = 0.0f;
		moveValue = move.m128_f32[0];
		//移動は箱の上の場合はしない
		if (box.centerPos.y < parPos.y) return;

		//侵入面検索
		float xmin = position.x - box.LeastPos.x;
		float xmax = box.MaxPos.x - position.x;
		float zmin = position.z - box.LeastPos.z;
		float zmax = box.MaxPos.z - position.z;
		bool direction = false;
		if (xmin < 0.0f)
		{
			xmin *= -1.0f;
		}
		if (xmax < 0.0f)
		{
			xmax *= -1.0f;
		}
		if (zmin < 0.0f)
		{
			zmin *= -1.0f;
		}
		if (zmax < 0.0f)
		{
			zmax *= -1.0f;
		}

		if (xmin < xmax)
		{
			if (zmin < zmax)
			{
				if (xmin < zmin)
				{
					xyz = 0;
					direction = true;
				}
				else
				{
					xyz = 2;
					direction = true;
				}
			}
			else
			{
				if (xmin < zmax)
				{
					xyz = 0;
					direction = true;
				}
				else
				{
					xyz = 2;
					direction = false;
				}
			}

		}
		else
		{
			if (zmin < zmax)
			{
				if (xmax < zmin)
				{
					xyz = 0;
					direction = false;
				}
				else
				{
					xyz = 2;
					direction = true;
				}
			}
			else
			{
				if (xmax < zmax)
				{
					xyz = 0;
					direction = false;
				}
				else
				{
					xyz = 2;
					direction = false;
				}
			}
		}

		
		XMVECTOR moveBoxValue = {};
		if (Input::GetInstance()->PushPadbutton(Button_A) && MoveBoxConditionFlag() == true)
		{
			//移動箱の移動確定
			moveBoxValue.m128_f32[xyz] = move.m128_f32[xyz] / 2.0f;
			moveBoxMax1 = MyMath::addVector(moveBoxMax1, moveBoxValue);
			box.centerPos = MyMath::addVector(box.centerPos, moveBoxValue);
			//プレイヤー押し戻し
			moveBoxValue.m128_f32[xyz] = (-1.0f) * (move.m128_f32[xyz] / 2.0f);
			if (xyz == 0)
			{
				moveBoxValue.m128_f32[2] = (-1.0f) * move.m128_f32[2];
			}
			else
			{
				moveBoxValue.m128_f32[0] = (-1.0f) * move.m128_f32[0];
			}
			
			//自機の角度による修正
			if (xyz == 0 && direction == true)
			{
				rotation.y = 90.0f;
			}
			else if (xyz == 0 && direction == false)
			{
				rotation.y = 270.0f;
			}
			else if (xyz == 2 && direction == true)
			{
				rotation.y = 0.0f;
			}
			else if (xyz == 2 && direction == false)
			{
				rotation.y = 180.0f;
			}
			//プレイヤーの押し出しアニメーションに変える
			animeNum = pushingWalking;
			moveBoxFlag = true;
		}
		else
		{
			//プレイヤー押し戻し
			moveBoxValue.m128_f32[xyz] = (-1.0f) * (move.m128_f32[xyz]);
		}
		
		JsonLoader::moveBoxObjects[i].get()->SetPosition(box.centerPos);
		position = MyMath::addVector(position, moveBoxValue);
		playerStatus = STATE_PUSHBOX;
		return;
	}
	
}

void Player::TimeManagement()
{
	//ゴールに触っていたらカウントしない
	if (goalFlag == true) return;

	//デバッグ用
	if (timeLimitcancel == true)
	{
		timeLimit = timeLimitMax;
		return;
	}
	

	if (timeLimit < 0.0f) return;

	timeLimit -= 1.0f / 60.0f;
}

void Player::AnimetionProcess()
{
	//アニメーションの初期化
	AnimationFlag = false;

	//アニメーションをしないなら返す
	if (animeFlag == false)
	{
		//全フレームの保存
		oldAnimeNum = animeNum;
		return;
	}

	//前フレームと違うアニメーションの場合モデルを入れ替える
	if (oldAnimeNum != animeNum)
	{
		switch (animeNum)
		{
		case 0:					//アイドリング
			SetModel(fbxModels[0]);
			loopPlayFlag = true;
			break;
		case 1:					//歩き
			SetModel(fbxModels[1]);
			loopPlayFlag = true;
			break;
		case 2:					//走り
			SetModel(fbxModels[2]);
			loopPlayFlag = true;
			break;
		case 3:					//ジャンプ
			SetModel(fbxModels[3]);
			loopPlayFlag = false;
			break;
		case 4:					//落下
			SetModel(fbxModels[4]);
			loopPlayFlag = false;
			break;
		case 5:					//壁のぼり
			SetModel(fbxModels[5]);
			loopPlayFlag = true;
			break;
		case 6:					//着地
			SetModel(fbxModels[6]);
			loopPlayFlag = false;
			break;
		case 7:					//崖ぶら下がりアイドリング
			SetModel(fbxModels[7]);
			loopPlayFlag = true;
			break;
		case 8:					//キック
			SetModel(fbxModels[8]);
			loopPlayFlag = true;
			break;
		case 9:					//スライディング
			SetModel(fbxModels[9]);
			//loopPlayFlag = true;
			break;
		case 10:					//推し歩き
			SetModel(fbxModels[10]);
			loopPlayFlag = true;
			break;
		case 11:					//疲れ歩き
			SetModel(fbxModels[11]);
			loopPlayFlag = true;
			break;
		case 12:					//疲れアイドル
			SetModel(fbxModels[12]);
			loopPlayFlag = true;
			break;
		case 13:					//壁蹴り
			SetModel(fbxModels[13]);
			loopPlayFlag = true;
			break;
		case 14:					//崖上がり
			SetModel(fbxModels[14]);
			loopPlayFlag = false;
			break;
		}
	}
	
	//アニメーション再生
	if(animeNum != sliding) AnimationFlag = true;
	//モデルの更新
	FbxObject3d::Update();
	
	//着地アニメーションが終わった場合着地フラグの解消
	if (animeNum == landing)
	{
		if (AnimetionFinFlag == true)
		{
			playerStatus = STATE_IDLING;
		}
	}

	//崖上がりアニメーション終了
	if (animeNum == hangingCliffUp)
	{
		if (AnimetionFinFlag == true)
		{
			playerStatus = STATE_IDLING;
		}
	}

	//全フレームの保存
	oldAnimeNum = animeNum;
}

void Player::SlopeDownhill(DirectX::XMVECTOR& move, float& power)
{
	rotation.x = 0.0f;
	//坂にいるかどうか

	if (slopeFlag == false) return;
	//ダッシュしているかどうか
	if (staminaBoostFlag == true) return;
	if (playerStatus == STATE_CLIMBING) return;

	// 坂が二つしかないため簡易処置 (Todo : 坂の角度を求める)
	if (position.x < 0.0f)			//小坂
	{
		//自機を X方向
		move.m128_f32[0] = 0.5f;
		//坂を下るときに滑らかに下がるため
		move.m128_f32[1] = -0.9f;
		//Z軸方向は移動可能だが移動量を変更する可能性あり(滑り落ちながら横移動)
	}
	else							//高坂
	{
		//自機を -X方向
		move.m128_f32[0] = -0.5f;
		//坂を下るときに滑らかに下がるため
		move.m128_f32[1] = -0.9f;
		//Z軸方向は移動可能だが移動量を変更する可能性あり(滑り落ちながら横移動)
	}
	
	// 自機モデル変更
	playerStatus = STATE_SLOPE_SLIDING;

	animeNum = 9;

	//自機モデルの角度変更
	rotation.x = 45.0f;
	rotation.y = 270.0f;
}

bool Player::climbingKickJump()
{
	//壁に走りながら接触したか
	if (wallHittingFlag != true) return false;
	if (oldWallHittingFlag != false) return false;
	if (staminaBoostFlag != true) return false;
	return true;
}

void Player::climbingCliff()
{
	//動く箱に対しての起き上がり
	if (moveBoxHitFlag == true)
	{
		if (playerStatus == STATE_CLIFFUP) return;

		if (JsonLoader::moveBoxObjects[moveBoxHitNum].get()->GetPosition().y <= position.y) return;

		if(Input::GetInstance()->TriggerPadbutton(Button_Y) && StaminaUnusable() == false)
		{
			playerStatus = STATE_CLIFFUP;
			position.y = JsonLoader::moveBoxObjects[moveBoxHitNum].get()->GetPosition().y + JsonLoader::moveBoxObjects[moveBoxHitNum].get()->GetScale().y;
		}
	}

	if (jumpWallHittingFlag != false) return;					//ジャンプした時に壁にぶつかっている
	if (wallHittingFlag != true) return;						//壁にぶつかっていない
	if (playerStatus != STATE_JUMP_DOWN && playerStatus != STATE_WALLKICK_DOWN) return;
	bool climbingCliffBoxHitFlag = false;
	int number = 0;
	for (int i = 0; i < JsonLoader::cliffClimbingObjects.size(); i++)
	{
		Box box;
		box.centerPos = JsonLoader::cliffClimbingObjects[i].get()->GetPosition();
		box.size = JsonLoader::cliffClimbingObjects[i].get()->GetScale();
		box.LeastPos = XMFLOAT3(box.centerPos.x - box.size.x, box.centerPos.y - box.size.y, box.centerPos.z - box.size.z);
		box.MaxPos = XMFLOAT3(box.centerPos.x + box.size.x, box.centerPos.y + box.size.y, box.centerPos.z + box.size.z);

		if (Collision::CheckBoxDot(box, position) == true)
		{
			
			climbingCliffBoxHitFlag = true;
			number = i;
			climbingCliffBoxNum = i;
			break;
		}
	}
	
	if (climbingCliffBoxHitFlag != true) return;
	playerStatus = STATE_CLIFF_IDLING;
	position.y = JsonLoader::cliffClimbingObjects[number].get()->GetPosition().y + JsonLoader::cliffClimbingObjects[number].get()->GetScale().y;

	return;
}

bool Player::StaminaUnusable()
{
	if (staminaCut == false && moveBoxFlag == false) return false;
	return true;
}

void Player::MoveAddDetermination(DirectX::XMVECTOR& move, float& power)
{
	if (playerStatus == STATE_IDLING) return;
	if (slopeFlag == true && staminaBoostFlag == false)
	{
		move.m128_f32[0] = move.m128_f32[0];
		move.m128_f32[2] = move.m128_f32[2];
	}
	else
	{
		move.m128_f32[0] = move.m128_f32[0] * power;
		move.m128_f32[2] = move.m128_f32[2] * power;
	}
	moveVal = {};
	moveVal = MyMath::addVector(moveVal, move);
	position = MyMath::addVector(position, move);
}

void Player::BoxInMove()
{
	Box box;
	box.centerPos = JsonLoader::cliffClimbingObjects[climbingCliffBoxNum].get()->GetPosition();
	box.size = JsonLoader::cliffClimbingObjects[climbingCliffBoxNum].get()->GetScale();
	box.LeastPos = XMFLOAT3(box.centerPos.x - box.size.z, box.centerPos.y - box.size.y, box.centerPos.z - box.size.x);
	box.MaxPos = XMFLOAT3(box.centerPos.x + box.size.z, box.centerPos.y + box.size.y, box.centerPos.z + box.size.x);
	if (moveBoxHitFlag == true)
	{
		box.centerPos = JsonLoader::moveBoxObjects[moveBoxHitNum].get()->GetPosition();
		box.size = JsonLoader::moveBoxObjects[moveBoxHitNum].get()->GetScale();
		box.LeastPos = XMFLOAT3(box.centerPos.x - box.size.z, box.centerPos.y - box.size.y, box.centerPos.z - box.size.x);
		box.MaxPos = XMFLOAT3(box.centerPos.x + box.size.z, box.centerPos.y + box.size.y, box.centerPos.z + box.size.x);
	}
	
	bool Xflag = false;
	bool Zflag = false;
	
	//X方向で自機位置が箱の右か左か
	if (box.centerPos.x < position.x)
	{
		Xflag = true;
	}
	//Z方向で自機位置が箱の奥か手前か
	if (box.centerPos.z < position.z)
	{
		Zflag = true;
	}

	if (Xflag == true)
	{
		position.x -= 1.5f;
		return;
	}
	else
	{
		position.x += 1.5f;
		return;
	}

	if (Zflag == true)
	{
		position.z -= 1.5f;
		return;
	}
	else
	{
		position.z += 1.5f;
		return;
	}


}

bool Player::SlopeRisingFlag()
{
	const float timeMax = 1.0f;
	static float countTime = timeMax;

	//坂から平地に変わった
	if (playerStatus == STATE_RUNNING) return false;
	if (oldSlopeFlag == true && slopeFlag == false)
	{
		slopeRising = true;
	}

	if (slopeRising != true) return false;

	//起き上がるまでの時間
	if (TimeCheck(countTime) == true)
	{
		slopeRising = false;
		countTime = timeMax;
		return false;
	}

	return true;
}

bool Player::StaminaConsumptionFlag()
{
	if (playerStatus == STATE_IDLING) return true;
	if (moveBoxFlag == true) return false;
	if (staminaBoostFlag == false && playerStatus != STATE_CLIMBING) return true;
	return false;
}

bool Player::MoveBoxConditionFlag()
{
	if (StaminaUnusable() != false) return false;
	if (playerStatus == STATE_JUMP_UP || playerStatus == STATE_JUMP_DOWN) return false;
	if (FallStateCheck() == true) return false;
	return true;
}

bool Player::MoveStickCheck()
{
	if (Input::GetInstance()->LeftStickIn(LEFT)) return true;
	if (Input::GetInstance()->LeftStickIn(RIGHT)) return true;
	if (Input::GetInstance()->LeftStickIn(UP)) return true;
	if (Input::GetInstance()->LeftStickIn(DOWN)) return true;
	return false;
}

bool Player::FallStateCheck()
{
	if (playerStatus == STATE_FALL) return true;
	if (playerStatus == STATE_JUMP_DOWN) return true;
	if (playerStatus == STATE_WALLKICK_DOWN) return true;
	return false;
}

bool Player::TimeCheck(float& time)
{
	float flame = 60.0f;
	time -= 1.0f / flame;

	if (time <= 0.0f) return true;
	return false;
}

void Player::StatusProsecc()
{
	//早期リターンステータス組
	if (playerStatus == STATE_CLIFFUP) return;
	if (playerStatus == STATE_CLIFF_IDLING) return;

	if (oldPlayerStatus == STATE_WALLKICK_UP || oldPlayerStatus == STATE_WALLKICK_DOWN)
	{
		//着地判定
		if (onGround == true)
		{
			playerStatus = STATE_LANDING;
			return;
		}

		//壁蹴り上昇中なら壁蹴りステータスに変更
		if (fallV.m128_f32[1] > 0.0f)
		{
			playerStatus = STATE_WALLKICK_UP;
		}
		else
		{
			playerStatus = STATE_WALLKICK_DOWN;
		}
	}
}
