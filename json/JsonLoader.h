#pragma once
#include <string>

class JsonLoader
{

public:// �萔
	// �f�t�H���g�̓ǂݍ��݃f�B���N�g��
	static const std::string DefaultDirectory;
	// �t�@�C���g���q
	static const std::string JsonExtension;

public:

	//�ǂݍ���
	static void LoadFile(const std::string& fileName);

};

