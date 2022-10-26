#include "JsonLoader.h"
#include <fstream>
#include <cassert>

std::vector<std::unique_ptr<Object3d>> JsonLoader::objects;
std::map< std::string, Model> JsonLoader::models;
std::vector<std::unique_ptr<ColliderObject>> JsonLoader::colliderObjects;
std::map< std::string, ColliderModel> JsonLoader::colliderModels;

std::vector<std::unique_ptr<Object3d>> JsonLoader::crystalObjects;
std::map< std::string, Model> JsonLoader::crystalModels;
std::vector<std::unique_ptr<ColliderObject>> JsonLoader::crystalColliderObjects;
std::map< std::string, ColliderModel> JsonLoader::crystalColliderModels;

std::vector<std::unique_ptr<Object3d>> JsonLoader::groundObjects;
std::map< std::string, Model> JsonLoader::groundModels;
std::vector<std::unique_ptr<ColliderObject>> JsonLoader::groundColliderObjects;
std::map< std::string, ColliderModel> JsonLoader::groundColliderModels;

std::vector<std::unique_ptr<Object3d>> JsonLoader::enemyObjects;
std::map< std::string, Model> JsonLoader::enemyModels;
std::vector<std::unique_ptr<ColliderObject>> JsonLoader::enemyColliderObjects;
std::map< std::string, ColliderModel> JsonLoader::enemyColliderModels;
std::vector<std::unique_ptr<ColliderObject>> JsonLoader::enemyNaviareaObjects;
std::map< std::string, ColliderModel> JsonLoader::enemyNaviareaModels;

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
			////要素追加
			levelData->objects.emplace_back(LevelData::ObjectData{});
			//今追加した要素の参照を得る
			LevelData::ObjectData& objectData = levelData->objects.back();

			if (object.contains("file_name"))
			{
				//ファイル名
				objectData.fileName = object["file_name"];
			}

			std::string typeName = object["type_name"].get<std::string>();
			objectData.typeName = typeName;

			//トランスフォームのパラメーター読み込み
			nlohmann::json& transform = object["transform"];
			//平行移動
			objectData.translation.m128_f32[0] = (float)transform["translation"][1];
			objectData.translation.m128_f32[1] = (float)transform["translation"][2];
			objectData.translation.m128_f32[2] = -(float)transform["translation"][0];
			objectData.translation.m128_f32[3] = 1.0;
			//回転角
			objectData.rotation.m128_f32[0] = -(float)transform["rotation"][1];
			objectData.rotation.m128_f32[1] = -(float)transform["rotation"][2];
			objectData.rotation.m128_f32[2] = (float)transform["rotation"][0];
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
				colliderObjectData.typeName = typeName;
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

			if (object.contains("naviarea"))
			{
				levelData->naviareaObjects.emplace_back(LevelData::ObjectData{});
				LevelData::ObjectData& naviareaObjectData = levelData->naviareaObjects.back();
				nlohmann::json& naviarea = object["naviarea"];
				naviareaObjectData.fileName = naviarea["type"];
				naviareaObjectData.typeName = typeName;
				if (object.contains("BOX") == 0)
				{
					//オブジェクトに対しての当たり判定の位置
					naviareaObjectData.cenyter.m128_f32[0] = (float)naviarea["center"][1];
					naviareaObjectData.cenyter.m128_f32[1] = (float)naviarea["center"][2];
					naviareaObjectData.cenyter.m128_f32[2] = -(float)naviarea["center"][0];
					naviareaObjectData.cenyter.m128_f32[3] = 1.0;
					//オブジェクトの位置
					naviareaObjectData.translation.m128_f32[0] = (float)transform["translation"][1];
					naviareaObjectData.translation.m128_f32[1] = (float)transform["translation"][2];
					naviareaObjectData.translation.m128_f32[2] = -(float)transform["translation"][0];
					naviareaObjectData.translation.m128_f32[3] = 1.0;
					//回転角
					//colliderObjectData.rotation.m128_f32[0] = -(float)collider["rotation"][1];
					//colliderObjectData.rotation.m128_f32[1] = -(float)collider["rotation"][2];
					//colliderObjectData.rotation.m128_f32[2] = (float)collider["rotation"][0];
					//colliderObjectData.rotation.m128_f32[3] = 0.0;
					//スケーリング
					naviareaObjectData.scaling.m128_f32[0] = (float)naviarea["size"][1];
					naviareaObjectData.scaling.m128_f32[1] = (float)naviarea["size"][2];
					naviareaObjectData.scaling.m128_f32[2] = (float)naviarea["size"][0];
					naviareaObjectData.scaling.m128_f32[3] = 0.0;
				}
				objectData.colliderName = naviareaObjectData.fileName;
			}
		}
	}
}

