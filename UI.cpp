#include "UI.h"
#include "Player.h"

Sprite* UI::stamina0 = nullptr;
Sprite* UI::stamina1 = nullptr;
UI::XMFLOAT2 UI::staminabarSize;
Sprite* UI::stamina2 = nullptr;
Sprite* UI::stamina3 = nullptr;

void UI::Initialize()
{
	Sprite::LoadTexture(2, L"Resources/staminaBar/staminaBar_0.png");
	Sprite::LoadTexture(3, L"Resources/staminaBar/staminaBar_1.png");
	Sprite::LoadTexture(4, L"Resources/staminaBar/staminaBar_2.png");
	Sprite::LoadTexture(5, L"Resources/staminaBar/staminaBar_3.png");

	stamina0 = Sprite::Create(2, { 800.0f,350.0f });
	stamina0->SetAnchorPoint({ 1.0f, 1.0f });
	stamina1 = Sprite::Create(3, { 800.0f,350.0f });
	stamina1->SetAnchorPoint({ 1.0f, 1.0f });
	staminabarSize = stamina1->GetSize();
	stamina2 = Sprite::Create(4, { 800.0f,350.0f });
	stamina2->SetAnchorPoint({ 1.0f, 1.0f });
	stamina3 = Sprite::Create(5, { 800.0f,350.0f });
	stamina3->SetAnchorPoint({ 1.0f, 1.0f });
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
}
