#include "DebugCamera.h"
#include "Input.h"
#include "Player.h"
#include "Enemy.h"
#include "SphereCollider.h"
#include "QueryCallback.h"
#include "CollisionManager.h"
#include "MyMath.h"

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

	//�R���C�_�[�̒ǉ�
	float radius = 0.6f;
	//���a�������������畂�������W�������S�ɂ���
	Object->SetCollider(new SphereCollider(XMVECTOR({ 0, radius, 0,0 }), radius));
	collider = Object->GetBaseCollider();
	collider->SetAttribute(COLLISION_ATTR_ALLIES);
	Object->SetBaseCollider(collider);
	rotaX = 180.0f;
}

void DebugCamera::Update()
{
	//�J�����̈ʒu�ړ�����(���ʏ�̊p�x�ړ�)
	XMFLOAT3 cameraPos = MoveUpdate();

	UpdateProcess(cameraPos);
	RaidCameraCount();
	//�R���C�_�[�X�V
	Object->UpdateWorldMatrix();
	collider->Update();

	SetEye(cameraPos);
	
	//�펞���@�Ƀ^�[�Q�b�g
	XMFLOAT3 targetPos = TargetProcess();
	
	//�P���^�C�~���O�Ŏ������O��
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

	cameraPos = TargetProcess();
	
	//���ʍ��W�n
	cameraPos.y += dis * cos(radiusY);
	cameraPos.x += dis * sin(radiusY) * cos(radiusX);
	cameraPos.z += dis * sin(radiusY) * sin(radiusX);

	return cameraPos;
}

DebugCamera::XMFLOAT3 DebugCamera::MoveUpdate()
{
	XMFLOAT3 cameraPos = {};
	//���a
	float disMax = 20.0f;

	if (PlayerJumpUp() != true)		//�ړ�����
	{
		//�L�[�{�[�h
		if (Input::GetInstance()->PushKey(DIK_UP)) { rotaY -= 1.0f; }
		else if (Input::GetInstance()->PushKey(DIK_DOWN)) { rotaY += 1.0f; }
		if (Input::GetInstance()->PushKey(DIK_RIGHT)) { rotaX += 1.0f; }
		else if (Input::GetInstance()->PushKey(DIK_LEFT)) { rotaX -= 1.0f; }
		//�R���g���[���[
		if (Input::GetInstance()->RightStickIn(UP) && rotaY < 175)
		{
			rotaY += 1.0f;
			if (dis <= disMax && hitFlag == false) { dis += 0.5f; }
		}
		else if (Input::GetInstance()->RightStickIn(DOWN) && rotaY > 5)
		{
			rotaY -= 1.0f;
			if (dis <= disMax && hitFlag == false) { dis += 0.5f; }
		}
		if (Input::GetInstance()->RightStickIn(RIGHT))
		{
			rotaX -= 1.0f;
			if (dis <= disMax && hitFlag == false) { dis += 0.5f; }
		}
		else if (Input::GetInstance()->RightStickIn(LEFT))
		{
			rotaX += 1.0f;
			if (dis <= disMax && hitFlag == false) { dis += 0.5f; }
		}

		if (dis <= disMax && hitFlag == false) { dis += 0.5f; }
	}
	
	//���_���ʈړ�
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

	//�R�オ�莞���_���ʈړ�
	if (static_cast<int>(Player::GetStatus()) == 8 && static_cast<int>(Player::GetOldStatus()) != 15)
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
	//���R���C�_�[���擾
	Object->SetPosition(cameraPos);
	Object->Update();

	SphereCollider* sphereCollider = dynamic_cast<SphereCollider*>(collider);
	assert(sphereCollider);

	//�N�G���\�R�[���o�b�N�N���X
	class CameraQueryCallback :public QueryCallback
	{
	public:
		CameraQueryCallback(Sphere* sphere) : sphere(sphere) {};

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
	CameraQueryCallback callback(sphereCollider);

	//���ƒn�`�̌�����S����
	CollisionManager::GetInstance()->QuerySphere(*sphereCollider, &callback, COLLISION_ATTR_LANDSHAPE);
	//�����ɂ��r�˕�������
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

	//���̏�[���狅�̉��[�܂ł̃��C�L���X�g�p���C������
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

	const float MoveMaxTime = 0.3f; //�ړ��ɂ����鎞��
	float timeRatio = MoveTime / MoveMaxTime;
	if (MoveTime <= MoveMaxTime)
	{
		//�G�t�F�N�g�̎��Ԃ�i�߂�
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

DebugCamera::XMFLOAT3 DebugCamera::TargetProcess()
{
	XMFLOAT3 result = {};
	//���ʈړ�
	result = { Player::GetPos().x, 0.0f,Player::GetPos().z };

	CliffFlagUpdate();

	//Y���W�ړ�
	if (static_cast<int>(Player::GetStatus()) == 3)			//�ʏ�W�����v�㏸������
	{
		//�W�����v�O���W
		result.y = oldTargetPos.y;
	}
	else if (static_cast<int>(Player::GetStatus()) == 4)	//�ʏ�W�����v���~������
	{
		//�W�����v�O���W
		result.y = oldTargetPos.y;
	}
	else if (static_cast<int>(Player::GetStatus()) == 13)	// �ǏR��W�����v�㏸�����Z
	{
		//�R�オ��O���W
		result.y = oldTargetPos.y;
	}
	else if (static_cast<int>(Player::GetStatus()) == 14)	// �ǏR��W�����v���~�����Z
	{
		//�R�オ��O���W
		result.y = oldTargetPos.y;
	}
	else if (static_cast<int>(Player::GetStatus()) == 8)	// �R���ݒ�
	{
		//�R�オ��O���W
		result.y = oldTargetPos.y;
	}
	else if (cliffTargetFlag == true)	// �R�オ��^�C�~���O����̃J�����ړ�
	{
		result.y = CliffMoveTargetState();
	}
	else //���̑��ړ��͎��@�̈ʒu�Ƀ}�[�N
	{
		result.y = Player::GetPos().y;
	}

	oldTargetPos = result;
	return result;
}

void DebugCamera::CliffFlagUpdate()
{
	//�R���t���O�������Ă����瑁�����^�[��
	if (cliffTargetFlag == true) return;

	//�R�オ��������u��
	if (static_cast<int>(Player::GetStatus()) == 15 && static_cast<int>(Player::GetOldStatus()) != 15)
	{
		cliffTargetFlag = true;
		moveAftaerPosY = Player::GetPos().y;
		movePreviousPosY = oldTargetPos.y;
	}
}

float DebugCamera::CliffMoveTargetState()
{
	const float timeMax = 1.0f;							//�ő厞��
	float time = timeMax - cliffTargetCount;			//���Z���Ԃɕω�
	float timeRate = min(time / timeMax, 1.0f);			//�^�C�����[�g 0.0f->1.0f

	//�ړ��������Ԃ̏�����
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
