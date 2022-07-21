#include "JsonLoader.h"
#include <fstream>
#include <cassert>
#include "nlohmann/json.hpp"

std::vector<std::unique_ptr<Object3d>> JsonLoader::objects;
std::map< std::string, Model> JsonLoader::models;
std::vector<std::unique_ptr<ColliderObject>> JsonLoader::colliderObjects;
std::map< std::string, ColliderModel> JsonLoader::colliderModels;
LevelData* JsonLoader::levelData;

const std::string JsonLoader::DefaultDirectory = "Resources/levels/";
const std::string JsonLoader::JsonExtension = ".json";

void JsonLoader::LoadFile(const std::string& fileName)
{

	// �A�����ăt���p�X�𓾂�
	const std::string fullpath = DefaultDirectory + fileName + JsonExtension;

	// �t�@�C���X�g���[��
	std::ifstream file;

	// �t�@�C�����J��
	file.open(fullpath);
	// �t�@�C���I�[�v�����s���`�F�b�N
	if (file.fail()) {
		assert(0);
	}

	// JSON�����񂩂�𓀂����f�[�^
	nlohmann::json deserialized;

	// ��
	file >> deserialized;

	// ���������x���f�[�^�t�@�C�����`�F�b�N
	assert(deserialized.is_object());
	assert(deserialized.contains("name"));
	assert(deserialized["name"].is_string());

	// "name"�𕶎���Ƃ��Ď擾
	std::string name = deserialized["name"].get<std::string>();
	// ���������x���f�[�^�t�@�C�����`�F�b�N
	assert(name.compare("scene") == 0);

	//���x���f�[�^�i�[�p�C���X�^���X�𐶐�
	levelData = new LevelData();

	// "objects"�̑S�I�u�W�F�N�g�𑖍�
	for (nlohmann::json& object : deserialized["objects"])
	{
		assert(object.contains("type"));

		//��ʂ��擾
		std::string type = object["type"].get<std::string>();

		//Mesh
		if (type.compare("MESH") == 0)
		{
			//�v�f�ǉ�
			levelData->objects.emplace_back(LevelData::ObjectData{});
			//���ǉ������v�f�̎Q�Ƃ𓾂�
			LevelData::ObjectData& objectData = levelData->objects.back();

			if (object.contains("file_name"))
			{
				//�t�@�C����
				objectData.fileName = object["file_name"];
			}

			//�g�����X�t�H�[���̃p�����[�^�[�ǂݍ���
			nlohmann::json& transform = object["transform"];
			//���s�ړ�
			objectData.translation.m128_f32[0] =  (float)transform["translation"][1];
			objectData.translation.m128_f32[1] =  (float)transform["translation"][2];
			objectData.translation.m128_f32[2] = -(float)transform["translation"][0];
			objectData.translation.m128_f32[3] =  1.0;
			//��]�p
			objectData.rotation.m128_f32[0] = -(float)transform["rotation"][1];
			objectData.rotation.m128_f32[1] = -(float)transform["rotation"][2];
			objectData.rotation.m128_f32[2] =  (float)transform["rotation"][0];
			objectData.rotation.m128_f32[3] = 0.0;
			//�X�P�[�����O
			objectData.scaling.m128_f32[0] = (float)transform["scaling"][1];
			objectData.scaling.m128_f32[1] = (float)transform["scaling"][2];
			objectData.scaling.m128_f32[2] = (float)transform["scaling"][0];
			objectData.scaling.m128_f32[3] = 0.0;

			// TODO: �I�u�W�F�N�g�������ċA�֐��ɂ܂Ƃ߁A�ċA�ďo���Ŏ}�𑖍�����
			if (object.contains("children"))
			{
			}

			//�R���C�_�[
			if (object.contains("collider"))
			{
				levelData->colliderObjects.emplace_back(LevelData::ObjectData{});
				LevelData::ObjectData& colliderObjectData = levelData->colliderObjects.back();
				nlohmann::json& collider = object["collider"];
				colliderObjectData.fileName = collider["type"];
				if (object.contains("BOX") == 0)
				{
					//�I�u�W�F�N�g�ɑ΂��Ă̓����蔻��̈ʒu
					colliderObjectData.cenyter.m128_f32[0] = (float)collider["center"][1];
					colliderObjectData.cenyter.m128_f32[1] = (float)collider["center"][2];
					colliderObjectData.cenyter.m128_f32[2] = -(float)collider["center"][0];
					colliderObjectData.cenyter.m128_f32[3] = 1.0;
					//�I�u�W�F�N�g�̈ʒu
					colliderObjectData.translation.m128_f32[0] = (float)transform["translation"][1];
					colliderObjectData.translation.m128_f32[1] = (float)transform["translation"][2];
					colliderObjectData.translation.m128_f32[2] = -(float)transform["translation"][0];
					colliderObjectData.translation.m128_f32[3] = 1.0;
					//��]�p
					//colliderObjectData.rotation.m128_f32[0] = -(float)collider["rotation"][1];
					//colliderObjectData.rotation.m128_f32[1] = -(float)collider["rotation"][2];
					//colliderObjectData.rotation.m128_f32[2] = (float)collider["rotation"][0];
					//colliderObjectData.rotation.m128_f32[3] = 0.0;
					//�X�P�[�����O
					colliderObjectData.scaling.m128_f32[0] = (float)collider["size"][1];
					colliderObjectData.scaling.m128_f32[1] = (float)collider["size"][2];
					colliderObjectData.scaling.m128_f32[2] = (float)collider["size"][0];
					colliderObjectData.scaling.m128_f32[3] = 0.0;
				}
				objectData.colliderName = colliderObjectData.fileName;
			}
		}
	}
}

