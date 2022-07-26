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

	// 連結してフルパスを得る
	const std::string fullpath = DefaultDirectory + fileName + JsonExtension;

	// ファイルストリーム
	std::ifstream file;

	// ファイルを開く
	file.open(fullpath);
	// ファイルオープン失敗をチェック
	if (file.fail()) {
		assert(0);
	}

	// JSON文字列から解凍したデータ
	nlohmann::json deserialized;

	// 解凍
	file >> deserialized;

	// 正しいレベルデータファイルかチェック
	assert(deserialized.is_object());
	assert(deserialized.contains("name"));
	assert(deserialized["name"].is_string());

	// "name"を文字列として取得
	std::string name = deserialized["name"].get<std::string>();
	// 正しいレベルデータファイルかチェック
	assert(name.compare("scene") == 0);

	//レベルデータ格納用インスタンスを生成
	levelData = new LevelData();

	// "objects"の全オブジェクトを走査
	for (nlohmann::json& object : deserialized["objects"])
	{
		assert(object.contains("type"));

		//種別を取得
		std::string type = object["type"].get<std::string>();

		//Mesh
		if (type.compare("MESH") == 0)
		{
			//要素追加
			levelData->objects.emplace_back(LevelData::ObjectData{});
			//今追加した要素の参照を得る
			LevelData::ObjectData& objectData = levelData->objects.back();

			if (object.contains("file_name"))
			{
				//ファイル名
				objectData.fileName = object["file_name"];
			}

			//トランスフォームのパラメーター読み込み
			nlohmann::json& transform = object["transform"];
			//平行移動
			objectData.translation.m128_f32[0] =  (float)transform["translation"][1];
			objectData.translation.m128_f32[1] =  (float)transform["translation"][2];
			objectData.translation.m128_f32[2] = -(float)transform["translation"][0];
			objectData.translation.m128_f32[3] =  1.0;
			//回転角
			objectData.rotation.m128_f32[0] = -(float)transform["rotation"][1];
			objectData.rotation.m128_f32[1] = -(float)transform["rotation"][2];
			objectData.rotation.m128_f32[2] =  (float)transform["rotation"][0];
			objectData.rotation.m128_f32[3] = 0.0;
			//スケーリング
			objectData.scaling.m128_f32[0] = (float)transform["scaling"][1];
			objectData.scaling.m128_f32[1] = (float)transform["scaling"][2];
			objectData.scaling.m128_f32[2] = (float)transform["scaling"][0];
			objectData.scaling.m128_f32[3] = 0.0;

			// TODO: オブジェクト走査を再帰関数にまとめ、再帰呼出しで枝を走査する
			if (object.contains("children"))
			{
			}

			//コライダー
			if (object.contains("collider"))
			{
				levelData->colliderObjects.emplace_back(LevelData::ObjectData{});
				LevelData::ObjectData& colliderObjectData = levelData->colliderObjects.back();
				nlohmann::json& collider = object["collider"];
				colliderObjectData.fileName = collider["type"];
				if (object.contains("BOX") == 0)
				{
					//オブジェクトに対しての当たり判定の位置
					colliderObjectData.cenyter.m128_f32[0] = (float)collider["center"][1];
					colliderObjectData.cenyter.m128_f32[1] = (float)collider["center"][2];
					colliderObjectData.cenyter.m128_f32[2] = -(float)collider["center"][0];
					colliderObjectData.cenyter.m128_f32[3] = 1.0;
					//オブジェクトの位置
					colliderObjectData.translation.m128_f32[0] = (float)transform["translation"][1];
					colliderObjectData.translation.m128_f32[1] = (float)transform["translation"][2];
					colliderObjectData.translation.m128_f32[2] = -(float)transform["translation"][0];
					colliderObjectData.translation.m128_f32[3] = 1.0;
					//回転角
					//colliderObjectData.rotation.m128_f32[0] = -(float)collider["rotation"][1];
					//colliderObjectData.rotation.m128_f32[1] = -(float)collider["rotation"][2];
					//colliderObjectData.rotation.m128_f32[2] = (float)collider["rotation"][0];
					//colliderObjectData.rotation.m128_f32[3] = 0.0;
					//スケーリング
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
	//レベルデータからオブジェクトを生成,配置
	for (auto& objectData : levelData->objects)
	{
		//ファイル名から登録済みモデルを検索
		Model* model = nullptr;
		model = Model::LoadFromOBJ(objectData.fileName);
		models[objectData.fileName] = *model;

		//モデルを指定して3Dオブジェクトを生成
		std::unique_ptr<Object3d> newObject;
		newObject = Object3d::Create();
		newObject->SetModel(model);

		//座標
		XMFLOAT3 pos;
		DirectX::XMStoreFloat3(&pos, objectData.translation);
		newObject->SetPosition(pos);

		//回転角
		XMFLOAT3 rot;
		DirectX::XMStoreFloat3(&rot, objectData.rotation);
		rot.y -= 90.0f;
		newObject->SetRotation(rot);

		//スケール
		XMFLOAT3 scale;
		DirectX::XMStoreFloat3(&scale, objectData.scaling);
		newObject->SetScale(scale);

		//配列の最後に登録
		objects.push_back(std::move(newObject));
	}

	//レベルデータからオブジェクトを生成,配置
	for (auto& colliderObjectData : levelData->colliderObjects)
	{
		//ファイル名から登録済みモデルを検索
		ColliderModel* colliderModel = nullptr;
		colliderModel = ColliderModel::ColliderModelCreate(colliderObjectData.fileName);
		colliderModels[colliderObjectData.fileName] = *colliderModel;

		//モデルを指定して3Dオブジェクトを生成
		std::unique_ptr<ColliderObject> newObject;
		newObject = ColliderObject::Create();
		newObject->SetModel(colliderModel);

		//座標
		XMFLOAT3 pos;
		DirectX::XMStoreFloat3(&pos, colliderObjectData.translation);
		
		//コライダー用ローカル座標
		XMFLOAT3 centerPos;
		DirectX::XMStoreFloat3(&centerPos, colliderObjectData.cenyter);
		pos.x = pos.x + centerPos.x;
		pos.y = pos.y + centerPos.y;
		pos.z = pos.z + centerPos.z;

		newObject->SetPosition(pos);

		//回転角
		XMFLOAT3 rot {0,0,0};
		//DirectX::XMStoreFloat3(&rot, colliderObjectData.rotation);
		//rot.y -= 90.0f;
		newObject->SetRotation(rot);

		//スケール
		XMFLOAT3 scale;
		DirectX::XMStoreFloat3(&scale, colliderObjectData.scaling);
		newObject->SetScale(scale);

		//配列の最後に登録
		colliderObjects.push_back(std::move(newObject));
	}

}

void JsonLoader::Update()
{
	//描画オブジェクト
	for (int i = 0; i < objects.size(); i++)
	{
		objects[i]->Update();
	}

	//当たり判定用オブジェクト
	for (int i = 0; i < colliderObjects.size(); i++)
	{
		colliderObjects[i]->Update();
	}
}

void JsonLoader::Draw()
{
	//描画オブジェクト
	for (int i = 0; i < objects.size(); i++)
	{
		objects[i]->Draw();
	}

	//当たり判定用オブジェクト
	for (int i = 0; i < objects.size(); i++)
	{
		colliderObjects[i]->Draw();
	}
}
