#pragma once
#include "FbxModel.h"
#include "Camera.h"
#include "FbxLoader.h"

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include <string>

#include "CollisionInfo.h"

class BaseCollider;

class FbxObject3d
{
protected: // �G�C���A�X
// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:	//�萔
	//�{�[���̍ő吔
	static const int MAX_BONES = 128;

public: // �T�u�N���X
	// �萔�o�b�t�@�p�f�[�^�\����(���W�ϊ��s��p)
	struct ConstBufferDataTransform
	{
		XMMATRIX viewproj;		//�r���[�v���W�F�N�V�����s��
		XMMATRIX world;			//���[���h�s��
		XMFLOAT3 cameraPos;		//�J�������W (���[���h���W)
	};

	//�萔�o�b�t�@�p�f�[�^�\����(�X�L�j���O)
	struct ConstBufferDataSkin
	{
		XMMATRIX bones[MAX_BONES];
	};

public: // �ÓI�����o�֐�
	// setter
	static void SetDevice(ID3D12Device *device) { FbxObject3d::device = device; }
	static void SetCamera(Camera *camera) { FbxObject3d::camera = camera; }
	

private: // �ÓI�����o�ϐ�
	//�f�o�C�X
	static ID3D12Device *device;
	// �J����
	static Camera *camera;
	//���[�g�V�O�l�`��
	static ComPtr<ID3D12RootSignature> rootsignature;
	//�p�C�v���C���X�e�[�g�I�u�W�F�N�g
	static ComPtr<ID3D12PipelineState> pipelinestate;

public: // �����o�֐�

	FbxObject3d() = default;

	virtual ~FbxObject3d();

	/// <summary>
	/// ������
	/// </summary>
	virtual void Initialize();
	/// <summary>
	/// �O���t�B�b�N�X�p�C�v���C���̐���
	/// </summary>
	static void CreateGraphicsPipeline();
	/// <summary>
	/// �}�C�t���[������
	/// </summary>
	virtual void Update();
	/// <summary>
	/// ���f���̃Z�b�g
	/// </summary>
	/// <param name="model">���f��</param>
	virtual void SetModel(FbxModel *fbxModel) { this->fbxModel = fbxModel; };
	/// <summary>
	/// �`��
	/// </summary>
	/// <param name="cmdList">�R�}���h���X�g</param>
	virtual void Draw(ID3D12GraphicsCommandList *cmdList);

	/// <summary>
	/// �A�j���[�V�����ǂݍ���
	/// </summary>
	virtual void LoadAnimation();

	/// <summary>
	/// �A�j���[�V�����J�n
	/// </summary>
	virtual void PlayAnimation();

	/// <summary>
	/// �R���C�_�[�̃Z�b�g
	/// </summary>
	/// <param name="collider">�R���C�_�[</param>
	void SetCollider(BaseCollider* collider);

	/// <summary>
	/// �Փˎ��R�[���o�b�N�֐�
	/// </summary>
	/// <param name="info">�Փˏ��</param>
	virtual void OnCollision(const CollisionInfo& info) {}

	void UpdateWorldMatrix();

	/// <summary>
	/// ���[���h�s��̎擾
	/// </summary>
	/// <returns>���[���h�s��</returns>
	const XMMATRIX& GetMatWorld() { return matWorld; }

protected:
	// �萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuffTransform;
	//���[�J���X�P�[��
	XMFLOAT3 scale = { 1,1,1 };
	//X,Y,Z�����̃��[�J����]�p
	XMFLOAT3 rotation = { 0,0,0 };
	//���[�J�����W
	XMFLOAT3 position = { 0,0,0 };
	//���[�J�����[���h�ϊ��s��
	XMMATRIX matWorld = {};
	//���f��
	FbxModel *fbxModel = nullptr;
	//�萔�o�b�t�@(�X�L��)
	ComPtr<ID3D12Resource> constBuffSkin;
	//1�t���[���̎���
	FbxTime frameTime;
	//�A�j���[�V�����J�n����
	FbxTime startTime;
	//�A�j���[�V�����I������
	FbxTime endTime;
	//���ݎ���(�A�j���[�V����)
	FbxTime currentTime;
	//�A�j���\�V�����Đ���
	bool isPlay = false;
	//�A�j���[�V�����̐�
	int fbxAnimationNum;
	//�A�j���[�V���������[�h�������̊m�F�p�t���O
	bool LoadedFlag = false;
	//�J��������
	XMFLOAT2 cameraPos2d = {};

	//�N���X�� (�f�o�b�N�p)
	const char* name = nullptr;
	BaseCollider* collider = nullptr;

public:
	//�A�j���[�V���������邩�̃t���O
	bool AnimationFlag = false;
	//�A�j���[�V�����̐�
	int AnimationNum = 0;
public:

	//getter
	bool GetisPlay() { return isPlay; }

	/// <summary>
	/// ���W�̎擾
	/// </summary>
	/// <returns>���W</returns>
	const XMFLOAT3& GetPosition() { return position; }

	/// <summary>
	/// ���W�̐ݒ�
	/// </summary>
	/// <param name="position">���W</param>
	void SetPosition(const XMFLOAT3& position) { this->position = position; }

	/// <summary>
	/// ��]�p�̎擾
	/// </summary>
	/// <returns>��]�p</returns>
	const XMFLOAT3& GetRotation() { return rotation; }

	/// <summary>
	/// ��]�p�̐ݒ�
	/// </summary>
	/// <param name="rotation">��]�p</param>
	void SetRotation(const XMFLOAT3& rotation) { this->rotation = rotation; }

	/// <summary>
	/// �X�P�[���̐ݒ�
	/// </summary>
	/// <param name="scale">�X�P�[��</param>
	void SetScale(const XMFLOAT3& scale) { this->scale = scale; }
};

