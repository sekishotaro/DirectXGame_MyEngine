#pragma once
#include <DirectXMath.h>
#include "Camera.h"
#include "Object3d.h"
#include "BaseCollider.h"
#include "CollisionAttribute.h"

class DebugCamera : public Camera
{
public: // �����o�֐�
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="window_width">��ʕ�</param>
	/// <param name="window_height">��ʍ���</param>
	DebugCamera(int window_width, int window_height);

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~DebugCamera() = default;

	/// <summary>
	/// ���t���[���X�V
	/// </summary>
	virtual void Update();

	XMFLOAT3 SphereCoordinateSystem();

	XMFLOAT3 MoveUpdate();

	void UpdateProcess( XMFLOAT3 &cameraPos);

	void UpdateOnly();

	void ViewpointSwitch(float endRota);

	float leap(float rotaA, float rotaB, float timeRatio);

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

	void CliffFlagUpdate();

	void CorrectionProcess();
	bool CorrectionCheck();

	static float GetRotaY() { return rotaY; }
	static float GetRotaX() { return rotaX; }
public:
	static XMFLOAT3 eye;
	static XMFLOAT3 target;
	static float dx;
	static float dy;
	static float dz;
	static float rotaX;
	static float rotaY;
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
};

