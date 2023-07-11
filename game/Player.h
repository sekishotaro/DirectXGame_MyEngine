#pragma once

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include "Input.h"
#include "FbxObject3d.h"
#include "Object3d.h"

class Player : public FbxObject3d
{
private: // �G�C���A�X
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

	enum PlayerAnimeState
	{
		idling,				// 0�F�A�C�h�����O
		walking,			// 1�F�E�H�[�L���O
		running,			// 2�F�����j���O
		jump,				// 3�F�W�����v
		fall,				// 4�F����
		climbing,			// 5�F�ǂ̂ڂ�
		landing,			// 6�F���n
		hangingCliff,		// 7�F�R�Ԃ牺����A�C�h�����O
		kick,				// 8�F�L�b�N
		sliding,			// 9�F�X���C�f�B���O
		pushingWalking,		// 10�F��������
		tiredWalking,		// 11�F�X�^�~�i�؂����
		tiredIdling,		// 12�F�X�^�~�i�؂�A�C�h�����O
		wallKickUp,			// 13�F�ǏR��W�����v
		hangingCliffUp		// 14�F�R�オ��
	};

	enum PlayerStatus
	{
		STATE_IDLING,				//00:�A�C�h�����
		STATE_WALKING,				//01:����
		STATE_RUNNING,				//02:����
		STATE_JUMP_UP,				//03:�W�����v�㏸
		STATE_JUMP_DOWN,			//04:�W�����v���~
		STATE_FALL,					//05:����
		STATE_CLIMBING,				//06:�ǂ̂ڂ�
		STATE_LANDING,				//07:���n
		STATE_CLIFF_IDLING,			//08:�R���݃A�C�h�����
		STATE_SLOPE_SLIDING,		//09:�⊊�藎��
		STATE_PUSHBOX,				//10:���������
		STATE_TIRED_IDLING,			//11:�X�^�~�i�؂�
		STATE_TIRED_WALKING,		//12:�X�^�~�i�؂����
		STATE_WALLKICK_UP,			//13:�ǏR��W�����v�㏸
		STATE_WALLKICK_DOWN,		//14:�ǏR��W�����v���~
		STATE_CLIFFUP,				//15:�R�オ��
	};

	enum StaminaStatus
	{
		sutaminaMax,		// 0�F���^����
		sutaminaUse,		// 1�F���
		sutaminaRecovery,	// 2�F�ʏ��
		sutaminaBurst		// 3�F�X�^�~�i�o�[�X�g,��
	};

	enum HitYStatus
	{
		onGrounds,		// 0�F�n�`(����)�̏�
		onSlope,		// 1�F�n�`(��)�̏�
		onObjects,		// 2�F�I�u�W�F�N�g�̏�
		air				// 3�F��
	};

	enum HitXStatus
	{
		wall,			// 0�F�n�`�̕�
		object,			// 1�F�I�u�W�F�N�g
		ivy,			// 2�F�����(��)���n�`�̕ǂɂ��������Ă��邪�ӂ�D��
		goal			// 3�F�S�[��
	};

public:

	/// <summary>
	/// 3D�I�u�W�F�N�g����
	/// </summary>
	/// <returns>�C���X�^���X����</returns>
	static Player* Create(FbxModel* model = nullptr);

public:

	/// <summary>
	/// ������
	/// </summary>
	/// <returns>����</returns>
	void Initialize() override;

	/// <summary>
	/// �}�C�t���[������
	/// </summary>
	void Update() override;

	/// <summary>
	/// �Փˎ��R�[���o�b�N�֐�
	/// </summary>
	/// <param name="info">�Փˏ��</param>
	void OnCollision(const CollisionInfo& info) override;

	/// <summary>
	/// �I�u�W�F�N�g�̃A�b�v�f�[�g
	/// </summary>
	void ObjectUpdate();

	void Finalize();

	void ReStart();

private:
	void PushBack(const DirectX::XMVECTOR& normal, const XMFLOAT3& distance);

	void ClimbWallJudge(const DirectX::XMVECTOR move);

	/// <summary>
	/// �ړ�����
	/// </summary>
	/// <param name="move"></param>
	void MoveOperation(DirectX::XMVECTOR& move, float &power);
	
