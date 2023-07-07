#pragma once
#include <DirectXMath.h>
#include "Camera.h"
#include "Object3d.h"
#include "BaseCollider.h"
#include "CollisionAttribute.h"

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
	virtual void Update();
	/// <summary>
	/// ���ʍ��W�n�̈ʒu����
	/// </summary>
	/// <returns>�J�����ʒu</returns>
	XMFLOAT3 SphereCoordinateSystem();
	/// <summary>
	/// �J�����ړ�����
	/// </summary>
	/// <returns>�J�����ʒu</returns>
	XMFLOAT3 MoveUpdate();
	/// <summary>
	/// �J��������
	/// </summary>
	void Operation();
	/// <summary>
	/// �J�����n�`�����蔻��
	/// </summary>
	/// <param name="cameraPos">�����Ԃ�</param>
	void TerrainPushBackProcess( XMFLOAT3 &cameraPos);
	/// <summary>
	/// 
	/// </summary>
	void UpdateOnly();
	/// <summary>
	/// ���ڏ���
	/// </summary>
	void NoticeProcess();
	/// <summary>
	/// ���ڊp�x�擾
	/// </summary>
	/// <returns>�v���C���[�̌����Ă���p�x</returns>
	float GetNoticeRot();
	bool PlayerJumpUp();
	/// <summary>
	/// �J�����^�[�Q�b�g���W��
	/// </summary>
	XMFLOAT3 TargetProcess();
	/// <summary>
	/// �R���݂����u�Ԃ�����
	/// </summary>
	/// <returns></returns>
	float CliffMoveTargetState();
	/// <summary>
	/// �R�オ��^�C�~���O�擾
	/// </summary>
	void CliffFlagUpdate();
	/// <summary>
	/// �J�����ړ��f�B���C
	/// </summary>
	void CorrectionProcess();
	/// <summary>
	/// �f�B���C�m�F
	/// </summary>
	/// <returns>�f�B���C�����邩</returns>
	bool CorrectionCheck();
	/// <summary>
	/// �n�ʂɂ��J�����̕␳
	/// </summary>
	void SlopeRotaYProcess();
	/// <summary>
	/// ��̏�̏���
	/// </summary>
	void OnSlopeProcess();
	/// <summary>
	/// ��̏�łȂ�����
	/// </summary>
	void UnSlopeProcess();

	float leap(float rotaA, float rotaB, float timeRatio);

	float AngleNormalize(const float rot);
public:
	static XMFLOAT3 eye;
	static XMFLOAT3 target;
	static float dx;
	static float dy;
	static float dz;
	XMFLOAT2 rota = { 270.0f, 70.0f };
	static float dis;
	float correctionDis = 0.0f;
	XMFLOAT3 correctionVal = {};
	static bool hitFlag;

	static std::unique_ptr<Object3d> Object;
	float oldPosY = 0.0f;
	static Model* Model;
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

