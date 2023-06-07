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
	static void Initialize();
	//çXêV
	static void Update();
	//ï`âÊ
	static void Draw();

private:
	static void timeUpdata();

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
	static Sprite* crystalUI9;
	static Sprite* crystalUI10;
	static Sprite* crystalUI11;
	static Sprite* crystalUI12;
	static Sprite* crystalUI13;
	static Sprite* crystalUI14;
	static Sprite* crystalUI15;
	static Sprite* crystalUI16;
	static Sprite* crystalUI17;
	static Sprite* crystalUI18;
	static Sprite* crystalUI19;
	static Sprite* crystalUI20;
	static Sprite* crystalUI21;
	static Sprite* crystalUI22;
	static Sprite* crystalUI23;
	static Sprite* crystalUI24;
	static Sprite* crystalUI25;


	static Sprite* timerCenter;
	static Sprite* timerPetal0;
	static Sprite* timerPetal1;
	static Sprite* timerPetal2;
	static Sprite* timerPetal3;
	static Sprite* timerPetal4;
	static Sprite* timerPetal5;
	static Sprite* timerPetal6;
	static Sprite* timerPetal7;
	static Sprite* timerPetal8;


	static Sprite* controllerUI;
};

