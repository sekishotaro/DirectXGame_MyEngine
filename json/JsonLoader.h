#pragma once
#include <string>
#include <DirectXMath.h>
#include <vector>
#include "Object3d.h"
#include "ColliderObject.h"
#include <memory>
#include <map>
#include "nlohmann/json.hpp"

// レベルデータ
struct LevelData {

	struct ObjectData {
		// ファイル名
		std::string fileName;
		//コライダー名
		std::string colliderName;
		// 平行移動
		DirectX::XMVECTOR translation;
		// 回転角
		DirectX::XMVECTOR rotation;
		// スケーリング
		DirectX::XMVECTOR scaling;
		// コライダー用ローカル座標
		DirectX::XMVECTOR cenyter;
		// タイプ名
		std::string typeName;
	};

	// 描画用オブジェクト配列
	std::vector<ObjectData> objects;

	// 当たり判定用オブジェクト配列
	std::vector<ObjectData> colliderObjects;

	// メッシュエリア用オブジェクト配列
	std::vector<ObjectData> naviareaObjects;
};

class TouchableObject;

class JsonLoader
{
private: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:// 定数
	// デフォルトの読み込みディレクトリ
	static const std::string DefaultDirectory;
	// ファイル拡張子
	static const std::string JsonExtension;

	//レベルデータ格納用インスタンスを生成
	static LevelData* levelData;

	//壁用
	//オブジェクト
	static std::vector<std::unique_ptr<Object3d>> objects;
	//モデル
	static std::map<std::string, Model> models;
	//コライダー用
	//オブジェクト
	static std::vector<std::unique_ptr<ColliderObject>> colliderObjects;
	//モデル
	static std::map<std::string, ColliderModel> colliderModels;

	//クリスタル用
	//オブジェクト
	static std::vector<std::unique_ptr<Object3d>> crystalObjects;
	//モデル
	static std::map<std::string, Model> crystalModels;
	//コライダー用
	//オブジェクト
	static std::vector<std::unique_ptr<ColliderObject>> crystalColliderObjects;
	//モデル
	static std::map<std::string, ColliderModel> crystalColliderModels;

	//地形当たり判定用
	//オブジェクト
	static std::vector<std::unique_ptr<Object3d>> groundObjects;
	static TouchableObject* objGround;
	//モデル
	static std::map<std::string, Model> groundModels;
	//別途地形用突起
	static std::vector<std::unique_ptr<Object3d>> terrainObjects;
	//モデル
	static std::map<std::string, Model> terrainModels;
	//コライダー用
	//オブジェクト
	static std::vector<std::unique_ptr<ColliderObject>> groundColliderObjects;
	//モデル
	static std::map<std::string, ColliderModel> groundColliderModels;

	//探索敵用
	//オブジェクト
	static std::vector<std::unique_ptr<Object3d>> enemyObjects;
	//モデル
	static std::map<std::string, Model> enemyModels;
	//コライダー用
	//オブジェクト
	static std::vector<std::unique_ptr<ColliderObject>> enemyColliderObjects;
	//モデル
	static std::map<std::string, ColliderModel> enemyColliderModels;
	//ナビエリア用
	//オブジェクト
	static std::vector<std::unique_ptr<ColliderObject>> enemyNaviareaObjects;
	//モデル
	static std::map<std::string, ColliderModel> enemyNaviareaModels;

	//襲撃敵
	//オブジェクト
	static std::vector<std::unique_ptr<Object3d>> raidEnemyObjects;
	//モデル
	static std::map<std::string, Model> raidEnemyModels;

	//壁のぼり用
	static std::vector<std::unique_ptr<Object3d>> climbWallObjects;
	static std::map<std::string, Model> climbWallModels;

	//ゴール
	static std::vector<std::unique_ptr<Object3d>> goalObjects;
	static std::map<std::string, Model> goalModels;

	static std::vector<std::unique_ptr<Object3d>> moveBoxObjects;
	static std::map<std::string, Model> moveBoxModels;

	//以下見た目用
	static std::vector<std::unique_ptr<Object3d>> rockObjects;
	static std::map<std::string, Model> rockModels;

	static std::vector<std::unique_ptr<Object3d>> sandGroundObjects;
	static std::map<std::string, Model> sandGroundModels;

	static bool hitTerrainDrawFlag;
public:

	//読み込み
	static void LoadFile(const std::string& fileName);

	//配置
	static void SetObject();

	//アップデート
	static void Update();

	//描画
	static void Draw();

	//終了
	static void Finalize();

private:
	//ブロック
	static void TypeSetModel( LevelData::ObjectData& objectData);
	static void TypeSetColliderModel( LevelData::ObjectData& colliderObjectData);
	//クリスタル
	static void TypeSetCrystalModel(LevelData::ObjectData& objectData);
	static void TypeSetColliderCrystalModel(LevelData::ObjectData& colliderObjectData);
	//地面
	static void TypeSetGroundModel(LevelData::ObjectData& objectData);
	static void TypeSetColliderGroundModel(LevelData::ObjectData& colliderObjectData);
	//地形用突起
	static void TypeSetTerrainModel(LevelData::ObjectData& objectData);
	//探索敵
	static void TypeSetEnemyModel(LevelData::ObjectData& objectData);
	static void TypeSetColliderEnemyModel(LevelData::ObjectData& colliderObjectData);
	static void TypeSetNaviareaEnemyModel(LevelData::ObjectData& colliderObjectData);
	//襲撃敵
	static void TypeSetRaidEnemyModel(LevelData::ObjectData& objectData);

	//よじ登れる壁
	static void TypeclimbWallModel(LevelData::ObjectData& objectData);

	//ゴール
	static void TypeGoalModel(LevelData::ObjectData& objectData);

	//箱
	static void TypeMoveBoxModel(LevelData::ObjectData& objectData);

	static void TypeRockModel(LevelData::ObjectData& objectData);

	static void TypeSandGroundModel(LevelData::ObjectData& objectData);
};

