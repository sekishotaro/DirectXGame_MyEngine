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

//���f��
FbxModel* Player::fbxModels[] = {nullptr,nullptr, nullptr, nullptr, nullptr, 
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr};

Player* Player::Create(FbxModel* model)
{
	//3D�I�u�W�F�N�g�̃C���X�^���X�𐶐�
	Player* instance = new Player();
	
	if (instance == nullptr)
	{
		return nullptr;
	}

	//������
	instance->Initialize();

	//���f���̃Z�b�g
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

	//�R���C�_�[�̒ǉ�
	float radius = 0.6f;
	//���a�������������畂�������W�������S�ɂ���
	SetCollider(new SphereCollider(XMVECTOR({ 0, radius * 1.8f, 0, 0 }), radius));

	collider->SetAttribute(COLLISION_ATTR_ALLIES);
	crystalNum = (int)JsonLoader::crystalObjects.size();
	crystalMaxNum = crystalNum;
	return;
}

void Player::Update()
{
	//�ߋ��X�e�[�^�X��ۑ�
	oldPlayerStatus = playerStatus;

	//�f�o�b�N�p�����ʒu�e���|�[�g
	if (teleportFlag == true)
	{
		position = {274.0f, 106.0f, 107.0f};
		position.y += 1.0f;
	}

	//�ړ��ʏ�����
	XMVECTOR move = { 0.0f, 0.0f, 0.0f, 0.0f};
	
	//�X�^�~�i�ɂ�鑬�x�ϐ�
	float power = 1.0f;
	
	//�ړ�����
	MoveOperation(move, power);

	//�X�^�~�i����
	StaminaManagement();

	//�⏈��
	SlopeDownhill(move, power);

	//��������
	GravityConfirmationProcess();

	//�ړ��m��
	MoveAddDetermination(move, power);

	//���ړ�����
	MoveBoxProcess(move, power);

	//OBJ�̃A�b�v�f�[�g
	UpdateWorldMatrix();

	////��Q��(AABB)�̏Փˏ���
	ObstacleConfirmationProcess(move);

	//�n�`�Ƃ̓����蔻��(���b�V���R���C�_�[)
	TerrainConfirmationProcess();

	//�R�オ��
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
	
	//�A�j���[�V��������
	AnimetionProcess();

	//�S�[���ƃv���C���[�̓����蔻��
	GoalConfirmationProcess();

	//�N���X�^���Ƃ̐ڐG
	CrystalConfirmationProcess();
	
	//��������
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
		//���n
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

	//�ړ����ĂȂ��ꍇreturn
	if (move.m128_f32[0] == 0.0f && move.m128_f32[1] == 0.0f && move.m128_f32[2] == 0.0f) return;

	//�ǂ̂ڂ�p�|���ɓ������Ă��邩�̔���
	Plate climbWall;
	Box climbWallBox;
	Ray ray;
	ray.start = XMLoadFloat3(&position);
	ray.dir = move;
	bool climbWallHit = false;
	//�����o�������x�N�g���ۑ��p
	XMVECTOR pushvec = {};


	for (int i = 0; i < (int)JsonLoader::climbWallObjects.size(); i++)
	{
		climbWall.position = XMLoadFloat3(&JsonLoader::climbWallObjects[i].get()->GetPosition());
		climbWall.size = JsonLoader::climbWallObjects[i].get()->GetScale();
		
		//�ʂ̖@�������߂� (���͌��ߑł��ł���Ă�"climbWall.obj")
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


		//�@�������߂�A���߂�͈̂�̎O�p�|���S���ŏ\��
		XMVECTOR p0_p1 = climbWall.vert2 - climbWall.vert1;
		XMVECTOR p0_p2 = climbWall.vert3 - climbWall.vert1;

		//�O�ςɂ��A 2�ӂɐ����ȃx�N�g�����Z�o����
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

	//�����蔻��ۑ�
	climbWallHitPar = climbWallHit;

	//�ǂɂ߂荞��ł���
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
	//���X�e�B�b�N
	inputX = Input::GetInstance()->LeftStickInXNum();
	inputY = Input::GetInstance()->LeftStickInYNum();
	
	//�ړ��ʏ�����
	move = { 0.0f,0.0f,0.0f,0 };

	//�X�^�~�i�؂�m�F
	if (staminaCut == true)
	{
		moveAdjustmentNum = 0.5f;
	}
	else
	{
		moveAdjustmentNum = 1.0f;
	}


	if (oldPlayerStatus == STATE_CLIFF_IDLING)													//�R�オ��
	{
		MoveClimbingCliff(move, power);
		slopeRising = false;
	}
	else if (oldPlayerStatus == STATE_CLIMBING)												//�ǂ̂ڂ�ړ�
	{
		MoveClimb(move, power);
	}
	else if (oldPlayerStatus != STATE_CLIFFUP)				//�ʏ�ړ�
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
	//�R���g���[���[���X�e�B�b�N�ɂ��v���C���[�̉�]
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

	//�ړ��x�N�g����Y�����̊p�x�ŉ�]

	XMMATRIX matRot = XMMatrixRotationY(XMConvertToRadians(rotation.y));
	move = { 0.0f,0.0f,0.1f,0 };
	move = XMVector3TransformNormal(move, matRot);

	//�X�^�~�i����
	if (Input::GetInstance()->PushPadbutton(Button_A) && StaminaUnusable() == false)
	{
		if (inputX == 0 && inputY == 0)	//�X�e�B�b�N�ړ������Ă��Ȃ��Ƃ��̓X�^�~�i������Ȃ�
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


	//�R���g���[���[
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
		else				//�_�b�V��
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
		else				//�_�b�V��
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
		else				//�_�b�V��
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
		else				//�_�b�V��
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

	//����
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


	//�N���X�^���Ƃ̐ڐG
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
	//���@����XZ���ň��̋����̒��ɏ�Q���I�u�W�F�N�g�̒��S���W��������̂��������蔻��p�̃R���e�i�Ɋi�[����
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

			//�������Ԃ̏�����
			timeLimit = timeLimitMax;
		}
	}
}

