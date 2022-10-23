#pragma once

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include "Mesh.h"

// 3D���f��

class Model
{
private: // �G�C���A�X
// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

private:
	static const std::string baseDirectory;
private: //�ÓI�����o�ϐ�
	//�f�o�C�X
	static ID3D12Device* device;
	// �f�X�N���v�^�T�C�Y
	static UINT descriptorHandleIncrementSize;

public: // �T�u�N���X	
	
	//���b�V���N���X�Ɉڍs
	//// ���_�f�[�^�\���́i�e�N�X�`������j
	//struct VertexPosNormalUv
	//{
	//	XMFLOAT3 pos; // xyz���W
	//	XMFLOAT3 normal; // �@���x�N�g��
	//	XMFLOAT2 uv;  // uv���W
	//};

	//�}�e���A���N���X�Ɉڍs
	// �萔�o�b�t�@�p�f�[�^�\����B1
	//struct ConstBufferDataB1
	//{
	//	XMFLOAT3 ambient;  //�A���r�G���g�W��
	//	float pad1;        //�p�f�B���O
	//	XMFLOAT3 diffuse;  //�f�B�t���[�Y�W��
	//	float pad2;        //�p�f�B���O
	//	XMFLOAT3 specular; //�X�y�L�����[�W��
	//	float alpha;       //�A���t�@
	//};

	//struct Material
	//{
	//	std::string name;             //�}�e���A����
	//	XMFLOAT3 ambient;             //�A���r�G���g�e���x
	//	XMFLOAT3 diffuse;             //�f�B�t���[�Y�e���x
	//	XMFLOAT3 specular;            //�X�y�L�����[�e���x
	//	float alpha;                  //�A���t�@
	//	std::string textureFilename;  //�e�N�X�`���t�@�C����
	//	//�R���X�g���N�^
	//	Material()
	//	{
	//		ambient = { 0.3f, 0.3f, 0.3f };
	//		diffuse = { 0.0f, 0.0f, 0.0f };
	//		specular = { 0.0f, 0.0f, 0.0f };
	//		alpha = 1.0f;
	//	}
	//};

public: // �ÓI�����o�֐�
	
	static Model *LoadFromOBJ(const std::string &modelname);


	static void SetDevice(ID3D12Device* device);


	void Initialize(const std::string& modelname);

	/// <summary>
	/// �f�X�N���v�^�q�[�v�̏�����
	/// </summary>
	/// <returns></returns>
	void InitializeDescriptorHeap();

	//�e��o�b�t�@����
	void CreateBuffers();

private:
	// ���O
	std::string name;
	// ���b�V���R���e�i
	std::vector<Mesh*> meshes;
	// �}�e���A���R���e�i
	std::unordered_map<std::string, Material*> materials;
	// �f�t�H���g�}�e���A��
	Material* defaultMaterial = nullptr;
	// �f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> descHeap;

	ComPtr<ID3D12DescriptorHeap> _deprhSRVHeap;

private: //����J�̃����o�֐�
	// OBJ�t�@�C������3D���f����ǂݍ��� (����J)
	void LoadFormOBJInternal(const std:: string &modelname);


private: // �����o�֐�
	
	/// <summary>
	/// �}�e���A���o�^
	/// </summary>
	void AddMaterial(Material* material);

	/// <summary>
	/// �}�e���A���ǂݍ���
	/// </summary>
	void LoadMaterial(const std::string &directoryPath, const std::string &filename);

	/// <summary>
	/// �e�N�X�`���ǂݍ���
	/// </summary>
	void LoadTexture();

public: //�����o�֐�
	void Draw(ID3D12GraphicsCommandList *cmdList);

	inline const std::vector<Mesh*>& GetMeshes() { return meshes; }

};