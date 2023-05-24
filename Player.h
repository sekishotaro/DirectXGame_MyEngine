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

	enum StaminaStatus
	{
		sutaminaMax,		//���^����
		sutaminaUse,		//���
		sutaminaRecovery,	//�ʏ��
		sutaminaBurst		//�X�^�~�i�o�[�X�g,��
	};

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

	/// <summary>
	/// 5/17�쐬������
	/// </summary>
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
	bool moveBoxConditionFlag();
public:
	//�f�o�b�N�p�Q�b�^�[
	static XMFLOAT3 GetPos() { return pos; }

	static XMFLOAT3 GetRot() { return rot; }

	static XMFLOAT3 GetMove() { return moveV; };

	static bool &GetNowMove() { return nowMove;}
	
	static bool &GetadhesionMesh() { return adhesionMesh;}

	static int GetCrystal() { return crystalNum; }

	static bool &GetGoalFlag() { return goalFlag; }

	static bool &GetClimbWallHitFlag() { return climbOperation; }

	static float GetTimeLimit() { return timeLimit; }

	static bool& GetStaminaFlag() { return staminaBoostFlag; }

	static float& GetStaminaQuantity() { return staminaQuantity; }

	static bool& GetStaminaCut() { return staminaCut; }

	static bool& GetAnimeFlag() { return animeFlag; }

	static int& GetInputNumX() { return inputX; }
	static int& GetInputNumY() { return inputY; }

	static float& GEtTestFloatNum() { return testRota; }

	static bool GetCrystalGetFlag() { return crystalGetFlag; }
	static bool GetMoveBoxHitFlag() { return moveBoxHitFlag;}
	static bool GetJumpFlag() { return jumpFlag; }
	static bool GetFallFlag() { return fallFlag; }
	static int& GetAnimeNum() { return animeNum; }

	static bool &GetSlopeFlag() { return slopeFlag; }
	bool& GetOldSlopeFlag() { return oldSlopeFlag; }
	static bool& GetWallHitFlag() { return wallHittingFlag; }
	static bool& GetJumpWallHitFlag() { return jumpWallHittingFlag; }
	static bool& GetClimbingCliffFlag() { return climbingCliffFlag; }
	static bool& GetLandingFlag() { return landingFlag; }
	static float GetTimeMax() { return timeLimitMax; }

	static PlayerStatus& GetStatus() { return playerStatus; }
	static PlayerStatus& GetOldStatus() { return oldPlayerStatus; }

	void SetModel1(FbxModel* fbxModel) { this->fbxModel1 = fbxModel; };
	void SetModel2(FbxModel* fbxModel) { this->fbxModel2 = fbxModel; };
	void SetModel3(FbxModel* fbxModel) { this->fbxModel3 = fbxModel; };
	void SetModel4(FbxModel* fbxModel) { this->fbxModel4 = fbxModel; };
	void SetModel5(FbxModel* fbxModel) { this->fbxModel5 = fbxModel; };
	void SetModel6(FbxModel* fbxModel) { this->fbxModel6 = fbxModel; };
	void SetModel7(FbxModel* fbxModel) { this->fbxModel7 = fbxModel; };
	void SetModel8(FbxModel* fbxModel) { this->fbxModel8 = fbxModel; };
	void SetModel9(FbxModel* fbxModel) { this->fbxModel9 = fbxModel; };
	void SetModel10(FbxModel* fbxModel) { this->fbxModel10 = fbxModel; };
	void SetModel11(FbxModel* fbxModel) { this->fbxModel11 = fbxModel; };
	void SetModel12(FbxModel* fbxModel) { this->fbxModel12 = fbxModel; };
	void SetModel13(FbxModel* fbxModel) { this->fbxModel13 = fbxModel; };
	void SetModel14(FbxModel* fbxModel) { this->fbxModel14 = fbxModel; };
	void SetModel15(FbxModel* fbxModel) { this->fbxModel15 = fbxModel; };