void Player::GoalConfirmationProcess()
{
	//�S�[���ƃv���C���[�̓����蔻��
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

	//���R���C�_�[���擾
	SphereCollider* sphereCollider = dynamic_cast<SphereCollider*>(collider);
	assert(sphereCollider);

	//�v���C���[�̉��������C
	Ray ray;
	ray.start = sphereCollider->center;
	ray.dir = { 0, -1.3f, 0, 0 };
	RaycastHit raycastHit;
	//�������锠
	Box box;
	for (int i = 0; i < JsonLoader::moveBoxObjects.size(); i++)
	{
		box.centerPos = JsonLoader::moveBoxObjects[i].get()->GetPosition();
		box.size = JsonLoader::moveBoxObjects[i].get()->GetScale();
		box.LeastPos = XMFLOAT3{ box.centerPos.x - box.size.x, box.centerPos.y - box.size.y, box.centerPos.z - box.size.z };
		box.MaxPos = XMFLOAT3{ box.centerPos.x + box.size.x, box.centerPos.y + box.size.y, box.centerPos.z + box.size.z };

		//�ړ����̏㕔�ɂ�铖���蔻��Ɣr��
		if (fallV.m128_f32[1] >= 0.0f) return;
		if (onGround == true && onObject == true)
		{
			//�X���[�Y�ɍ������ׂ̋z������u
			const float adsDistance = 0.6f;

			//�ڒn���ێ�
			if (Collision::CheckRayBox(ray, box) == true)
			{
				onGround = true;
				position.y = box.MaxPos.y;
				onObject = true;
				return;
			}
		}									//����Ń_�b�V���W�����v�������̓����蔻��p�ɒǉ�
		else if (fallV.m128_f32[1] <= 0.0f)//�������
		{
			if (Collision::CheckRayBox(ray, box) == true)
			{
				//���n
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

		//�ړ����̏㕔�ɂ�铖���蔻��Ɣr��
		if (fallV.m128_f32[1] >= 0.0f) return;
		if (onGround == true && onObject == true)
		{
			//�X���[�Y�ɍ������ׂ̋z������
			const float adsDistance = 0.6f;

			//�ڒn���ێ�
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
		}									//����Ń_�b�V���W�����v�������̓����蔻��p�ɒǉ�
		else if (fallV.m128_f32[1] <= 0.0f)//�������
		{
			if (Collision::CheckRayBox(ray, box) == true)
			{
				//���n
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
		//�����������x
		const float fallAcc = -0.01f;
		const float fallVYMin = -0.5f;
		//����
		fallV.m128_f32[1] = max(fallV.m128_f32[1] + fallAcc, fallVYMin);
		//�ړ�
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
		const float jumpVYFist = 0.2f; //�W�����v�����������
		fallV = { 0.0f, jumpVYFist, 0.0f };
	}
	else if (climbingKickJump()) //�ǏR����
	{
		playerStatus = STATE_WALLKICK_UP;

		onGround = false;
		onObject = false;
		const float jumpVYFist = 0.35f; //�W�����v�����������
		fallV = { 0.0f, jumpVYFist, 0.0f };
	}

	// ���[���h�s��X�V
	UpdateWorldMatrix();
	collider->UpdateF();
}

void Player::TerrainConfirmationProcess()
{
	//���R���C�_�[���擾
	SphereCollider* sphereCollider = dynamic_cast<SphereCollider*>(collider);
	assert(sphereCollider);
	sphereCollider->center.m128_f32[0] += 1.0f;
	oldSlopeFlag = slopeFlag;
	slopeFlag = false;

	//�N�G���\�R�[���o�b�N�N���X
	class PlayerQueryCallback :public QueryCallback
	{
	public:
		PlayerQueryCallback(Sphere* sphere) : sphere(sphere) {};

		//�Փˎ��R�[���o�b�N�֐�
		bool OnQueryHit(const QueryHit& info)
		{
			//���[���h�̏����
			const XMVECTOR up = { 0,1,0,0 };
			//�r�˕���
			XMVECTOR rejectDir = XMVector3Normalize(info.reject);
			//������Ɣr�˕����̊p�x����cos�l
			float cos = XMVector3Dot(rejectDir, up).m128_f32[0];

			//�n�ʔ��肵�����l�p�x
			const float threshold = cosf(XMConvertToRadians(70.0f));
			float ccos = acosf(cos);
			ccos = ccos * 180.0f / 3.1415f;

			if (10.0f <= ccos && ccos < 60.0f)
			{
				slopeFlag = true;
			}

			if (-threshold < cos && cos < threshold)
			{
				//����r�� (�����o��)
				sphere->center += info.reject;
				move += info.reject;
			}
			return true;
		}

		//�N�G���\�Ɏg�p���鋅
		Sphere* sphere = nullptr;
		//�r�˂ɂ��ړ���(�ݐϒl)
		DirectX::XMVECTOR move = {};
	};

	class PlayerQueryCallbackSlop :public QueryCallback
	{
	public:
		PlayerQueryCallbackSlop(Sphere* sphere) : sphere(sphere) {};

		//�Փˎ��R�[���o�b�N�֐�
		bool OnQueryHit(const QueryHit& info)
		{
			//���[���h�̏����
			const XMVECTOR up = { 0,1,0,0 };
			//�r�˕���
			XMVECTOR rejectDir = XMVector3Normalize(info.reject);
			//������Ɣr�˕����̊p�x����cos�l
			float cos = XMVector3Dot(rejectDir, up).m128_f32[0];

			float ccos = acosf(cos);
			ccos = ccos * 180.0f / 3.1415f;
			
			if (10.0f <= ccos && ccos < 60.0f)
			{
				slopeFlag = true;
			}
			return true;
		}

		//�N�G���\�Ɏg�p���鋅
		Sphere* sphere = nullptr;
		//�r�˂ɂ��ړ���(�ݐϒl)
		DirectX::XMVECTOR move = {};
	};

	//�N�G���\�R�[���o�b�N�̊֐��I�u�W�F�N�g
	PlayerQueryCallbackSlop callbackSlop(sphereCollider);
	//���ƒn�`�̌�����S����
	CollisionManager::GetInstance()->QuerySphere(*sphereCollider, &callbackSlop, COLLISION_ATTR_LANDSHAPE);
	
	sphereCollider->center.m128_f32[0] -= 1.0f;
	//�N�G���\�R�[���o�b�N�̊֐��I�u�W�F�N�g
	PlayerQueryCallback callback(sphereCollider);
	//���ƒn�`�̌�����S����
	CollisionManager::GetInstance()->QuerySphere(*sphereCollider, &callback, COLLISION_ATTR_LANDSHAPE);
	//�����ɂ��r�˕�������
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

	//�W�����v�����オ�ǂɓ����邩�`�F�b�N
	SphereCollider* sphereColliderJ = dynamic_cast<SphereCollider*>(collider);
	sphereColliderJ->center.m128_f32[1] += 5.0f;
	assert(sphereColliderJ);
	PlayerQueryCallback callbackJ(sphereColliderJ);
	
	//���ƒn�`�̌�����S����
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


	//�R���C�_�[�X�V
	UpdateWorldMatrix();
	collider->UpdateF();

	//�ǂ̂ڂ蔻��
	if (FallStateCheck() == false)
	{
		ClimbWallJudge(callback.move);
	}

	//�ǂ̂ڂ��ԂȂ珰�̔�����΂�
	if (playerStatus == STATE_CLIMBING)
	{
		return;
	}

	//���̏�[���狅�̉��[�܂ł̃��C�L���X�g�p���C������
	Ray ray;
	ray.start = sphereCollider->center;
	ray.dir = { 0, -1, 0, 0 };
	RaycastHit raycastHit;

	//�n�ʐڒn���
	//���b�V���R���C�_�[
	//Y��
	if (onGround && onObject == false)
	{
		//�X���[�Y�ɍ������ׂ̋z������
		const float adsDistance = 0.6f;

		//�ڒn���ێ�
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
	else if (onObject == false && slopeFlag == true)//�������
	{
		if (CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_LANDSHAPE,
			&raycastHit, sphereCollider->GetRadius() * 2.0f) == true)
		{
			//���n
			onGround = true;
			playerStatus = STATE_LANDING;
			position.y -= (raycastHit.distance - sphereCollider->GetRadius() * 2.0f);
		}
	}
	else if (onObject == false && fallV.m128_f32[1] <= 0.0f )//�������
	{
		if (CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_LANDSHAPE,
			&raycastHit, sphereCollider->GetRadius() * 2.0f) == true)
		{
			//���n
			onGround = true;
			playerStatus = STATE_LANDING;
			position.y -= (raycastHit.distance - sphereCollider->GetRadius() * 2.0f);
		}
	}

}

void Player::StaminaManagement()
{
	const float staminaQuantityMax = 100.0f;

	//�X�^�~�i�����
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


	if (staminaCut == false)	//�X�^�~�i��������
	{
		//�X�^�~�i�c�ʂ��Ȃ����̊m�F
		if (staminaQuantity <= 0.0f)
		{
			staminaCut = true;
			return;
		}

		//��
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
		else //�X�^�~�i������Ă���ꍇ
		{
			staminaRecoveryTime = 2.0f;

			if (staminaQuantity <= 0.0f) return;
			staminaQuantity -= staminaDecreaseAmount;
		}
	}
	else						//�X�^�~�i���Ȃ��ꍇ
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
	
	//���O������ړ���
	
	//������
	moveBoxFlag = false;
	moveBoxHitFlag = false;
	//�������������锠���Ȃ��Ȃ�Ԃ�
	if (JsonLoader::moveBoxObjects.size() == 0) return;

	SphereF playerSphere;
	playerSphere.center = position;
	playerSphere.radius = 1;
	for (int i = 0; i < JsonLoader::moveBoxObjects.size(); i++)
	{
		//�ړ���ړ������锠�ɂ߂荞��ł��邩
		Box box;
		box.centerPos = JsonLoader::moveBoxObjects[i].get()->GetPosition();
		box.size = JsonLoader::moveBoxObjects[i].get()->GetScale();
		box.LeastPos = XMFLOAT3(box.centerPos.x - box.size.x, box.centerPos.y - box.size.y, box.centerPos.z - box.size.z);
		box.MaxPos = XMFLOAT3(box.centerPos.x + box.size.x, box.centerPos.y + box.size.y, box.centerPos.z + box.size.z);
		if (Collision::CheckBoxDot(box, position) == false) return;
		moveBoxHitFlag = true;
		moveBoxHitNum = i;
		//��������������
		int xyz = 0;
		float moveValue = 0.0f;
		float moveValueP = 0.0f;
		moveValue = move.m128_f32[0];
		//�ړ��͔��̏�̏ꍇ�͂��Ȃ�
		if (box.centerPos.y < parPos.y) return;

		//�N���ʌ���
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
			//�ړ����̈ړ��m��
			moveBoxValue.m128_f32[xyz] = move.m128_f32[xyz] / 2.0f;
			moveBoxMax1 = MyMath::addVector(moveBoxMax1, moveBoxValue);
			box.centerPos = MyMath::addVector(box.centerPos, moveBoxValue);
			//�v���C���[�����߂�
			moveBoxValue.m128_f32[xyz] = (-1.0f) * (move.m128_f32[xyz] / 2.0f);
			if (xyz == 0)
			{
				moveBoxValue.m128_f32[2] = (-1.0f) * move.m128_f32[2];
			}
			else
			{
				moveBoxValue.m128_f32[0] = (-1.0f) * move.m128_f32[0];
			}
			
			//���@�̊p�x�ɂ��C��
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
			//�v���C���[�̉����o���A�j���[�V�����ɕς���
			animeNum = pushingWalking;
			moveBoxFlag = true;
		}
		else
		{
			//�v���C���[�����߂�
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
	//�S�[���ɐG���Ă�����J�E���g���Ȃ�
	if (goalFlag == true) return;

	//�f�o�b�O�p
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
	//�A�j���[�V�����̏�����
	AnimationFlag = false;

	//�A�j���[�V���������Ȃ��Ȃ�Ԃ�
	if (animeFlag == false)
	{
		//�S�t���[���̕ۑ�
		oldAnimeNum = animeNum;
		return;
	}

	//�O�t���[���ƈႤ�A�j���[�V�����̏ꍇ���f�������ւ���
	if (oldAnimeNum != animeNum)
	{
		switch (animeNum)
		{
		case 0:					//�A�C�h�����O
			SetModel(fbxModels[0]);
			loopPlayFlag = true;
			break;
		case 1:					//����
			SetModel(fbxModels[1]);
			loopPlayFlag = true;
			break;
		case 2:					//����
			SetModel(fbxModels[2]);
			loopPlayFlag = true;
			break;
		case 3:					//�W�����v
			SetModel(fbxModels[3]);
			loopPlayFlag = false;
			break;
		case 4:					//����
			SetModel(fbxModels[4]);
			loopPlayFlag = false;
			break;
		case 5:					//�ǂ̂ڂ�
			SetModel(fbxModels[5]);
			loopPlayFlag = true;
			break;
		case 6:					//���n
			SetModel(fbxModels[6]);
			loopPlayFlag = false;
			break;
		case 7:					//�R�Ԃ牺����A�C�h�����O
			SetModel(fbxModels[7]);
			loopPlayFlag = true;
			break;
		case 8:					//�L�b�N
			SetModel(fbxModels[8]);
			loopPlayFlag = true;
			break;
		case 9:					//�X���C�f�B���O
			SetModel(fbxModels[9]);
			//loopPlayFlag = true;
			break;
		case 10:					//��������
			SetModel(fbxModels[10]);
			loopPlayFlag = true;
			break;
		case 11:					//������
			SetModel(fbxModels[11]);
			loopPlayFlag = true;
			break;
		case 12:					//���A�C�h��
			SetModel(fbxModels[12]);
			loopPlayFlag = true;
			break;
		case 13:					//�ǏR��
			SetModel(fbxModels[13]);
			loopPlayFlag = true;
			break;
		case 14:					//�R�オ��
			SetModel(fbxModels[14]);
			loopPlayFlag = false;
			break;
		}
	}
	
	//�A�j���[�V�����Đ�
	if(animeNum != sliding) AnimationFlag = true;
	//���f���̍X�V
	FbxObject3d::Update();
	
	//���n�A�j���[�V�������I������ꍇ���n�t���O�̉���
	if (animeNum == landing)
	{
		if (AnimetionFinFlag == true)
		{
			playerStatus = STATE_IDLING;
		}
	}

	//�R�オ��A�j���[�V�����I��
	if (animeNum == hangingCliffUp)
	{
		if (AnimetionFinFlag == true)
		{
			playerStatus = STATE_IDLING;
		}
	}

	//�S�t���[���̕ۑ�
	oldAnimeNum = animeNum;
}

void Player::SlopeDownhill(DirectX::XMVECTOR& move, float& power)
{
	rotation.x = 0.0f;
	//��ɂ��邩�ǂ���

	if (slopeFlag == false) return;
	//�_�b�V�����Ă��邩�ǂ���
	if (staminaBoostFlag == true) return;
	if (playerStatus == STATE_CLIMBING) return;

	// �₪������Ȃ����ߊȈՏ��u (Todo : ��̊p�x�����߂�)
	if (position.x < 0.0f)			//����
	{
		//���@�� X����
		move.m128_f32[0] = 0.5f;
		//�������Ƃ��Ɋ��炩�ɉ����邽��
		move.m128_f32[1] = -0.9f;
		//Z�������͈ړ��\�����ړ��ʂ�ύX����\������(���藎���Ȃ��牡�ړ�)
	}
	else							//����
	{
		//���@�� -X����
		move.m128_f32[0] = -0.5f;
		//�������Ƃ��Ɋ��炩�ɉ����邽��
		move.m128_f32[1] = -0.9f;
		//Z�������͈ړ��\�����ړ��ʂ�ύX����\������(���藎���Ȃ��牡�ړ�)
	}
	
	// ���@���f���ύX
	playerStatus = STATE_SLOPE_SLIDING;

	animeNum = 9;

	//���@���f���̊p�x�ύX
	rotation.x = 45.0f;
	rotation.y = 270.0f;
}

bool Player::climbingKickJump()
{
	//�ǂɑ���Ȃ���ڐG������
	if (wallHittingFlag != true) return false;
	if (oldWallHittingFlag != false) return false;
	if (staminaBoostFlag != true) return false;
	return true;
}

void Player::climbingCliff()
{
	//�������ɑ΂��Ă̋N���オ��
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

	if (jumpWallHittingFlag != false) return;					//�W�����v�������ɕǂɂԂ����Ă���
	if (wallHittingFlag != true) return;						//�ǂɂԂ����Ă��Ȃ�
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
	
	//X�����Ŏ��@�ʒu�����̉E������
	if (box.centerPos.x < position.x)
	{
		Xflag = true;
	}
	//Z�����Ŏ��@�ʒu�����̉�����O��
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

	//�₩�畽�n�ɕς����
	if (playerStatus == STATE_RUNNING) return false;
	if (oldSlopeFlag == true && slopeFlag == false)
	{
		slopeRising = true;
	}

	if (slopeRising != true) return false;

	//�N���オ��܂ł̎���
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
	//�������^�[���X�e�[�^�X�g
	if (playerStatus == STATE_CLIFFUP) return;
	if (playerStatus == STATE_CLIFF_IDLING) return;

	if (oldPlayerStatus == STATE_WALLKICK_UP || oldPlayerStatus == STATE_WALLKICK_DOWN)
	{
		//���n����
		if (onGround == true)
		{
			playerStatus = STATE_LANDING;
			return;
		}

		//�ǏR��㏸���Ȃ�ǏR��X�e�[�^�X�ɕύX
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
