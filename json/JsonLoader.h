#pragma once
#include <string>
#include <DirectXMath.h>
#include <vector>
#include "Object3d.h"
#include <memory>
#include <map>

// レベルデータ
struct LevelData {

	struct ObjectData {
		// ファイル名
		std::string fileName;
		// 平行移動
		DirectX::XMVECTOR translation;
		// 回転角
		DirectX::XMVECTOR rotation;
		// スケーリング
		DirectX::XMVECTOR scaling;
	};

	// オブジェクト配列
	std::vector<ObjectData> objects;
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

	//オブジェクト
	static std::vector<std::unique_ptr<Object3d>> objects;

	//モデル
	static std::map<std::string, Model> models;

public:

	//読み込み
	static void LoadFile(const std::string& fileName);

	//配置
	static void SetObject();

	static void Update();

	static void Draw();

};

