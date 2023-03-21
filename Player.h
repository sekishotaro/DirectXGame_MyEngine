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

private:
	void PushBack(const DirectX::XMVECTOR& normal, const XMFLOAT3& distance);

	void ClimbWallJudge(const DirectX::XMVECTOR move);

	/// <summary>
	/// �ړ�����
	/// </summary>
	/// <param name="move"></param>
	void MoveOperation(DirectX::XMVECTOR& move, float &power);

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

	bool StaminaUnusable();

public:
	//�f�o�b�N�p�Q�b�^�[
	static XMFLOAT3 GetPos() { return pos; }

	static XMFLOAT3 GetRot() { return rot; }

	static XMFLOAT3 GetMove() { return moveV; };

	static bool &GetNowMove() { return nowMove;}
	
	static bool &GetadhesionMesh() { return adhesionMesh;}

	static int GetCrystal() { return crystalNum; }

	static bool &GetGoalFlag() { return goalFlag; }

	static bool &GetWallHitFlag() { return climbOperation; }

	static float GetTimeLimit() { return timeLimit; }

	static bool& GetStaminaFlag() { return staminaBoostFlag; }

	static float& GetStaminaQuantity() { return staminaQuantity; }

	static bool& GetStaminaCut() { return staminaCut; }

	static bool& GetAnimeFlag() { return animeFlag; }

	static int& GetInputNumX() { return inputX; }
	static int& GetInputNumY() { return inputY; }

	static float& GEtTestFloatNum() { return testRota; }

	static bool GetCrystalGetFlag() { return crystalGetFlag; }

	static bool GetJumpFlag() { return jumpFlag; }

	static int& GetAnimeNum() { return animeNum; }

	static bool &GetSlopeFlag() { return slopeFlag; }

	static float GetTimeMax() { return timeLimitMax; }

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

	//�W�����v�m�F�t���O
	static bool jumpFlag;
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
	static FbxModel* fbxModel7; //�R�オ��
	static FbxModel* fbxModel8; //�_���X
	static FbxModel* fbxModel9; //�L�b�N
	static FbxModel* fbxModel10; //�X���C�f�B���O
	static FbxModel* fbxModel11; //��������
	static FbxModel* fbxModel12; //������
	static FbxModel* fbxModel13; //������
};

