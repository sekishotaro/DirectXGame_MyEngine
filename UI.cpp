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

Sprite* UI::timerCenter = nullptr;
Sprite* UI::timerPetal0 = nullptr;
Sprite* UI::timerPetal1 = nullptr;
Sprite* UI::timerPetal2 = nullptr;
Sprite* UI::timerPetal3 = nullptr;
Sprite* UI::timerPetal4 = nullptr;
Sprite* UI::timerPetal5 = nullptr;
Sprite* UI::timerPetal6 = nullptr;
Sprite* UI::timerPetal7 = nullptr;
Sprite* UI::timerPetal8 = nullptr;

Sprite* UI::controllerUI = nullptr;

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

	Sprite::LoadTexture(30, L"Resources/UI/timer/timerCenter.png");
	Sprite::LoadTexture(31, L"Resources/UI/timer/timerPetal0.png");
	Sprite::LoadTexture(32, L"Resources/UI/timer/timerPetal1.png");
	Sprite::LoadTexture(33, L"Resources/UI/timer/timerPetal2.png");
	Sprite::LoadTexture(34, L"Resources/UI/timer/timerPetal3.png");
	Sprite::LoadTexture(35, L"Resources/UI/timer/timerPetal4.png");
	Sprite::LoadTexture(36, L"Resources/UI/timer/timerPetal5.png");

	Sprite::LoadTexture(40, L"Resources/UI/controller/controllerUI.png");
	Sprite::LoadTexture(41, L"Resources/UI/controller/controllerUI3.png");

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

	timerCenter = Sprite::Create(30, { 1174 , 83.0f });
	timerCenter->SetSize({ 32, 32 });

	timerPetal0 = Sprite::Create(31, { 1190 , 100.0f }, XMFLOAT4{ 1, 1, 1, 1 }, XMFLOAT2{ 0.5f, 1.0f });
	timerPetal0->SetSize({ 32, 64 });
	timerPetal0->SetRotation(0.0f);
	timerPetal1 = Sprite::Create(31, { 1190 , 100.0f }, XMFLOAT4{ 1, 1, 1, 1 }, XMFLOAT2{ 0.5f, 1.0f });
	timerPetal1->SetSize({ 32, 64 });
	timerPetal1->SetRotation(40.0f);
	timerPetal2 = Sprite::Create(31, { 1190 , 100.0f }, XMFLOAT4{ 1, 1, 1, 1 }, XMFLOAT2{ 0.5f, 1.0f });
	timerPetal2->SetSize({ 32, 64 });
	timerPetal2->SetRotation(80.0f);
	timerPetal3 = Sprite::Create(31, { 1190 , 100.0f }, XMFLOAT4{ 1, 1, 1, 1 }, XMFLOAT2{ 0.5f, 1.0f });
	timerPetal3->SetSize({ 32, 64 });
	timerPetal3->SetRotation(120.0f);
	timerPetal4 = Sprite::Create(31, { 1190 , 100.0f }, XMFLOAT4{ 1, 1, 1, 1 }, XMFLOAT2{ 0.5f, 1.0f });
	timerPetal4->SetSize({ 32, 64 });
	timerPetal4->SetRotation(160.0f);
	timerPetal5 = Sprite::Create(31, { 1190 , 100.0f }, XMFLOAT4{ 1, 1, 1, 1 }, XMFLOAT2{ 0.5f, 1.0f });
	timerPetal5->SetSize({ 32, 64 });
	timerPetal5->SetRotation(200.0f);
	timerPetal6 = Sprite::Create(31, { 1190 , 100.0f }, XMFLOAT4{ 1, 1, 1, 1 }, XMFLOAT2{ 0.5f, 1.0f });
	timerPetal6->SetSize({ 32, 64 });
	timerPetal6->SetRotation(240.0f);
	timerPetal7 = Sprite::Create(31, { 1190 , 100.0f }, XMFLOAT4{ 1, 1, 1, 1 }, XMFLOAT2{ 0.5f, 1.0f });
	timerPetal7->SetSize({ 32, 64 });
	timerPetal7->SetRotation(280.0f);
	timerPetal8 = Sprite::Create(31, { 1190 , 100.0f }, XMFLOAT4{ 1, 1, 1, 1 }, XMFLOAT2{ 0.5f, 1.0f });
	timerPetal8->SetSize({ 32, 64 });
	timerPetal8->SetRotation(320.0f);

	controllerUI = Sprite::Create(40, { 70.0f , 500.0f });
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

	timeUpdata();
	
	if (Player::GetWallHitFlag() == true)
	{
		controllerUI->SetTexNum(41);
	}
	else
	{
		controllerUI->SetTexNum(40);
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


	
	timerPetal0->Draw();
	timerPetal1->Draw();
	timerPetal2->Draw();
	timerPetal3->Draw();
	timerPetal4->Draw();
	timerPetal5->Draw();
	timerPetal6->Draw();
	timerPetal7->Draw();
	timerPetal8->Draw();
	timerCenter->Draw();

	controllerUI->Draw();
}

