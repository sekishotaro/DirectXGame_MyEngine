#pragma once
#include <string>
#include <DirectXMath.h>
#include <vector>
#include "Object3d.h"
#include "ColliderObject.h"
#include <memory>
#include <map>
#include "nlohmann/json.hpp"

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
		// �^�C�v��
		std::string typeName;
	};

	// �`��p�I�u�W�F�N�g�z��
	std::vector<ObjectData> objects;

	// �����蔻��p�I�u�W�F�N�g�z��
	std::vector<ObjectData> colliderObjects;

	// ���b�V���G���A�p�I�u�W�F�N�g�z��
	std::vector<ObjectData> naviareaObjects;
};

class TouchableObject;

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

	//�N���X�^���p
	//�I�u�W�F�N�g
	static std::vector<std::unique_ptr<Object3d>> crystalObjects;
	//���f��
	static std::map<std::string, Model> crystalModels;
	//�R���C�_�[�p
	//�I�u�W�F�N�g
	static std::vector<std::unique_ptr<ColliderObject>> crystalColliderObjects;
	//���f��
	static std::map<std::string, ColliderModel> crystalColliderModels;

	//���p
	//�I�u�W�F�N�g
	static std::vector<std::unique_ptr<Object3d>> groundObjects;
	
	static TouchableObject* objGround;
	//���f��
	static std::map<std::string, Model> groundModels;
	
	//�R���C�_�[�p
	//�I�u�W�F�N�g
	static std::vector<std::unique_ptr<ColliderObject>> groundColliderObjects;
	//���f��
	static std::map<std::string, ColliderModel> groundColliderModels;

	//�G�p
	//�I�u�W�F�N�g
	static std::vector<std::unique_ptr<Object3d>> enemyObjects;
	//���f��
	static std::map<std::string, Model> enemyModels;
	//�R���C�_�[�p
	//�I�u�W�F�N�g
	static std::vector<std::unique_ptr<ColliderObject>> enemyColliderObjects;
	//���f��
	static std::map<std::string, ColliderModel> enemyColliderModels;
	//�i�r�G���A�p
	//�I�u�W�F�N�g
	static std::vector<std::unique_ptr<ColliderObject>> enemyNaviareaObjects;
	//���f��
	static std::map<std::string, ColliderModel> enemyNaviareaModels;

	//�ǂ̂ڂ�p
	static std::vector<std::unique_ptr<Object3d>> climbWallObjects;
	static std::map<std::string, Model> climbWallModels;

	//�S�[��
	static std::vector<std::unique_ptr<Object3d>> goalObjects;
	static std::map<std::string, Model> goalModels;

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
	//�u���b�N
	static void TypeSetModel( LevelData::ObjectData& objectData);
	static void TypeSetColliderModel( LevelData::ObjectData& colliderObjectData);
	//�N���X�^��
	static void TypeSetCrystalModel(LevelData::ObjectData& objectData);
	static void TypeSetColliderCrystalModel(LevelData::ObjectData& colliderObjectData);
	//�n��
	static void TypeSetGroundModel(LevelData::ObjectData& objectData);
	static void TypeSetColliderGroundModel(LevelData::ObjectData& colliderObjectData);
	//�G
	static void TypeSetEnemyModel(LevelData::ObjectData& objectData);
	static void TypeSetColliderEnemyModel(LevelData::ObjectData& colliderObjectData);
	static void TypeSetNaviareaEnemyModel(LevelData::ObjectData& colliderObjectData);

	//�悶�o����
	static void TypeclimbWallModel(LevelData::ObjectData& objectData);

	//�S�[��
	static void TypeGoalModel(LevelData::ObjectData& objectData);
};

