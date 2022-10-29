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
	//3D�I�u�W�F�N�g�̃C���X�^���X�𐶐�
	Player* instance = new Player();
	
	if (instance == nullptr)
	{
		return nullptr;
	}

	//������
	if (!instance->Initialize())
	{
		delete instance;
		assert(0);
	}

	//���f���̃Z�b�g
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

	//�R���C�_�[�̒ǉ�
	float radius = 0.6f;
	//���a�������������畂�������W�������S�ɂ���
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
		//A,D�Ő���
		if (input->PushKey(DIK_A))
		{
			rotation.y -= 2.0f;
		}
		else if (input->PushKey(DIK_D))
		{
			rotation.y += 2.0f;
		}

		//�ړ��x�N�g����Y�����̊p�x�ŉ�]
		
		XMMATRIX matRot = XMMatrixRotationY(XMConvertToRadians(rotation.y));
		move = XMVector3TransformNormal(move, matRot);
		float power = 1.0f;
		//�����Ă�������Ɉړ�
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
			if (climbNormal.m128_f32[2] == 1.0f)
			{
				position.x += 1.0f;
			}
			else if (climbNormal.m128_f32[2] == -1.0f)
			{
				position.x -= 1.0f;
			}

			if (climbNormal.m128_f32[0] == 1.0f)
			{
				position.x -= 1.0f;
			}
			else if (climbNormal.m128_f32[0] == -1.0f)
			{
				position.z += 1.0f;
			}
		}
		else if (input->PushKey(DIK_D))
		{
			if (climbNormal.m128_f32[2] == 1.0f)
			{
				position.x -= 1.0f;
			}
			else if (climbNormal.m128_f32[2] == -1.0f)
			{
				position.x += 1.0f;
			}

			if (climbNormal.m128_f32[0] == 1.0f)
			{
				position.x += 1.0f;
			}
			else if (climbNormal.m128_f32[0] == -1.0f)
			{
				position.z -= 1.0f;
			}
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

	//��������
	if (!onGround && climbOperation == false)
	{
		//�����������x
		const float fallAcc = -0.01f;
		const float fallVYMin = -0.5f;
		//����
		fallV.m128_f32[1] = max(fallV.m128_f32[1] + fallAcc, fallVYMin);

		//�ړ�
		position.x += fallV.m128_f32[0];
		position.y += fallV.m128_f32[1];
		position.z += fallV.m128_f32[2];
	}
	else if (Input::GetInstance()->TriggerKey(DIK_SPACE) && climbOperation == false)//�W�����v
	{
		onGround = false;
		nowMove = true;
		const float jumpVYFist = 1.5f; //�W�����v�����������
		 fallV = { 0, jumpVYFist, 0,0 };
	}
	// ���[���h�s��X�V
	UpdateWorldMatrix();
	collider->Update();


	//���R���C�_�[���擾
	SphereCollider* sphereCollider = dynamic_cast<SphereCollider*>(collider);
	assert(sphereCollider);

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
			const float threshold = cosf(XMConvertToRadians(45.0f));
			//�p�x���ɂ���ēV��܂��͒n�ʂƔ��肳�����̂�������
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

	//�N�G���\�R�[���o�b�N�̊֐��I�u�W�F�N�g
	PlayerQueryCallback callback(sphereCollider);

	//���ƒn�`�̌�����S����
	CollisionManager::GetInstance()->QuerySphere(*sphereCollider, &callback, COLLISION_ATTR_LANDSHAPE);
	//�����ɂ��r�˕�������
	position.x += callback.move.m128_f32[0];
	position.y += callback.move.m128_f32[1];
	position.z += callback.move.m128_f32[2];
	//�R���C�_�[�X�V
	UpdateWorldMatrix();
	collider->Update();


	//�ǂ̂ڂ蔻��
	ClimbWallJudge(callback.move);


	//���̏�[���狅�̉��[�܂ł̃��C�L���X�g�p���C������
	Ray ray;
	ray.start = sphereCollider->center;
	//ray.start.m128_f32[1] += sphereCollider->GetRadius();
	ray.dir = { 0, -1, 0, 0 };
	RaycastHit raycastHit;

	pos = position;
	moveV.x = move.m128_f32[0];
	moveV.y = move.m128_f32[1];
	moveV.z = move.m128_f32[2];

	//�n�ʐڒn���
	//���b�V���R���C�_�[
	//Y��
	if (onGround && onObject == false)
	{
		//�X���[�Y�ɍ������ׂ̋z������
		const float adsDistance = 0.2f;

		//�ڒn���ێ�
		if (CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_LANDSHAPE,
			&raycastHit, sphereCollider->GetRadius() * 2.0f + adsDistance) == true)
		{
			onGround = true;
			position.y -= (raycastHit.distance - sphereCollider->GetRadius() * 2.0f);
			//�s��̍X�V�Ȃ�
			Object3d::Update();
		}
		else
		{
			onGround = false;
			fallV = {};
		}
	}
	else if (fallV.m128_f32[1] <= 0.0f)//�������
	{
		nowMove = true;
		if (CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_LANDSHAPE,
			&raycastHit, sphereCollider->GetRadius() * 2.0f) == true)
		{
			//���n
			onGround = true;
			position.y -= (raycastHit.distance - sphereCollider->GetRadius() * 2.0f);
			//�s��̍X�V�Ȃ�
			Object3d::Update();
		}
	}


	//���@�̈��̋������̏�Q���𒊏o���A���̏�Q���Ƃ��������蔻������B
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

	//���@����XZ���ň��̋����̒��ɏ�Q���I�u�W�F�N�g�̒��S���W��������̂��������蔻��p�̃R���e�i�Ɋi�[����
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
		//��Q��
		if (adhesion)
		{
			//�߂荞�܂Ȃ��悤�ɂ���ڒ���Ԃ̈ێ�
			if ( Collision::Check2Box(playerBox, boxs[i], distance) == true)
			{
				normal = Collision::CheckRayBoxforPlane(ray, boxs[i]);
				adhesion = true;
				PushBack(normal, distance);
				//�s��̍X�V�Ȃ�
				Object3d::Update();
			}
			else
			{
				adhesion = false;
			}
		}
		else if (nowMove == true) //�ړ����
		{
			//�������Ă��邩�̊m�F
			if (Collision::Check2Box(playerBox, boxs[i], distance) == true)
			{
				normal = Collision::CheckRayBoxforPlane(ray, box);
				adhesion = true;
				//�߂荞�܂Ȃ��悤�ɉ����߂�����
				PushBack(normal, distance);
				//�s��̍X�V�Ȃ�
				Object3d::Update();
			}
		}
	}

	Object3d::Update();

	//�N���X�^���Ƃ̐ڐG
	std::vector<Box> crystal;
	onObject = false;

	//���@����XZ���ň��̋����̒��ɏ�Q���I�u�W�F�N�g�̒��S���W��������̂��������蔻��p�̃R���e�i�Ɋi�[����
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
	//�ǂ̂ڂ�p�|���ɓ������Ă��邩�̔���
	Plate climbWall;
	
	Box climbWallBox;

	Ray ray;
	ray.start = XMLoadFloat3(&pos);
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

		if (Collision::CheckBoxDot(climbWallBox, pos) == true)
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

	if (climbWallHit == false)
	{
		climbOperation = false;
		return;
	}


	//�ǂɂ߂荞��ł���
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
		climbNormal =  pushvec;
		climbOperation = true;
		count = 0;
	}

}

