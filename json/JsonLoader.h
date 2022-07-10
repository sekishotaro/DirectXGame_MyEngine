#pragma once
#include <string>
#include <DirectXMath.h>
#include <vector>

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

public:// 定数
	// デフォルトの読み込みディレクトリ
	static const std::string DefaultDirectory;
	// ファイル拡張子
	static const std::string JsonExtension;

public:

	//読み込み
	static void LoadFile(const std::string& fileName);

};