void JsonLoader::SetObject()
{
	//���x���f�[�^����I�u�W�F�N�g�𐶐�,�z�u
	for (auto& objectData : levelData->objects)
	{
		//�t�@�C��������o�^�ς݃��f��������
		Model* model = nullptr;
		model = Model::LoadFromOBJ(objectData.fileName);
		models[objectData.fileName] = *model;

		//���f�����w�肵��3D�I�u�W�F�N�g�𐶐�
		std::unique_ptr<Object3d> newObject;
		newObject = Object3d::Create();
		newObject->SetModel(model);

		//���W
		XMFLOAT3 pos;
		DirectX::XMStoreFloat3(&pos, objectData.translation);
		newObject->SetPosition(pos);

		//��]�p
		XMFLOAT3 rot;
		DirectX::XMStoreFloat3(&rot, objectData.rotation);
		rot.y -= 90.0f;
		newObject->SetRotation(rot);

		//�X�P�[��
		XMFLOAT3 scale;
		DirectX::XMStoreFloat3(&scale, objectData.scaling);
		newObject->SetScale(scale);

		//�z��̍Ō�ɓo�^
		objects.push_back(std::move(newObject));
	}

	//���x���f�[�^����I�u�W�F�N�g�𐶐�,�z�u
	for (auto& colliderObjectData : levelData->colliderObjects)
	{
		//�t�@�C��������o�^�ς݃��f��������
		ColliderModel* colliderModel = nullptr;
		colliderModel = ColliderModel::ColliderModelCreate(colliderObjectData.fileName);
		colliderModels[colliderObjectData.fileName] = *colliderModel;

		//���f�����w�肵��3D�I�u�W�F�N�g�𐶐�
		std::unique_ptr<ColliderObject> newObject;
		newObject = ColliderObject::Create();
		newObject->SetModel(colliderModel);

		//���W
		XMFLOAT3 pos;
		DirectX::XMStoreFloat3(&pos, colliderObjectData.translation);
		
		//�R���C�_�[�p���[�J�����W
		XMFLOAT3 centerPos;
		DirectX::XMStoreFloat3(&centerPos, colliderObjectData.cenyter);
		pos.x = pos.x + centerPos.x;
		pos.y = pos.y + centerPos.y;
		pos.z = pos.z + centerPos.z;

		newObject->SetPosition(pos);

		//��]�p
		XMFLOAT3 rot {0,0,0};
		//DirectX::XMStoreFloat3(&rot, colliderObjectData.rotation);
		//rot.y -= 90.0f;
		newObject->SetRotation(rot);

		//�X�P�[��
		XMFLOAT3 scale;
		DirectX::XMStoreFloat3(&scale, colliderObjectData.scaling);
		newObject->SetScale(scale);

		//�z��̍Ō�ɓo�^
		colliderObjects.push_back(std::move(newObject));
	}

}

void JsonLoader::Update()
{
	//�`��I�u�W�F�N�g
	for (int i = 0; i < objects.size(); i++)
	{
		objects[i]->Update();
	}

	//�����蔻��p�I�u�W�F�N�g
	for (int i = 0; i < colliderObjects.size(); i++)
	{
		colliderObjects[i]->Update();
	}
}

void JsonLoader::Draw()
{
	//�`��I�u�W�F�N�g
	for (int i = 0; i < objects.size(); i++)
	{
		objects[i]->Draw();
	}

	//�����蔻��p�I�u�W�F�N�g
	for (int i = 0; i < objects.size(); i++)
	{
		colliderObjects[i]->Draw();
	}
}
