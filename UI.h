#pragma once
#include "Sprite.h"

class UI
{
public:
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;

public:
	//������
	static void Initialize();
	//�X�V
	static void Update();
	//�`��
	static void Draw();

private:
	static Sprite* stamina0;
	static Sprite* stamina1;
	static XMFLOAT2 staminabarSize;
	static Sprite* stamina2;
	static Sprite* stamina3;

	static Sprite* crystalUI0;
	static Sprite* crystalUI1;
	static Sprite* crystalUI2;
	static Sprite* crystalUI3;
	static Sprite* crystalUI4;
	static Sprite* crystalUI5;
	static Sprite* crystalUI6;
	static Sprite* crystalUI7;
	static Sprite* crystalUI8;
};