void JsonLoader::SetObject()
{
	//レベルデータからオブジェクトを生成,配置
	for (auto& objectData : levelData->objects)
	{
		if (objectData.typeName == "wall")
		{
			TypeSetModel(objectData);
		}
		else if (objectData.typeName == "crystal")
		{
			TypeSetCrystalModel(objectData);
		}
		else if (objectData.typeName == "ground")
		{
			TypeSetGroundModel(objectData);
		}
		else if (objectData.typeName == "enemy")
		{
			TypeSetEnemyModel(objectData);
		}
	}

	//レベルデータからコライダーオブジェクトを生成,配置
	for (auto& colliderObjectData : levelData->colliderObjects)
	{
		if (colliderObjectData.typeName == "wall")
		{
			TypeSetColliderModel(colliderObjectData);
		}
		else if (colliderObjectData.typeName == "crystal")
		{
			TypeSetColliderCrystalModel(colliderObjectData);
		}
		else if (colliderObjectData.typeName == "ground")
		{
			TypeSetColliderGroundModel(colliderObjectData);
		}
		else if (colliderObjectData.typeName == "enemy")
		{
			TypeSetColliderEnemyModel(colliderObjectData);
		}
	}

	for (auto& naviareaObjectData : levelData->naviareaObjects)
	{
		if (naviareaObjectData.typeName == "enemy")
		{
			TypeSetNaviareaEnemyModel(naviareaObjectData);
		}
	}
}

void JsonLoader::Update()
{
	//描画オブジェクト
	for (int i = 0; i < objects.size(); i++)
	{
		objects[i]->Update();
	}
	
	for (int i = 0; i < crystalObjects.size(); i++)
	{
		crystalObjects[i]->Update();
	}

	for (int i = 0; i < groundObjects.size(); i++)
	{
		groundObjects[i]->Update();
	}

	//当たり判定用オブジェクト
	for (int i = 0; i < colliderObjects.size(); i++)
	{
		if (colliderObjects[i].get()->GetCollFlag() == true)
		{
			colliderObjects[i].get()->SetColor({ 1,1,0 });
		}
		else
		{
			colliderObjects[i].get()->SetColor({ 1,0,0 });
		}
		

		colliderObjects[i]->Update();
	}

	for (int i = 0; i < crystalColliderObjects.size(); i++)
	{
		if (crystalColliderObjects[i].get()->GetCollFlag() == true)
		{
			crystalColliderObjects[i].get()->SetColor({ 1,1,0 });
		}
		else
		{
			crystalColliderObjects[i].get()->SetColor({ 1,0,0 });
		}


		crystalColliderObjects[i]->Update();
	}

	for (int i = 0; i < groundColliderObjects.size(); i++)
	{
		if (groundColliderObjects[i].get()->GetCollFlag() == true)
		{
			groundColliderObjects[i].get()->SetColor({ 1,1,0 });
		}
		else
		{
			groundColliderObjects[i].get()->SetColor({ 1,0,0 });
		}


		groundColliderObjects[i]->Update();
	}

	for (int i = 0; i < enemyNaviareaObjects.size(); i++)
	{
		if (enemyNaviareaObjects[i].get()->GetCollFlag() == true)
		{
			enemyNaviareaObjects[i].get()->SetColor({ 1,1,0 });
		}
		else
		{
			enemyNaviareaObjects[i].get()->SetColor({ 0,0,1 });
		}


		enemyNaviareaObjects[i]->Update();
	}

}

void JsonLoader::Draw()
{
	//描画オブジェクト
	for (int i = 0; i < objects.size(); i++)
	{
		objects[i]->Draw();
	}

	for (int i = 0; i < crystalObjects.size(); i++)
	{
		crystalObjects[i]->Draw();
	}

	for (int i = 0; i < groundObjects.size(); i++)
	{
		groundObjects[i]->Draw();
	}

	//当たり判定用オブジェクト
	for (int i = 0; i < colliderObjects.size(); i++)
	{
		//colliderObjects[i]->Draw();
	}

	for (int i = 0; i < crystalColliderObjects.size(); i++)
	{
		//crystalColliderObjects[i]->Draw();
	}

	for (int i = 0; i < groundColliderObjects.size(); i++)
	{
		//groundColliderObjects[i]->Draw();
	}

	for (int i = 0; i < enemyNaviareaObjects.size(); i++)
	{
		//enemyNaviareaObjects[i]->Draw();
	}
}

void JsonLoader::TypeSetModel( LevelData::ObjectData& objectData)
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

