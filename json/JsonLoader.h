#pragma once
#include <string>
#include <DirectXMath.h>
#include <vector>
#include "Object3d.h"
#include "ColliderObject.h"
#include <memory>
#include <map>

// ���x���f�[�^
struct LevelData {

	struct ObjectData {
		// �t�@�C����
		std::string fileName;
		//�R���C�_�[��
		std::string colliderName;
		// ���s�ړ�
		DirectX::XMVECTOR translation;
		// ��]�p
		DirectX::XMVECTOR rotation;
		// �X�P�[�����O
		DirectX::XMVECTOR scaling;
		// �R���C�_�[�p���[�J�����W
		DirectX::XMVECTOR cenyter;
	};

	// �`��p�I�u�W�F�N�g�z��
	std::vector<ObjectData> objects;

	// �����蔻��p�I�u�W�F�N�g�z��
	std::vector<ObjectData> colliderObjects;
};

class JsonLoader
{
private: // �G�C���A�X
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:// �萔
	// �f�t�H���g�̓ǂݍ��݃f�B���N�g��
	static const std::string DefaultDirectory;
	// �t�@�C���g���q
	static const std::string JsonExtension;

	//���x���f�[�^�i�[�p�C���X�^���X�𐶐�
	static LevelData* levelData;

	//�Ǘp
	//�I�u�W�F�N�g
	static std::vector<std::unique_ptr<Object3d>> objects;
	//���f��
	static std::map<std::string, Model> models;
	//�R���C�_�[�p
	//�I�u�W�F�N�g
	static std::vector<std::unique_ptr<ColliderObject>> colliderObjects;
	//���f��
	static std::map<std::string, ColliderModel> colliderModels;

public:

	//�ǂݍ���
	static void LoadFile(const std::string& fileName);

	//�z�u
	static void SetObject();

	//�A�b�v�f�[�g
	static void Update();

	//�`��
	static void Draw();

private:
	//static void WallLoad( nlohmann::json& object);

};

