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

	//�n�`�����蔻��p
	//�I�u�W�F�N�g
	static std::vector<std::unique_ptr<Object3d>> groundObjects;
	static TouchableObject* objGround;
	//���f��
	static std::map<std::string, Model> groundModels;
	//�ʓr�n�`�p�ˋN
	static std::vector<std::unique_ptr<Object3d>> terrainObjects;
	//���f��
	static std::map<std::string, Model> terrainModels;
	//�R���C�_�[�p
	//�I�u�W�F�N�g
	static std::vector<std::unique_ptr<ColliderObject>> groundColliderObjects;
	//���f��
	static std::map<std::string, ColliderModel> groundColliderModels;

	//�T���G�p
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

	//�P���G
	//�I�u�W�F�N�g
	static std::vector<std::unique_ptr<Object3d>> raidEnemyObjects;
	//���f��
	static std::map<std::string, Model> raidEnemyModels;

	//�ǂ̂ڂ�p
	static std::vector<std::unique_ptr<Object3d>> climbWallObjects;
	static std::map<std::string, Model> climbWallModels;

	//�S�[��
	static std::vector<std::unique_ptr<Object3d>> goalObjects;
	static std::map<std::string, Model> goalModels;

	static std::vector<std::unique_ptr<Object3d>> moveBoxObjects;
	static std::map<std::string, Model> moveBoxModels;

	//�ȉ������ڗp
	static std::vector<std::unique_ptr<Object3d>> rockObjects;
	static std::map<std::string, Model> rockModels;

	static std::vector<std::unique_ptr<Object3d>> sandGroundObjects;
	static std::map<std::string, Model> sandGroundModels;

	static bool hitTerrainDrawFlag;
public:

	//�ǂݍ���
	static void LoadFile(const std::string& fileName);

	//�z�u
	static void SetObject();

	//�A�b�v�f�[�g
	static void Update();

	//�`��
	static void Draw();

	//�I��
	static void Finalize();

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
	//�n�`�p�ˋN
	static void TypeSetTerrainModel(LevelData::ObjectData& objectData);
	//�T���G
	static void TypeSetEnemyModel(LevelData::ObjectData& objectData);
	static void TypeSetColliderEnemyModel(LevelData::ObjectData& colliderObjectData);
	static void TypeSetNaviareaEnemyModel(LevelData::ObjectData& colliderObjectData);
	//�P���G
	static void TypeSetRaidEnemyModel(LevelData::ObjectData& objectData);

	//�悶�o����
	static void TypeclimbWallModel(LevelData::ObjectData& objectData);

	//�S�[��
	static void TypeGoalModel(LevelData::ObjectData& objectData);

	//��
	static void TypeMoveBoxModel(LevelData::ObjectData& objectData);

	static void TypeRockModel(LevelData::ObjectData& objectData);

	static void TypeSandGroundModel(LevelData::ObjectData& objectData);
};

