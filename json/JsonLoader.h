#pragma once
#include <string>
#include <DirectXMath.h>
#include <vector>

// ���x���f�[�^
struct LevelData {

	struct ObjectData {
		// �t�@�C����
		std::string fileName;
		// ���s�ړ�
		DirectX::XMVECTOR translation;
		// ��]�p
		DirectX::XMVECTOR rotation;
		// �X�P�[�����O
		DirectX::XMVECTOR scaling;
	};

	// �I�u�W�F�N�g�z��
	std::vector<ObjectData> objects;
};

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

