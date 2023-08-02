#include "GameCamera.h"
#include "Input.h"
#include "Player.h"
#include "SphereCollider.h"
#include "QueryCallback.h"
#include "CollisionManager.h"
#include "MyMath.h"

using namespace DirectX;

GameCamera::GameCamera(int window_width, int window_height) : Camera(window_width, window_height)
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
	rota.x = 180.0f;
}

void GameCamera::Update(Player* player)
{
	//�J�����̈ʒu�ړ�����(���ʏ�̊p�x�ړ�)
	XMFLOAT3 cameraPos = MoveUpdate(player);

	TerrainPushBackProcess(cameraPos, player);
	//�R���C�_�[�X�V
	Object->UpdateWorldMatrix();
	collider->Update();

	CorrectionProcess(player);
	cameraPos = cameraPos + correctionVal;
	SetEye(cameraPos);
	
	//�펞���@�Ƀ^�[�Q�b�g
	XMFLOAT3 targetPos = TargetProcess(player);
	//�J�����ʒu������
	targetPos.y += 6.0f;
	
	Camera::SetTarget(targetPos);
	Camera::Update();
}

GameCamera::XMFLOAT3 GameCamera::SphereCoordinateSystem(Player* player)
{
	XMFLOAT2 radius = { rota.x * 3.14f / 180.0f, rota.y * 3.14f / 180.0f };
	XMFLOAT3 cameraPos = {};

	cameraPos = TargetProcess(player);
	
	//���ʍ��W�n
	cameraPos.y += (dis) * cos(radius.y);
	cameraPos.x += (dis) * sin(radius.y) * cos(radius.x);
	cameraPos.z += (dis) * sin(radius.y) * sin(radius.x);

	return cameraPos;
}

GameCamera::XMFLOAT3 GameCamera::MoveUpdate(Player* player)
{
	XMFLOAT3 cameraPos = {};
	//����
	Operation(player);
	//��
	SlopeRotaYProcess(player);

	//���_���ʈړ�
	NoticeProcess(player);

	cameraPos = SphereCoordinateSystem(player);

	oldPosY = cameraPos.y;
	return cameraPos;
}

void GameCamera::Operation(Player* player)
{
	if (PlayerJumpUp(player) == true) return;


	//�L�[�{�[�h
	if (Input::GetInstance()->PushKey(DIK_UP)) { rota.y -= 1.0f; }
	else if (Input::GetInstance()->PushKey(DIK_DOWN)) { rota.y += 1.0f; }
	if (Input::GetInstance()->PushKey(DIK_RIGHT)) { rota.x += 1.0f; }
	else if (Input::GetInstance()->PushKey(DIK_LEFT)) { rota.x -= 1.0f; }
	//�R���g���[���[
	if (Input::GetInstance()->RightStickIn(UP) && rota.y < limitAngleY[1])
	{
		rota.y += 1.0f;
		slopeRotaFlag = false;
		if (dis <= disMax && hitFlag == false) { dis += disAddPower; }
	}
	else if (Input::GetInstance()->RightStickIn(DOWN) && rota.y > limitAngleY[0])
	{
		rota.y -= 1.0f;
		slopeRotaFlag = false;
		if (dis <= disMax && hitFlag == false) { dis += disAddPower; }
	}
	
	if (Input::GetInstance()->RightStickIn(RIGHT))
	{
		rota.x -= 1.0f;
		if (dis <= disMax && hitFlag == false) { dis += disAddPower; }
	}
	else if (Input::GetInstance()->RightStickIn(LEFT))
	{
		rota.x += 1.0f;
		if (dis <= disMax && hitFlag == false) { dis += disAddPower; }
	}
	else if (player->GetStatus() == Player::STATE_RUNNING && rotaAuto == true)
	{
		RotaXAutoProcess(player);
	}

	if (dis <= disMax && hitFlag == false) { dis += 0.5f; }
}