	/// <summary>
	/// �R�オ��
	/// </summary>
	void MoveClimbingCliff(DirectX::XMVECTOR& move, float& power);

	/// <summary>
	/// �ʏ�ړ�����
	/// </summary>
	/// <param name="move"></param>
	void MoveNormal(DirectX::XMVECTOR& move, float &power);

	/// <summary>
	/// �ǂ̂ڂ�ړ�����
	/// </summary>
	/// <param name="move"></param>
	void MoveClimb(DirectX::XMVECTOR& move, float &power);

	/// <summary>
	/// �N���X�^������
	/// </summary>
	void CrystalConfirmationProcess();

	/// <summary>
	/// �S�[���ڐG�m�F����
	/// </summary>
	void GoalConfirmationProcess();

	/// <summary>
	/// ��Q���ڐG����
	/// </summary>
	void ObstacleConfirmationProcess(const DirectX::XMVECTOR& move);

	/// <summary>
	/// �d�͏���
	/// </summary>
	void GravityConfirmationProcess();

	/// <summary>
	/// �n�`�ڐG����(���b�V���R���C�_�[)
	/// </summary>
	void TerrainConfirmationProcess();

	//�X�^�~�i�Ǘ�
	void StaminaManagement();

	//���ړ�����
	void MoveBoxProcess(DirectX::XMVECTOR& move, float &power);

	//���ԊǗ�
	void TimeManagement();

	//�A�j���[�V����
	void AnimetionProcess();

	//�≺�蓖���蔻��
	void SlopeDownhill(DirectX::XMVECTOR& move, float& power);

	//�ǃL�b�N�W�����v
	bool climbingKickJump();

	//�R�オ��
	void climbingCliff();

	//�X�^�~�i�g�p�s�m�F
	bool StaminaUnusable();

	//�ړ��l���Z����
	void MoveAddDetermination(DirectX::XMVECTOR& move, float& power);

	//�R�オ�莞�̓����Ɉړ�����
	void BoxInMove();
	//�₩�痧���オ��m�F
	bool SlopeRisingFlag();

	bool TimeCheck(float& time);

	void StatusProsecc();

private:
	bool StaminaConsumptionFlag();
	bool MoveBoxConditionFlag();
	bool MoveStickCheck();
	bool FallStateCheck();
public:
	//�f�o�b�N�p�Q�b�^�[
	static XMFLOAT3 GetMove() { return moveVal; };
	int GetCrystal() { return crystalNum; }
	int GetCrystalGetNum() { return crystalGetNum; }
	bool GetGoalFlag() { return goalFlag; }
	int GetCrystalMaxNum() { return crystalMaxNum; }
	int GetCrystalNum() { return crystalNum; }
	bool GetStaminaCutFlag() { return staminaCut; }
	float GetStaminaQuantity() { return staminaQuantity; }
	static float GetTimeLimit() { return timeLimit; }
	bool GetStaminaBoostFlag() { return staminaBoostFlag; }
	static bool GetCrystalGetFlag() { return crystalGetFlag; }
	static bool GetMoveBoxHitFlag() { return moveBoxHitFlag;}
	static int GetAnimeNum() { return animeNum; }
	static bool GetSlopeFlag() { return slopeFlag; }
	static float GetTimeMax() { return timeLimitMax; }

	PlayerStatus& GetStatus() { return playerStatus; }
	PlayerStatus& GetOldStatus() { return oldPlayerStatus; }

	void SetModel1(FbxModel* fbxModel) { this->fbxModels[0] = fbxModel; };
	void SetModel2(FbxModel* fbxModel) { this->fbxModels[1] = fbxModel; };
	void SetModel3(FbxModel* fbxModel) { this->fbxModels[2] = fbxModel; };
	void SetModel4(FbxModel* fbxModel) { this->fbxModels[3] = fbxModel; };
	void SetModel5(FbxModel* fbxModel) { this->fbxModels[4] = fbxModel; };
	void SetModel6(FbxModel* fbxModel) { this->fbxModels[5] = fbxModel; };
	void SetModel7(FbxModel* fbxModel) { this->fbxModels[6] = fbxModel; };
	void SetModel8(FbxModel* fbxModel) { this->fbxModels[7] = fbxModel; };
	void SetModel9(FbxModel* fbxModel) { this->fbxModels[8] = fbxModel; };
	void SetModel10(FbxModel* fbxModel) { this->fbxModels[9] = fbxModel; };
	void SetModel11(FbxModel* fbxModel) { this->fbxModels[10] = fbxModel; };
	void SetModel12(FbxModel* fbxModel) { this->fbxModels[11] = fbxModel; };
	void SetModel13(FbxModel* fbxModel) { this->fbxModels[12] = fbxModel; };
	void SetModel14(FbxModel* fbxModel) { this->fbxModels[13] = fbxModel; };
	void SetModel15(FbxModel* fbxModel) { this->fbxModels[14] = fbxModel; };
private:
	
