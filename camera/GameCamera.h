#pragma once
#include <DirectXMath.h>
#include "Camera.h"
#include "Object3d.h"
#include "BaseCollider.h"
#include "CollisionAttribute.h"
#include "Player.h"

class GameCamera : public Camera
{
public: // �����o�֐�
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="window_width">��ʕ�</param>
	/// <param name="window_height">��ʍ���</param>
	GameCamera(int window_width, int window_height);

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~GameCamera() = default;

	/// <summary>
	/// ���t���[���X�V
	/// </summary>
	virtual void Update(Player* player);
	/// <summary>
	/// ���ʍ��W�n�̈ʒu����
	/// </summary>
	/// <returns>�J�����ʒu</returns>
	XMFLOAT3 SphereCoordinateSystem(Player* player);
	/// <summary>
	/// �J�����ړ�����
	/// </summary>
	/// <returns>�J�����ʒu</returns>
	XMFLOAT3 MoveUpdate(Player* player);
	/// <summary>
	/// �J��������
	/// </summary>
	void Operation(Player* player);
	/// <summary>
	/// �J�����n�`�����蔻��
	/// </summary>
	/// <param name="cameraPos">�����Ԃ�</param>
	void TerrainPushBackProcess( XMFLOAT3 &cameraPos, Player* player);
	/// <summary>
	/// ���ڏ���
	/// </summary>
	void NoticeProcess(Player* player);
	/// <summary>
	/// ���ڊp�x�擾
	/// </summary>
	/// <returns>�v���C���[�̌����Ă���p�x</returns>
	float GetNoticeRot(Player* player);
	bool PlayerJumpUp(Player* player);
	/// <summary>
	/// �J�����^�[�Q�b�g���W��
	/// </summary>
	XMFLOAT3 TargetProcess(Player* player);
	/// <summary>
	/// �R���݂����u�Ԃ�����
	/// </summary>
	/// <returns></returns>
	float CliffMoveTargetState();
	/// <summary>
	/// �R�オ��^�C�~���O�擾
	/// </summary>
	void CliffFlagUpdate(Player* player);
	/// <summary>
	/// �J�����ړ��f�B���C
	/// </summary>
	void CorrectionProcess(Player* player);
	/// <summary>
	/// �f�B���C�m�F
	/// </summary>
	/// <returns>�f�B���C�����邩</returns>
	bool CorrectionCheck(Player* player);
	/// <summary>
	/// �n�ʂɂ��J�����̕␳
	/// </summary>
	void SlopeRotaYProcess(Player* player);
	/// <summary>
	/// ��̏�̏���
	/// </summary>
	void OnSlopeProcess(Player* player);
	/// <summary>
	/// ��̏�łȂ�����
	/// </summary>
	void UnSlopeProcess();

	float leap(float rotaA, float rotaB, float timeRatio);

	float AngleNormalize(const float rot);
public:

	XMFLOAT3 eye = {};
	XMFLOAT3 target = {};
	float dx = 0;
	float dy = 0;
	float dz = 0;
	XMFLOAT2 rota = { 270.0f, 70.0f };
	float dis = 20.0f;
	float correctionDis = 0.0f;
	XMFLOAT3 correctionVal = {};
	bool hitFlag = false;

	std::unique_ptr<Object3d> Object;
	float oldPosY = 0.0f;
	Model* Model = nullptr;
	BaseCollider* collider = nullptr;

	bool viewpointSwitchFlag = false;
	float viewpointSwitchposParRotX = 0.0f;
	float viewpointSwitchposParRotY = 0.0f;
	float viewpointSwitchposParDis = 0.0f;

	XMFLOAT3 oldTargetPos = {};

	bool cliffTargetFlag = false;

	float cliffTargetCount = 1.0f;
	float movePreviousPosY = 0.0f;
	float moveAftaerPosY = 0.0f;

	bool slopeRotaFlag = true;
};