void GameCamera::TerrainPushBackProcess( XMFLOAT3& cameraPos, Player* player)
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
		if (dis > 5.0f) { dis -= disAddPower; }
		cameraPos = SphereCoordinateSystem(player);
	} 
	else if (callback.move.m128_f32[1] != 0)
	{
		hitFlag = true;
		if (dis > 5.0f) { dis -= disAddPower; }
		cameraPos = SphereCoordinateSystem(player);
	}
	else if (callback.move.m128_f32[2] != 0)
	{
		hitFlag = true;
		if (dis > 5.0f) { dis -= disAddPower; }
		cameraPos = SphereCoordinateSystem(player);
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
		rota.y -= 1.0f;
		slopeRotaFlag = false;
		cameraPos = SphereCoordinateSystem(player);
	}
}

void GameCamera::NoticeProcess(Player* player)
{
	//���_���ʈړ�
	static float endRota = 0;
	if (Input::GetInstance()->PushPadbutton(GAMEPAD_RIGHT_SHOULDER) && viewpointSwitchFlag == false)
	{
		endRota = GetNoticeRot(player);
	}

	//�R�オ�莞���_���ʈړ�
	if (static_cast<int>(player->GetStatus()) == Player::STATE_CLIFF_IDLING && static_cast<int>(player->GetOldStatus()) != Player::STATE_CLIFFUP)
	{
		endRota = GetNoticeRot(player);
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

	if (dis <= disMax && hitFlag == false) { dis += disAddPower; }
	rota.x = leap(viewpointSwitchposParRotX, endRota, timeRatio);
	rota.y = leap(viewpointSwitchposParRotY, 60.0f, timeRatio);
}

float GameCamera::GetNoticeRot(Player* player)
{
	float endRota = 0.0f;
	endRota -= player->GetRotation().y + difference;
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

bool GameCamera::XAutoCheck(Player* player)
{
	static int oldInputX = 0;
	static int oldInputY = 0;
	bool result = false;
	if (player->GetInputX() != oldInputX) result = true;
	if (player->GetInputY() != oldInputY) result = true;
	if (player->GetStatus() != Player::STATE_RUNNING) result = true;

	oldInputX = player->GetInputX();
	oldInputY = player->GetInputY();
	return result;
}

void GameCamera::RotaXAutoProcess(Player* player)
{
	static float MoveTime = 0.0f;

	//�i�ޕ������؂�ւ������
	if (XAutoCheck(player) == true)
	{
		MoveTime = 0.0f;
		viewpointSwitchposParRotX = rota.x;
	}


	float endRota = 0.0f;
	endRota -= player->GetRotation().y + difference;
	endRota = AngleNormalize(endRota);

	

	endRota = AngleNormalize(endRota);

	const float MoveMaxTime = 1.0f; //�ړ��ɂ����鎞��
	float timeRatio = MoveTime / MoveMaxTime;
	if (MoveTime <= MoveMaxTime)
	{
		//�G�t�F�N�g�̎��Ԃ�i�߂�
		MoveTime += 1.0f / 60.0f;
	}
	

	rota.x = leap(viewpointSwitchposParRotX, endRota, timeRatio);
	//rota.x = endRota;
}

bool GameCamera::PlayerJumpUp(Player* player)
{
	if (player->GetStatus() == Player::jump && player->GetStatus() == Player::fall)
	{
		return true;
	}
	else if (player->GetAnimeNum() == Player::wallKickUp)
	{
		return true;
	}
	else if (player->GetAnimeNum() == Player::hangingCliffUp)
	{
		return true;
	}
	
	return false;
}

GameCamera::XMFLOAT3 GameCamera::TargetProcess(Player* player)
{
	XMFLOAT3 result = {};
	//���ʈړ�
	result = { player->GetPosition().x, 0.0f, player->GetPosition().z};

	CliffFlagUpdate(player);

	//Y���W�ړ�
	if (static_cast<int>(player->GetStatus()) == Player::STATE_JUMP_UP)			//�ʏ�W�����v�㏸������
	{
		//�W�����v�O���W
		result.y = oldTargetPos.y;
	}
	else if (static_cast<int>(player->GetStatus()) == Player::STATE_JUMP_DOWN)	//�ʏ�W�����v���~������
	{
		//�W�����v�O���W
		result.y = oldTargetPos.y;
	}
	else if (static_cast<int>(player->GetStatus()) == Player::STATE_WALLKICK_UP)	// �ǏR��W�����v�㏸�����Z
	{
		//�R�オ��O���W
		result.y = oldTargetPos.y;
	}
	else if (static_cast<int>(player->GetStatus()) == Player::STATE_WALLKICK_DOWN)	// �ǏR��W�����v���~�����Z
	{
		//�R�オ��O���W
		result.y = oldTargetPos.y;
	}
	else if (static_cast<int>(player->GetStatus()) == Player::STATE_CLIFF_IDLING)	// �R���ݒ�
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
		result.y = player->GetPosition().y;
	}

	oldTargetPos = result;
	return result;
}

void GameCamera::CliffFlagUpdate(Player* player)
{
	//�R���t���O�������Ă����瑁�����^�[��
	if (cliffTargetFlag == true) return;

	//�R�オ��������u��
	if (static_cast<int>(player->GetStatus()) == Player::STATE_CLIFFUP &&
		static_cast<int>(player->GetOldStatus()) == Player::STATE_CLIFF_IDLING)
	{
		cliffTargetFlag = true;
		moveAftaerPosY = player->GetPosition().y;
		movePreviousPosY = oldTargetPos.y;
	}
}

void GameCamera::CorrectionProcess(Player* player)
{
	static XMFLOAT3 moveVal = {};

	if (CorrectionCheck(player) == true) //�ړ�
	{
		if (moveVal.x < 1.0f && moveVal.x > -1.0f)
		{
			moveVal.x += (-1.0f) * player->GetMove().x;
		}

		if (moveVal.z < 1.0f && moveVal.z > -1.0f)
		{
			moveVal.z += (-1.0f) * player->GetMove().z;
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

bool GameCamera::CorrectionCheck(Player* player)
{
	if (static_cast<int>(player->GetStatus()) == Player::STATE_IDLING) return true;
	if (static_cast<int>(player->GetStatus()) == Player::STATE_WALKING) return true;
	if (static_cast<int>(player->GetStatus()) == Player::STATE_RUNNING) return true;
	if (static_cast<int>(player->GetStatus()) == Player::STATE_JUMP_UP) return true;
	return false;
}

void GameCamera::SlopeRotaYProcess(Player* player)
{
	if (player->GetSlopeFlag() == true)	//��̏�ɂ���
	{
		OnSlopeProcess(player);
	}
	else
	{
		UnSlopeProcess();
	}
}

void GameCamera::OnSlopeProcess(Player* player)
{
	if (slopeRotaFlag == false)
	{
		slopeRotaFlag = true;
	}
	//		��̏㉺�p�x		�E		��
	const XMFLOAT2 slopeUp	= {  75.0f, 105.0f };
	const XMFLOAT2 slopeDown	= { 255.0f, 285.0f };
	//��㉺Y�p�x			��		��
	XMFLOAT2 slopeRotY = { 100.0f, 50.0f };
	float slope_lower[2] = { 180.0f, 360.0f };


	if (player->GetRotation().y >= slopeUp.x && player->GetRotation().y <= slopeUp.y)
	{
		if (rota.y < slopeRotY.x)
		{
			rota.y += 0.5f;
		}
	}
	else if (player->GetRotation().y >= slopeDown.x && player->GetRotation().y <= slopeDown.y)
	{
		if (rota.y > slopeRotY.y)
		{
			rota.y -= 0.5f;
		}

		if (rota.x < 1.0f || rota.x > 359.0f)
		{
			rota.x = 0;
		}
		else if (rota.x <= slope_lower[0] && rota.x > 0)
		{
			rota.x -= 1.0f;
		}
		else if (rota.x > slope_lower[0] && rota.x < slope_lower[1])
		{
			rota.x += 1.0f;
		}
	}
}

void GameCamera::UnSlopeProcess()
{
	if (slopeRotaFlag == false) return;
	
	//��{�p�x
	float flatRotaY = 70.0f;

	if (rota.y > flatRotaY)
	{
		rota.y -= 0.5f;
	}
	
	if (rota.y < flatRotaY)
	{
		rota.y += 0.5f;
	}
}

float GameCamera::CliffMoveTargetState()
{
	float flame = 60.0f;
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

	cliffTargetCount -= 1.0f / flame;

	return pos.y;
}
