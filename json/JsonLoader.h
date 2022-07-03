#pragma once
#include <string>

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