void UI::timeUpdata()
{
	
	//Todo:絶対時間が出来たら修正


	const int timeMax = (int)Player::GetTimeMax();
	const int time = (int)Player::GetTimeLimit();
	//float time = Player::GetTimeLimit();


	const int seconds_between = 2;

	if (time == timeMax)
	{
		timerPetal0->SetTexNum(31);
		timerPetal1->SetTexNum(31);
		timerPetal2->SetTexNum(31);
		timerPetal3->SetTexNum(31);
		timerPetal4->SetTexNum(31);
		timerPetal5->SetTexNum(31);
		timerPetal6->SetTexNum(31);
		timerPetal7->SetTexNum(31);
		timerPetal8->SetTexNum(31);
	}

	
	if (time <= timeMax * (0))
	{
		timerPetal8->SetTexNum(36);
	}
	else if (time <= (int)((float)timeMax * (1.0f / 54.0f)))
	{
		timerPetal8->SetTexNum(36);
	}
	else if (time <= (int)((float)timeMax * (2.0f / 54.0f)))
	{
		timerPetal8->SetTexNum(35);
	}
	else if (time <= (int)((float)timeMax * (3.0f / 54.0f)))
	{
		timerPetal8->SetTexNum(34);
	}
	else if (time <= (int)((float)timeMax * (4.0f / 54.0f)))
	{
		timerPetal8->SetTexNum(33);
	}
	else if (time <= (int)((float)timeMax * (5.0f / 54.0f)))
	{
		timerPetal8->SetTexNum(32);
	}
	else if (time <= (int)((float)timeMax * (6.0f / 54.0f)))
	{
		timerPetal8->SetTexNum(31);
	}
	else if (time <= (int)((float)timeMax * (7.0f / 54.0f)))
	{
		timerPetal7->SetTexNum(36);
	}
	else if (time <= (int)((float)timeMax * (8.0f / 54.0f)))
	{
		timerPetal7->SetTexNum(35);
	}
	else if (time <= (int)((float)timeMax * (9.0f / 54.0f)))
	{
		timerPetal7->SetTexNum(34);
	}
	else if (time <= (int)((float)timeMax * (10.0f / 54.0f)))
	{
		timerPetal7->SetTexNum(33);
	}
	else if (time <= (int)((float)timeMax * (11.0f / 54.0f)))
	{
		timerPetal7->SetTexNum(32);
	}
	else if (time <= (int)((float)timeMax * (12.0f / 54.0f)))
	{
		timerPetal7->SetTexNum(31);
	}
	else if (time <= (int)((float)timeMax * (13.0f / 54.0f)))
	{
		timerPetal6->SetTexNum(36);
	}
	else if (time <= (int)((float)timeMax * (14.0f / 54.0f)))
	{
		timerPetal6->SetTexNum(35);
	}
	else if (time <= (int)((float)timeMax * (15.0f / 54.0f)))
	{
		timerPetal6->SetTexNum(34);
	}
	else if (time <= (int)((float)timeMax * (16.0f / 54.0f)))
	{
		timerPetal6->SetTexNum(33);
	}
	else if (time <= (int)((float)timeMax * (17.0f / 54.0f)))
	{
		timerPetal6->SetTexNum(32);
	}
	else if (time <= (int)((float)timeMax * (18.0f / 54.0f)))
	{
		timerPetal6->SetTexNum(31);
	}
	else if (time <= (int)((float)timeMax * (19.0f / 54.0f)))
	{
		timerPetal5->SetTexNum(36);
	}
	else if (time <= (int)((float)timeMax * (20.0f / 54.0f)))
	{
		timerPetal5->SetTexNum(35);
	}
	else if (time <= (int)((float)timeMax * (21.0f / 54.0f)))
	{
		timerPetal5->SetTexNum(34);
	}
	else if (time <= (int)((float)timeMax * (22.0f / 54.0f)))
	{
		timerPetal5->SetTexNum(33);
	}
	else if (time <= (int)((float)timeMax * (23.0f / 54.0f)))
	{
		timerPetal5->SetTexNum(32);
	}
	else if (time <= (int)((float)timeMax * (24.0f / 54.0f)))
	{
		timerPetal5->SetTexNum(31);
	}
	else if (time <= (int)((float)timeMax * (25.0f / 54.0f)))
	{
		timerPetal4->SetTexNum(36);
	}
	else if (time <= (int)((float)timeMax * (26.0f / 54.0f)))
	{
		timerPetal4->SetTexNum(35);
	}
	else if (time <= (int)((float)timeMax * (27.0f / 54.0f)))
	{
		timerPetal4->SetTexNum(34);
	}
	else if (time <= (int)((float)timeMax * (28.0f / 54.0f)))
	{
		timerPetal4->SetTexNum(33);
	}
	else if (time <= (int)((float)timeMax * (29.0f / 54.0f)))
	{
		timerPetal4->SetTexNum(32);
	}
	else if (time <= (int)((float)timeMax * (30.0f / 54.0f)))
	{
		timerPetal4->SetTexNum(31);
	}
	else if (time <= (int)((float)timeMax * (31.0f / 54.0f)))
	{
		timerPetal3->SetTexNum(36);
	}
	else if (time <= (int)((float)timeMax * (32.0f / 54.0f)))
	{
		timerPetal3->SetTexNum(35);
	}
	else if (time <= (int)((float)timeMax * (33.0f / 54.0f)))
	{
		timerPetal3->SetTexNum(34);
	}
	else if (time <= (int)((float)timeMax * (34.0f / 54.0f)))
	{
		timerPetal3->SetTexNum(33);
	}
	else if (time <= (int)((float)timeMax * (35.0f / 54.0f)))
	{
		timerPetal3->SetTexNum(32);
	}
	else if (time <= (int)((float)timeMax * (36.0f / 54.0f)))
	{
		timerPetal3->SetTexNum(31);
	}
	else if (time <= (int)((float)timeMax * (37.0f / 54.0f)))
	{
		timerPetal2->SetTexNum(36);
	}
	else if (time <= (int)((float)timeMax * (38.0f / 54.0f)))
	{
		timerPetal2->SetTexNum(35);
	}
	else if (time <= (int)((float)timeMax * (39.0f / 54.0f)))
	{
		timerPetal2->SetTexNum(34);
	}
	else if (time <= (int)((float)timeMax * (40.0f / 54.0f)))
	{
		timerPetal2->SetTexNum(33);
	}
	else if (time <= (int)((float)timeMax * (41.0f / 54.0f)))
	{
		timerPetal2->SetTexNum(32);
	}
	else if (time <= (int)((float)timeMax * (42.0f / 54.0f)))
	{
		timerPetal2->SetTexNum(31);
	}
	else if (time <= (int)((float)timeMax * (43.0f / 54.0f)))
	{
		timerPetal1->SetTexNum(36);
	}
	else if (time <= (int)((float)timeMax * (44.0f / 54.0f)))
	{
		timerPetal1->SetTexNum(35);
	}
	else if (time <= (int)((float)timeMax * (45.0f / 54.0f)))
	{
		timerPetal1->SetTexNum(34);
	}
	else if (time <= (int)((float)timeMax * (46.0f / 54.0f)))
	{
		timerPetal1->SetTexNum(33);
	}
	else if (time <= (int)((float)timeMax * (47.0f / 54.0f)))
	{
		timerPetal1->SetTexNum(32);
	}
	else if (time <= (int)((float)timeMax * (48.0f / 54.0f)))
	{
		timerPetal1->SetTexNum(31);
	}
	else if (time <= (int)((float)timeMax * (49.0f / 54.0f)))
	{
		timerPetal0->SetTexNum(36);
	}
	else if (time <= (int)((float)timeMax * (50.0f / 54.0f)))
	{
		timerPetal0->SetTexNum(35);
	}
	else if (time <= (int)((float)timeMax * (51.0f / 54.0f)))
	{
		timerPetal0->SetTexNum(34);
	}
	else if (time <= (int)((float)timeMax * (52.0f / 54.0f)))
	{
		timerPetal0->SetTexNum(33);
	}
	else if (time <= (int)((float)timeMax * (53.0f / 54.0f)))
	{
		timerPetal0->SetTexNum(32);
	}
	else if (time <= (int)((float)timeMax * (54.0f / 54.0f)))
	{
		timerPetal0->SetTexNum(31);
	}

	//切り替えタイミングでないならそのまま返す
	/*if (time % seconds_between != 0) return;

	if (time == 108)
	{
		timerPetal0->SetTexNum(31);
	}
	else if (time == 106)
	{
		timerPetal0->SetTexNum(32);
	}
	else if (time == 104)
	{
		timerPetal0->SetTexNum(33);
	}
	else if (time == 102)
	{
		timerPetal0->SetTexNum(34);
	}
	else if (time == 100)
	{
		timerPetal0->SetTexNum(35);
	}
	else if (time == 98)
	{
		timerPetal0->SetTexNum(36);
	}

	if (time == 96)
	{
		timerPetal1->SetTexNum(31);
	}
	else if (time == 94)
	{
		timerPetal1->SetTexNum(32);
	}
	else if (time == 92)
	{
		timerPetal1->SetTexNum(33);
	}
	else if (time == 90)
	{
		timerPetal1->SetTexNum(34);
	}
	else if (time == 88)
	{
		timerPetal1->SetTexNum(35);
	}
	else if (time == 86)
	{
		timerPetal1->SetTexNum(36);
	}

	if (time == 84)
	{
		timerPetal2->SetTexNum(31);
	}
	else if (time == 82)
	{
		timerPetal2->SetTexNum(32);
	}
	else if (time == 80)
	{
		timerPetal2->SetTexNum(33);
	}
	else if (time == 78)
	{
		timerPetal2->SetTexNum(34);
	}
	else if (time == 76)
	{
		timerPetal2->SetTexNum(35);
	}
	else if (time == 74)
	{
		timerPetal2->SetTexNum(36);
	}

	if (time == 72)
	{
		timerPetal3->SetTexNum(31);
	}
	else if (time == 70)
	{
		timerPetal3->SetTexNum(32);
	}
	else if (time == 68)
	{
		timerPetal3->SetTexNum(33);
	}
	else if (time == 66)
	{
		timerPetal3->SetTexNum(34);
	}
	else if (time == 64)
	{
		timerPetal3->SetTexNum(35);
	}
	else if (time == 62)
	{
		timerPetal3->SetTexNum(36);
	}

	if (time == 60)
	{
		timerPetal4->SetTexNum(31);
	}
	else if (time == 58)
	{
		timerPetal4->SetTexNum(32);
	}
	else if (time == 56)
	{
		timerPetal4->SetTexNum(33);
	}
	else if (time == 54)
	{
		timerPetal4->SetTexNum(34);
	}
	else if (time == 52)
	{
		timerPetal4->SetTexNum(35);
	}
	else if (time == 50)
	{
		timerPetal4->SetTexNum(36);
	}

	if (time == 48)
	{
		timerPetal5->SetTexNum(31);
	}
	else if (time == 46)
	{
		timerPetal5->SetTexNum(32);
	}
	else if (time == 44)
	{
		timerPetal5->SetTexNum(33);
	}
	else if (time == 42)
	{
		timerPetal5->SetTexNum(34);
	}
	else if (time == 40)
	{
		timerPetal5->SetTexNum(35);
	}
	else if (time == 38)
	{
		timerPetal5->SetTexNum(36);
	}

	if (time == 36)
	{
		timerPetal6->SetTexNum(31);
	}
	else if (time == 34)
	{
		timerPetal6->SetTexNum(32);
	}
	else if (time == 32)
	{
		timerPetal6->SetTexNum(33);
	}
	else if (time == 30)
	{
		timerPetal6->SetTexNum(34);
	}
	else if (time == 28)
	{
		timerPetal6->SetTexNum(35);
	}
	else if (time == 26)
	{
		timerPetal6->SetTexNum(36);
	}

	if (time == 24)
	{
		timerPetal7->SetTexNum(31);
	}
	else if (time == 22)
	{
		timerPetal7->SetTexNum(32);
	}
	else if (time == 20)
	{
		timerPetal7->SetTexNum(33);
	}
	else if (time == 18)
	{
		timerPetal7->SetTexNum(34);
	}
	else if (time == 16)
	{
		timerPetal7->SetTexNum(35);
	}
	else if (time == 14)
	{
		timerPetal7->SetTexNum(36);
	}

	if (time == 12)
	{
		timerPetal8->SetTexNum(31);
	}
	else if (time == 10)
	{
		timerPetal8->SetTexNum(32);
	}
	else if (time == 8)
	{
		timerPetal8->SetTexNum(33);
	}
	else if (time == 6)
	{
		timerPetal8->SetTexNum(34);
	}
	else if (time == 4)
	{
		timerPetal8->SetTexNum(35);
	}
	else if (time == 2)
	{
		timerPetal8->SetTexNum(36);
	}*/
}