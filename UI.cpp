#include "UI.h"
#include "Player.h"

Sprite* UI::stamina0 = nullptr;
Sprite* UI::stamina1 = nullptr;
UI::XMFLOAT2 UI::staminabarSize;
Sprite* UI::stamina2 = nullptr;
Sprite* UI::stamina3 = nullptr;

Sprite* UI::crystalUI0 = nullptr;
Sprite* UI::crystalUI1 = nullptr;
Sprite* UI::crystalUI2 = nullptr;
Sprite* UI::crystalUI3 = nullptr;
Sprite* UI::crystalUI4 = nullptr;
Sprite* UI::crystalUI5 = nullptr;
Sprite* UI::crystalUI6 = nullptr;
Sprite* UI::crystalUI7 = nullptr;
Sprite* UI::crystalUI8 = nullptr;

void UI::Initialize()
{
	Sprite::LoadTexture(2, L"Resources/staminaBar/staminaBar_0.png");
	Sprite::LoadTexture(3, L"Resources/staminaBar/staminaBar_1.png");
	Sprite::LoadTexture(4, L"Resources/staminaBar/staminaBar_2.png");
	Sprite::LoadTexture(5, L"Resources/staminaBar/staminaBar_3.png");
	
	Sprite::LoadTexture(20, L"Resources/UI/CrystalUI/Crystal_Null.png");
	Sprite::LoadTexture(21, L"Resources/UI/CrystalUI/Crystal_1.png");
	Sprite::LoadTexture(22, L"Resources/UI/CrystalUI/Crystal_2.png");
	Sprite::LoadTexture(23, L"Resources/UI/CrystalUI/Crystal_3.png");
	Sprite::LoadTexture(24, L"Resources/UI/CrystalUI/Crystal_4.png");
	Sprite::LoadTexture(25, L"Resources/UI/CrystalUI/Crystal_5.png");
	Sprite::LoadTexture(26, L"Resources/UI/CrystalUI/Crystal_6.png");
	Sprite::LoadTexture(27, L"Resources/UI/CrystalUI/Crystal_7.png");
	Sprite::LoadTexture(28, L"Resources/UI/CrystalUI/Crystal_8.png");

	stamina0 = Sprite::Create(2, { 800.0f,350.0f });
	stamina0->SetAnchorPoint({ 1.0f, 1.0f });
	stamina1 = Sprite::Create(3, { 800.0f,350.0f });
	stamina1->SetAnchorPoint({ 1.0f, 1.0f });
	staminabarSize = stamina1->GetSize();
	stamina2 = Sprite::Create(4, { 800.0f,350.0f });
	stamina2->SetAnchorPoint({ 1.0f, 1.0f });
	stamina3 = Sprite::Create(5, { 800.0f,350.0f });
	stamina3->SetAnchorPoint({ 1.0f, 1.0f });

	crystalUI0 = Sprite::Create(20, { 1080.0f , 90.0f });
	crystalUI0->SetSize({ 200, 500 });
	crystalUI1 = Sprite::Create(21, { 1080.0f , 90.0f });
	crystalUI1->SetSize({ 200, 500 });
	crystalUI2 = Sprite::Create(22, { 1080.0f , 90.0f });
	crystalUI2->SetSize({ 200, 500 });
	crystalUI3 = Sprite::Create(23, { 1080.0f , 90.0f });
	crystalUI3->SetSize({ 200, 500 });
	crystalUI4 = Sprite::Create(24, { 1080.0f , 90.0f });
	crystalUI4->SetSize({ 200, 500 });
	crystalUI5 = Sprite::Create(25, { 1080.0f , 90.0f });
	crystalUI5->SetSize({ 200, 500 });
	crystalUI6 = Sprite::Create(26, { 1080.0f , 90.0f });
	crystalUI6->SetSize({ 200, 500 });
	crystalUI7 = Sprite::Create(27, { 1080.0f , 90.0f });
	crystalUI7->SetSize({ 200, 500 });
	crystalUI8 = Sprite::Create(28, { 1080.0f , 90.0f });
	crystalUI8->SetSize({ 200, 500 });
}

void UI::Update()
{
	float staminaSizeY = staminabarSize.y * Player::GetStaminaQuantity() / 100.0f;
	stamina1->SetSize({ staminabarSize.x, staminaSizeY });

	if (Player::GetStaminaCut() != true)
	{
		if (Player::GetStaminaQuantity() >= 30.0f)
		{
			stamina1->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
		}
		else
		{
			stamina1->SetColor({ 1.0f, 0.5f, 0.5f, 1.0f });
		}
	}
	else
	{
		stamina1->SetColor({ 1.0f, 0.5f, 0.5f, 1.0f });
	}
}

void UI::Draw()
{
	if (Player::GetStaminaFlag() == true || Player::GetStaminaQuantity() != 100.0f)
	{
		stamina0->Draw();
		stamina1->Draw();
		stamina2->Draw();
		stamina3->Draw();
	}

	if (Player::GetCrystal() == 0)
	{
		crystalUI8->Draw();
	}
	else if (Player::GetCrystal() == 1)
	{
		crystalUI7->Draw();
	}
	else if (Player::GetCrystal() == 2)
	{
		crystalUI6->Draw();
	}
	else if (Player::GetCrystal() == 3)
	{
		crystalUI5->Draw();
	}
	else if (Player::GetCrystal() == 4)
	{
		crystalUI4->Draw();
	}
	else if (Player::GetCrystal() == 5)
	{
		crystalUI3->Draw();
	}
	else if (Player::GetCrystal() == 6)
	{
		crystalUI2->Draw();
	}
	else if (Player::GetCrystal() == 7)
	{
		crystalUI1->Draw();
	}
	else if (Player::GetCrystal() == 8)
	{
		crystalUI0->Draw();
	}



}
