#pragma once
#include <string>
#include <DirectXMath.h>
#include <vector>
#include "Object3d.h"
#include "ColliderObject.h"
#include <memory>
#include <map>

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
	};

	// 描画用オブジェクト配列
	std::vector<ObjectData> objects;

	// 当たり判定用オブジェクト配列
	std::vector<ObjectData> colliderObjects;
};

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

public:

	//読み込み
	static void LoadFile(const std::string& fileName);

	//配置
	static void SetObject();

	//アップデート
	static void Update();

	//描画
	static void Draw();

private:
	//static void WallLoad( nlohmann::json& object);

};

