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
	static void Draw(int crystalNum);

private:
	static void timeUpdata();

private:
	static Sprite* stamina[4];
	static XMFLOAT2 staminabarSize;

	static Sprite* crystalUI[26];

	static Sprite* timerCenter;
	static Sprite* timerPetal[9];

	static Sprite* controllerUI;
};