void JsonLoader::TypeSetColliderModel(LevelData::ObjectData& colliderObjectData)
{
	//レベルデータからコライダーオブジェクトを生成,配置
	//ファイル名から登録済みモデルを検索
	ColliderModel* colliderModel = nullptr;
	colliderModel = ColliderModel::ColliderModelCreate(colliderObjectData.fileName);
	colliderModels[colliderObjectData.fileName] = *colliderModel;

	//モデルを指定して3Dオブジェクトを生成
	std::unique_ptr<ColliderObject> newColliderObject;
	newColliderObject = ColliderObject::Create();
	newColliderObject->SetModel(colliderModel);

	//座標
	XMFLOAT3 pos;
	DirectX::XMStoreFloat3(&pos, colliderObjectData.translation);

	//コライダー用ローカル座標
	XMFLOAT3 centerPos;
	DirectX::XMStoreFloat3(&centerPos, colliderObjectData.cenyter);
	pos.x = pos.x + centerPos.x;
	pos.y = pos.y + centerPos.y;
	pos.z = pos.z + centerPos.z;

	newColliderObject->SetPosition(pos);

	//回転角
	XMFLOAT3 rot{ 0,0,0 };
	//DirectX::XMStoreFloat3(&rot, colliderObjectData.rotation);
	//rot.y -= 90.0f;
	newColliderObject->SetRotation(rot);

	//スケール
	XMFLOAT3 scale;
	DirectX::XMStoreFloat3(&scale, colliderObjectData.scaling);
	newColliderObject->SetScale(scale);

	//配列の最後に登録
	colliderObjects.push_back(std::move(newColliderObject));
}

void JsonLoader::TypeSetCrystalModel(LevelData::ObjectData& objectData)
{
	//ファイル名から登録済みモデルを検索
	Model* model = nullptr;
	model = Model::LoadFromOBJ(objectData.fileName);
	crystalModels[objectData.fileName] = *model;

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
	crystalObjects.push_back(std::move(newObject));
}

void JsonLoader::TypeSetColliderCrystalModel(LevelData::ObjectData& colliderObjectData)
{
	//レベルデータからコライダーオブジェクトを生成,配置
	//ファイル名から登録済みモデルを検索
	ColliderModel* colliderModel = nullptr;
	colliderModel = ColliderModel::ColliderModelCreate(colliderObjectData.fileName);
	crystalColliderModels[colliderObjectData.fileName] = *colliderModel;

	//モデルを指定して3Dオブジェクトを生成
	std::unique_ptr<ColliderObject> newColliderObject;
	newColliderObject = ColliderObject::Create();
	newColliderObject->SetModel(colliderModel);

	//座標
	XMFLOAT3 pos;
	DirectX::XMStoreFloat3(&pos, colliderObjectData.translation);

	//コライダー用ローカル座標
	XMFLOAT3 centerPos;
	DirectX::XMStoreFloat3(&centerPos, colliderObjectData.cenyter);
	pos.x = pos.x + centerPos.x;
	pos.y = pos.y + centerPos.y;
	pos.z = pos.z + centerPos.z;

	newColliderObject->SetPosition(pos);

	//回転角
	XMFLOAT3 rot{ 0,0,0 };
	//DirectX::XMStoreFloat3(&rot, colliderObjectData.rotation);
	//rot.y -= 90.0f;
	newColliderObject->SetRotation(rot);

	//スケール
	XMFLOAT3 scale;
	DirectX::XMStoreFloat3(&scale, colliderObjectData.scaling);
	newColliderObject->SetScale(scale);

	//配列の最後に登録
	crystalColliderObjects.push_back(std::move(newColliderObject));
}

void JsonLoader::TypeSetGroundModel(LevelData::ObjectData& objectData)
{
	//ファイル名から登録済みモデルを検索
	Model* model = nullptr;
	model = Model::LoadFromOBJ(objectData.fileName);
	groundModels[objectData.fileName] = *model;

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
	groundObjects.push_back(std::move(newObject));
}

