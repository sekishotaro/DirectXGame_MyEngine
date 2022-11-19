#pragma once

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>

#include "Input.h"
#include "FbxObject3d.h"
#include "Object3d.h"

class Player : public Object3d
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
	static Player* Create(Model* model = nullptr);

public:

	/// <summary>
	/// ������
	/// </summary>
	/// <returns>����</returns>
	bool Initialize() override;

	/// <summary>
	/// �}�C�t���[������
	/// </summary>
	void Update() override;

	/// <summary>
	/// �Փˎ��R�[���o�b�N�֐�
	/// </summary>
	/// <param name="info">�Փˏ��</param>
	void OnCollision(const CollisionInfo& info) override;


private:
	void PushBack(const DirectX::XMVECTOR& normal, const XMFLOAT3& distance);

	void ClimbWallJudge(const DirectX::XMVECTOR move);

	/// <summary>
	/// �ړ�����
	/// </summary>
	void MoveOperation(DirectX::XMVECTOR& move);

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
	void StaminaManagement(const DirectX::XMVECTOR& move);

	//���ԊǗ�
	void TimeManagement();

public:
	//�f�o�b�N�p�Q�b�^�[
	static XMFLOAT3 GetPos() { return pos; }

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

	static int& GetInputNumX() { return inputX; }
	static int& GetInputNumY() { return inputY; }

private:
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

	//���@�ƃS�[���̓����蔻��
	static bool goalFlag;

	//�����x�N�g��
	DirectX::XMVECTOR fallV = {};
	//�v���C���[�̈ʒu

	static XMFLOAT3 pos;
	static XMFLOAT3 moveV;

	//�ړ��ʒ�����
	static float moveAdjustmentNum;


	static int inputX;
	static int inputY;

	//�N���X�^���̐�
	static int crystalNum;

	//�c�^�ɓ����������̂߂荞�ݎ��J�E���g
	int wallCount = 0;

	static bool climbOperation;
	//�ǂ̂ڂ�p�ۑ��߂荞�ݖ@��
	DirectX::XMVECTOR climbNormal = {};
	//�̂ڂ�p�|���Ƃ̓����蔻��
	static bool climbWallHit;
	//�O�t���[���̓����蔻��̌���
	bool climbWallHitPar = false;
	
	//��������
	static float timeLimit;
	float flame = 0.0f;


	//�X�^�~�i���g����
	static bool staminaBoostFlag;
	//�X�^�~�i�c��
	static float staminaQuantity;
	//�X�^�~�i���Ȃ��Ȃ������̉񕜎���
	float forciblyRecoveryTime = 10.0f;
	//�X�^�~�i���g���邩�ǂ���(�X�^�~�i���g���؂����ꍇ�ɂȂ���)
	static bool staminaCut;
};

