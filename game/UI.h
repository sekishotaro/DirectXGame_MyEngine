#pragma once
#include "Sprite.h"

class UI
{
public:
	// DirectX::Çè»ó™
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;

public:
	//èâä˙âª
	void Initialize();
	//çXêV
	void Update();
	//ï`âÊ
	void Draw(int crystalNum);

private:
	void timeUpdata();

private:
	Sprite* stamina[4] = { nullptr, nullptr, nullptr, nullptr };
	XMFLOAT2 staminabarSize;
	Sprite* crystalUI[26] = { nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };
	Sprite* timerCenter = nullptr;
	Sprite* timerPetal[9] = { nullptr,nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr};
	Sprite* controllerUI = nullptr;
};