void JsonLoader::TypeSetColliderGroundModel(LevelData::ObjectData& colliderObjectData)
{
	//レベルデータからコライダーオブジェクトを生成,配置
//ファイル名から登録済みモデルを検索
	ColliderModel* colliderModel = nullptr;
	colliderModel = ColliderModel::ColliderModelCreate(colliderObjectData.fileName);
	groundColliderModels[colliderObjectData.fileName] = *colliderModel;

	//モデルを指定して3Dオブジェクトを生成
	std::unique_ptr<ColliderObject> newColliderObject;
	newColliderObject = ColliderObject::Create();
	newColliderObject->SetModel(colliderModel);

	//座標
	XMFLOAT3 pos;
	DirectX::XMStoreFloat3(&pos, colliderObjectData.translation);

	//コライダー用ローカル座標
	XMFLOAT3 centerPos;
	DirectX::XMStoreFloat3(&centerPos, colliderObjectData.cenyter);
	pos.x = pos.x + centerPos.x;
	pos.y = pos.y + centerPos.y;
	pos.z = pos.z + centerPos.z;

	newColliderObject->SetPosition(pos);

	//回転角
	XMFLOAT3 rot{ 0,0,0 };
	//DirectX::XMStoreFloat3(&rot, colliderObjectData.rotation);
	//rot.y -= 90.0f;
	newColliderObject->SetRotation(rot);

	//スケール
	XMFLOAT3 scale;
	DirectX::XMStoreFloat3(&scale, colliderObjectData.scaling);
	newColliderObject->SetScale(scale);

	//配列の最後に登録
	groundColliderObjects.push_back(std::move(newColliderObject));
}

void JsonLoader::TypeSetEnemyModel(LevelData::ObjectData& objectData)
{
	//ファイル名から登録済みモデルを検索
	Model* model = nullptr;
	model = Model::LoadFromOBJ(objectData.fileName);
	enemyModels[objectData.fileName] = *model;

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
	enemyObjects.push_back(std::move(newObject));
}

void JsonLoader::TypeSetColliderEnemyModel(LevelData::ObjectData& colliderObjectData)
{
	//レベルデータからコライダーオブジェクトを生成,配置
	//ファイル名から登録済みモデルを検索
	ColliderModel* colliderModel = nullptr;
	colliderModel = ColliderModel::ColliderModelCreate(colliderObjectData.fileName);
	enemyColliderModels[colliderObjectData.fileName] = *colliderModel;

	//モデルを指定して3Dオブジェクトを生成
	std::unique_ptr<ColliderObject> newColliderObject;
	newColliderObject = ColliderObject::Create();
	newColliderObject->SetModel(colliderModel);

	//座標
	XMFLOAT3 pos;
	DirectX::XMStoreFloat3(&pos, colliderObjectData.translation);

	//コライダー用ローカル座標
	XMFLOAT3 centerPos;
	DirectX::XMStoreFloat3(&centerPos, colliderObjectData.cenyter);
	pos.x = pos.x + centerPos.x;
	pos.y = pos.y + centerPos.y;
	pos.z = pos.z + centerPos.z;

	newColliderObject->SetPosition(pos);

	//回転角
	XMFLOAT3 rot{ 0,0,0 };
	//DirectX::XMStoreFloat3(&rot, colliderObjectData.rotation);
	//rot.y -= 90.0f;
	newColliderObject->SetRotation(rot);

	//スケール
	XMFLOAT3 scale;
	DirectX::XMStoreFloat3(&scale, colliderObjectData.scaling);
	newColliderObject->SetScale(scale);

	//配列の最後に登録
	enemyColliderObjects.push_back(std::move(newColliderObject));
}

void JsonLoader::TypeSetNaviareaEnemyModel(LevelData::ObjectData& colliderObjectData)
{
	//レベルデータからコライダーオブジェクトを生成,配置
	//ファイル名から登録済みモデルを検索
	ColliderModel* colliderModel = nullptr;
	colliderModel = ColliderModel::ColliderModelCreate(colliderObjectData.fileName);
	enemyNaviareaModels[colliderObjectData.fileName] = *colliderModel;

	//モデルを指定して3Dオブジェクトを生成
	std::unique_ptr<ColliderObject> newColliderObject;
	newColliderObject = ColliderObject::Create();
	newColliderObject->SetModel(colliderModel);

	//座標
	XMFLOAT3 pos;
	DirectX::XMStoreFloat3(&pos, colliderObjectData.translation);

	//コライダー用ローカル座標
	XMFLOAT3 centerPos;
	DirectX::XMStoreFloat3(&centerPos, colliderObjectData.cenyter);
	pos.x = pos.x + centerPos.x;
	pos.y = pos.y + centerPos.y;
	pos.z = pos.z + centerPos.z;

	newColliderObject->SetPosition(pos);

	//回転角
	XMFLOAT3 rot{ 0,0,0 };
	//DirectX::XMStoreFloat3(&rot, colliderObjectData.rotation);
	//rot.y -= 90.0f;
	newColliderObject->SetRotation(rot);

	//スケール
	XMFLOAT3 scale;
	DirectX::XMStoreFloat3(&scale, colliderObjectData.scaling);
	newColliderObject->SetScale(scale);

	//配列の最後に登録
	enemyNaviareaObjects.push_back(std::move(newColliderObject));
}