	XMFLOAT3 parPos;					//�O�t���[���̈ʒu
	static XMFLOAT3 moveV;
	static XMFLOAT3 moveVal;			//�v���C���[�̈ړ���
	DirectX::XMVECTOR fallV = {};		//�����x�N�g��
	static float moveAdjustmentNum;		//�ړ��ʒ�����
	int inputX = 0;					//�R���g���[���[�X�e�B�b�N����X
	int inputY = 0;					//�R���g���[���[�X�e�B�b�N����Y
	int wallCount = 0;					//�c�^�ɓ����������̂߂荞�ݎ��J�E���g
	DirectX::XMVECTOR climbNormal = {};	//�ǂ̂ڂ�p�ۑ��߂荞�ݖ@��
	static float timeLimit;				//���݂̎���
	static const float timeLimitMax;	//��������
	float forciblyRecoveryTime = 10.0f;	//�X�^�~�i���Ȃ��Ȃ������̉񕜎���
	float staminaRecoveryTime = 5.0f;	//�X�^�~�i�񕜑ҋ@����
	float jumpHeightPosY = 0.0f;		//�W�����v�������ɕǂƐڐG���Ȃ�����
	/// <summary>
	/// �X�e�[�^�X�Ǘ�
	/// </summary>
	PlayerAnimeState PlayerState = idling;			//�A�j���[�V����
	PlayerAnimeState oldPlayerState = idling;		//�O�t���[���̃A�j���[�V����
	PlayerStatus playerStatus = STATE_IDLING;		//�v���C���[
	PlayerStatus oldPlayerStatus = STATE_IDLING;	//�O�t���[���̃v���C���[
	StaminaStatus staminaStatus = sutaminaMax;		//�X�^�~�i

	//�A�j���[�V����
	static int animeNum;
	static int oldAnimeNum;
	static bool animeFlag;
	//���f��
	static FbxModel* fbxModels[];

	//��m�F�t���O
	static bool slopeFlag;
	bool oldSlopeFlag = false;
	bool slopeRising = false;
	//�̂ڂ�p�|���Ƃ̓����蔻��
	static bool climbWallHit;
	//�O�t���[���̓����蔻��̌���
	bool climbWallHitPar = false;
	//�X�^�~�i���g���邩�ǂ���(�X�^�~�i���g���؂����ꍇ�ɂȂ���)
	static bool crystalGetFlag;
	int crystalGetNum = 0;
	//�ǂƐڐG�m�F�t���O
	bool wallHittingFlag = false;
	bool oldWallHittingFlag = false;
	bool jumpWallHittingFlag = false;
	//�R�オ��
	int climbingCliffBoxNum = 0;
	//���ړ����Ă��m�F�t���O
	static bool moveBoxFlag;
	static bool moveBoxHitFlag;
	int moveBoxHitNum = 0;
	//�ڒn�t���O
	bool onGround = false;
	//�I�u�W�F�N�ڒn�t���O
	bool onObject = false;
	//���@�ƃS�[���̓����蔻��
	bool goalFlag = true;
	//�N���X�^���̐�
	int crystalNum = 0;
	int crystalMaxNum = 0;
	bool staminaCut = false;
	float staminaQuantity = 100.0f;		// �X�^�~�i�c��
	// �X�^�~�i����m�F�t���O
	bool staminaBoostFlag = false;
public:
	XMFLOAT3 moveBoxMax1 = { 0.0f, 0.0f, 0.0f };
	//debug
	bool teleportFlag = false;
	bool testFlag = false;
	bool timeLimitcancel = false;
	bool moveLimitFlag = false;
};