private:
	
	//�S�t���[���̈ʒu
	XMFLOAT3 parPos;
	//�ڒn�t���O
	static bool onGround;
	//�ڒ��t���O
	static bool adhesionMesh;

	//��Q���ڒ��t���O
	bool adhesion = false;
	//�I�u�W�F�N�ڒn�t���O
	bool onObject = false;
	//�ړ����m�F�t���O
	static bool nowMove;
	//��m�F�t���O
	static bool slopeFlag;
	bool oldSlopeFlag = false;
	bool slopeRising = false;
	//�W�����v�m�F�t���O
	static bool jumpFlag;
	static bool wallKickUpFlag;
	static bool fallFlag;
	static bool landingFlag;

	//���@�ƃS�[���̓����蔻��
	static bool goalFlag;

	//�����x�N�g��
	DirectX::XMVECTOR fallV = {};
	//�v���C���[�̈ʒu

	static XMFLOAT3 pos;
	static XMFLOAT3 rot;
	static XMFLOAT3 moveV;

	//�ړ��ʒ�����
	static float moveAdjustmentNum;


	static int inputX;
	static int inputY;

	//�m�F�p
	static float testRota;

	//�N���X�^���̐�
	static int crystalNum;

	//�c�^�ɓ����������̂߂荞�ݎ��J�E���g
	int wallCount = 0;

	//�ǂ̂ڂ���
	static bool climbOperation;
	static bool oldClimbOperation;
	//�ǂ̂ڂ�p�ۑ��߂荞�ݖ@��
	DirectX::XMVECTOR climbNormal = {};
	//�̂ڂ�p�|���Ƃ̓����蔻��
	static bool climbWallHit;
	//�O�t���[���̓����蔻��̌���
	bool climbWallHitPar = false;
	
	//��������
	static float timeLimit;
	static const float timeLimitMax;
	float flame = 0.0f;


	/// <summary>
	/// �X�^�~�i����m�F�t���O
	/// </summary>
	static bool staminaBoostFlag;
	/// <summary>
	/// �X�^�~�i�c��
	/// </summary>
	static float staminaQuantity;
	//�X�^�~�i���Ȃ��Ȃ������̉񕜎���
	float forciblyRecoveryTime = 10.0f;
	//�X�^�~�i�񕜑ҋ@����
	float staminaRecoveryTime = 5.0f;
	//�X�^�~�i���g���邩�ǂ���(�X�^�~�i���g���؂����ꍇ�ɂȂ���)
	static bool staminaCut;
	static bool crystalGetFlag;
	//���ړ����Ă��m�F�t���O
	static bool moveBoxFlag;
	static bool moveBoxHitFlag;
	int moveBoxHitNum = 0;
	//�ǂƐڐG�m�F�t���O
	static bool wallHittingFlag;
	static bool oldWallHittingFlag;
	static bool jumpWallHittingFlag;
	//�W�����v�������ɕǂƐڐG���Ȃ�����
	float jumpHeightPosY = 0.0f;
	//�R�オ��
	static bool climbingCliffFlag;
	static bool climbingCliffUpFlag;
	int climbingCliffBoxNum = 0;
	//�A�j���[�V����
	static int animeNum;
	static int oldAnimeNum;
	static bool animeFlag;

	//���f��
	static FbxModel* fbxModel1;	//��{
	static FbxModel* fbxModel2;	//�E�H�[�L���O
	static FbxModel* fbxModel3;	//�����j���O
	static FbxModel* fbxModel4; //�W�����v
	static FbxModel* fbxModel5;	//����W�����v
	static FbxModel* fbxModel6;	//�N���C�~���O
	static FbxModel* fbxModel7; //���n
	static FbxModel* fbxModel8; //�R�Ԃ牺����A�C�h�����O
	static FbxModel* fbxModel9; //�L�b�N
	static FbxModel* fbxModel10; //�X���C�f�B���O
	static FbxModel* fbxModel11; //��������
	static FbxModel* fbxModel12; //�X�^�~�i�؂����
	static FbxModel* fbxModel13; //�X�^�~�i�؂�A�C�h�����O
	static FbxModel* fbxModel14; //�ǏR��W�����v
	static FbxModel* fbxModel15; //�R�オ��

	PlayerAnimeState PlayerState = idling;

	static PlayerStatus playerStatus;
	static PlayerStatus oldPlayerStatus;

public:
	XMFLOAT3 moveBoxMax1 = { 0.0f, 0.0f, 0.0f };
	bool movingFlag = false;
	//debug
	bool teleportFlag = false;
	bool testFlag = false;
	bool timeLimitcancel = false;
};

