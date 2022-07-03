#include <fstream>
#include <cassert>
#include <json.hpp>
#include "JsonLoader.h"



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
	std::string name = 
		deserialized["name"].get<std::string>();
	// 正しいレベルデータファイルかチェック
	assert(name.compare("scene") == 0);
}
