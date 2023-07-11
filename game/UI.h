#pragma once
#include "Sprite.h"
#include "Player.h"

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
	virtual void Initialize() = 0;
	//çXêV
	virtual void Update(Player* player) = 0;
	//ï`âÊ
	virtual void Draw(Player* player) = 0;
};

class StaminaUI : public UI
{
public:
	void Initialize() override;
	void Update(Player* player);
	void Draw(Player* player);
public:
	Sprite* stamina[4] = { nullptr, nullptr, nullptr, nullptr };
	XMFLOAT2 staminabarSize = {};
};

class TimeUI : public UI
{
public:
	void Initialize() override;
	void Update(Player* player);
	void Draw(Player* player);
private:
	const int timePetalNum = 9;
	const int timePetalPattern = 6;
public:
	Sprite* timerCenter = nullptr;
	Sprite* timerPetal[9] = { nullptr,nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };
};

class ControllerUI : public UI
{
public:
	void Initialize() override;
	void Update(Player* player);
	void Draw(Player* player);
public:
	Sprite* controllerUI = nullptr;
};

class CrystalUI : public UI
{
public:
	void Initialize() override;
	void Update(Player* player);
	void Draw(Player* player);
public:
	Sprite* crystalUI[26] = { nullptr, nullptr, nullptr, nullptr, nullptr,
		nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
		nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };
};
