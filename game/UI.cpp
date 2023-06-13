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
Sprite* UI::crystalUI9 = nullptr;
Sprite* UI::crystalUI10 = nullptr;
Sprite* UI::crystalUI11 = nullptr;
Sprite* UI::crystalUI12 = nullptr;
Sprite* UI::crystalUI13 = nullptr;
Sprite* UI::crystalUI14 = nullptr;
Sprite* UI::crystalUI15 = nullptr;
Sprite* UI::crystalUI16 = nullptr;
Sprite* UI::crystalUI17 = nullptr;
Sprite* UI::crystalUI18 = nullptr;
Sprite* UI::crystalUI19 = nullptr;
Sprite* UI::crystalUI20 = nullptr;
Sprite* UI::crystalUI21 = nullptr;
Sprite* UI::crystalUI22 = nullptr;
Sprite* UI::crystalUI23 = nullptr;
Sprite* UI::crystalUI24 = nullptr;
Sprite* UI::crystalUI25 = nullptr;


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
	Sprite::LoadTexture(29, L"Resources/UI/CrystalUI/Crystal_9.png");
	Sprite::LoadTexture(40, L"Resources/UI/CrystalUI/Crystal_10.png");
	Sprite::LoadTexture(41, L"Resources/UI/CrystalUI/Crystal_11.png");
	Sprite::LoadTexture(42, L"Resources/UI/CrystalUI/Crystal_12.png");
	Sprite::LoadTexture(43, L"Resources/UI/CrystalUI/Crystal_13.png");
	Sprite::LoadTexture(44, L"Resources/UI/CrystalUI/Crystal_14.png");
	Sprite::LoadTexture(45, L"Resources/UI/CrystalUI/Crystal_15.png");
	Sprite::LoadTexture(46, L"Resources/UI/CrystalUI/Crystal_16.png");
	Sprite::LoadTexture(47, L"Resources/UI/CrystalUI/Crystal_17.png");
	Sprite::LoadTexture(48, L"Resources/UI/CrystalUI/Crystal_18.png");
	Sprite::LoadTexture(49, L"Resources/UI/CrystalUI/Crystal_19.png");
	Sprite::LoadTexture(50, L"Resources/UI/CrystalUI/Crystal_20.png");
	Sprite::LoadTexture(51, L"Resources/UI/CrystalUI/Crystal_21.png");
	Sprite::LoadTexture(52, L"Resources/UI/CrystalUI/Crystal_22.png");
	Sprite::LoadTexture(53, L"Resources/UI/CrystalUI/Crystal_23.png");
	Sprite::LoadTexture(54, L"Resources/UI/CrystalUI/Crystal_24.png");
	Sprite::LoadTexture(55, L"Resources/UI/CrystalUI/Crystal_25.png");

	Sprite::LoadTexture(30, L"Resources/UI/timer/timerCenter.png");
	
	Sprite::LoadTexture(31, L"Resources/UI/timer/timerPetal0.png");
	Sprite::LoadTexture(32, L"Resources/UI/timer/timerPetal1.png");
	Sprite::LoadTexture(33, L"Resources/UI/timer/timerPetal2.png");
	Sprite::LoadTexture(34, L"Resources/UI/timer/timerPetal3.png");
	Sprite::LoadTexture(35, L"Resources/UI/timer/timerPetal4.png");
	Sprite::LoadTexture(36, L"Resources/UI/timer/timerPetal5.png");

	Sprite::LoadTexture(60, L"Resources/UI/controller/controllerUI.png");
	Sprite::LoadTexture(61, L"Resources/UI/controller/controllerUI2.png");
	Sprite::LoadTexture(62, L"Resources/UI/controller/controllerUI3.png");
	Sprite::LoadTexture(63, L"Resources/UI/controller/controllerUI4.png");

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
	crystalUI9 = Sprite::Create(29, { 1080.0f , 90.0f });
	crystalUI9->SetSize({ 200, 500 });
	crystalUI10 = Sprite::Create(40, { 1080.0f , 90.0f });
	crystalUI10->SetSize({ 200, 500 });
	crystalUI11 = Sprite::Create(41, { 1080.0f , 90.0f });
	crystalUI11->SetSize({ 200, 500 });
	crystalUI12 = Sprite::Create(42, { 1080.0f , 90.0f });
	crystalUI12->SetSize({ 200, 500 });
	crystalUI13 = Sprite::Create(43, { 1080.0f , 90.0f });
	crystalUI13->SetSize({ 200, 500 });
	crystalUI14 = Sprite::Create(44, { 1080.0f , 90.0f });
	crystalUI14->SetSize({ 200, 500 });
	crystalUI15 = Sprite::Create(45, { 1080.0f , 90.0f });
	crystalUI15->SetSize({ 200, 500 });
	crystalUI16 = Sprite::Create(46, { 1080.0f , 90.0f });
	crystalUI16->SetSize({ 200, 500 });
	crystalUI17 = Sprite::Create(47, { 1080.0f , 90.0f });
	crystalUI17->SetSize({ 200, 500 });
	crystalUI18 = Sprite::Create(48, { 1080.0f , 90.0f });
	crystalUI18->SetSize({ 200, 500 });
	crystalUI19 = Sprite::Create(49, { 1080.0f , 90.0f });
	crystalUI19->SetSize({ 200, 500 });
	crystalUI20 = Sprite::Create(50, { 1080.0f , 90.0f });
	crystalUI20->SetSize({ 200, 500 });
	crystalUI21 = Sprite::Create(51, { 1080.0f , 90.0f });
	crystalUI21->SetSize({ 200, 500 });
	crystalUI22 = Sprite::Create(52, { 1080.0f , 90.0f });
	crystalUI22->SetSize({ 200, 500 });
	crystalUI23 = Sprite::Create(53, { 1080.0f , 90.0f });
	crystalUI23->SetSize({ 200, 500 });
	crystalUI24 = Sprite::Create(54, { 1080.0f , 90.0f });
	crystalUI24->SetSize({ 200, 500 });
	crystalUI25 = Sprite::Create(55, { 1080.0f , 90.0f });
	crystalUI25->SetSize({ 200, 500 });

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

	controllerUI = Sprite::Create(40, { 0.0f , 400.0f });
	controllerUI->SetSize({ 400.0f, 400.0f });
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
			stamina1->SetColor({ 1.0f, 0.1f, 0.1f, 1.0f });
		}
	}
	else
	{
		stamina1->SetColor({ 1.0f, 0.1f, 0.1f, 1.0f });
	}

	timeUpdata();
	
	if (Player::GetStatus() == 6)
	{
		controllerUI->SetTexNum(62);
	}
	else if (Player::GetMoveBoxHitFlag())
	{
		controllerUI->SetTexNum(63);
	}
	else if (Player::GetClimbingCliffFlag())
	{
		controllerUI->SetTexNum(61);
	}
	else
	{
		controllerUI->SetTexNum(60);
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
		crystalUI25->Draw();
	}
	else if (Player::GetCrystal() == 1)
	{
		crystalUI24->Draw();
	}
	else if (Player::GetCrystal() == 2)
	{
		crystalUI23->Draw();
	}
	else if (Player::GetCrystal() == 3)
	{
		crystalUI22->Draw();
	}
	else if (Player::GetCrystal() == 4)
	{
		crystalUI21->Draw();
	}
	else if (Player::GetCrystal() == 5)
	{
		crystalUI20->Draw();
	}
	else if (Player::GetCrystal() == 6)
	{
		crystalUI19->Draw();
	}
	else if (Player::GetCrystal() == 7)
	{
		crystalUI18->Draw();
	}
	else if (Player::GetCrystal() == 8)
	{
		crystalUI17->Draw();
	}
	else if (Player::GetCrystal() == 9)
	{
		crystalUI16->Draw();
	}
	else if (Player::GetCrystal() == 10)
	{
		crystalUI15->Draw();
	}
	else if (Player::GetCrystal() == 11)
	{
		crystalUI14->Draw();
	}
	else if (Player::GetCrystal() == 12)
	{
		crystalUI13->Draw();
	}
	else if (Player::GetCrystal() == 13)
	{
		crystalUI12->Draw();
	}
	else if (Player::GetCrystal() == 14)
	{
		crystalUI11->Draw();
	}
	else if (Player::GetCrystal() == 15)
	{
		crystalUI10->Draw();
	}
	else if (Player::GetCrystal() == 16)
	{
		crystalUI9->Draw();
	}
	else if (Player::GetCrystal() == 17)
	{
		crystalUI8->Draw();
	}
	else if (Player::GetCrystal() == 18)
	{
		crystalUI7->Draw();
	}
	else if (Player::GetCrystal() == 19)
	{
		crystalUI6->Draw();
	}
	else if (Player::GetCrystal() == 20)
	{
		crystalUI5->Draw();
	}
	else if (Player::GetCrystal() == 21)
	{
		crystalUI4->Draw();
	}
	else if (Player::GetCrystal() == 22)
	{
		crystalUI3->Draw();
	}
	else if (Player::GetCrystal() == 23)
	{
		crystalUI2->Draw();
	}
	else if (Player::GetCrystal() == 24)
	{
		crystalUI1->Draw();
	}
	else if (Player::GetCrystal() == 25)
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
	
	//Todo:â‘ÎŽžŠÔ‚ªo—ˆ‚½‚çC³


	const int timeMax = (int)Player::GetTimeMax();
	const int time = (int)Player::GetTimeLimit();
	//float time = Player::GetTimeLimit();


	const int seconds_between = 2;

	if (Player::GetCrystalGetFlag() || time == timeMax)
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

	
	if (time <= timeMax * (53.0f / 53.0f))
	{
		timerPetal0->SetTexNum(31);
	}
	if (time <= timeMax * (52.0f / 53.0f))
	{
		timerPetal0->SetTexNum(32);
	}
	if (time <= timeMax * (51.0f / 53.0f))
	{
		timerPetal0->SetTexNum(33);
	}
	if (time <= timeMax * (50.0f / 53.0f))
	{
		timerPetal0->SetTexNum(34);
	}
	if (time <= timeMax * (49.0f / 53.0f))
	{
		timerPetal0->SetTexNum(35);
	}
	if (time <= timeMax * (48.0f / 53.0f))
	{
		timerPetal0->SetTexNum(36);
	}
	
	if (time <= timeMax * (47.0f / 53.0f))
	{
		timerPetal1->SetTexNum(31);
	}
	if (time <= timeMax * (46.0f / 53.0f))
	{
		timerPetal1->SetTexNum(32);
	}
	if (time <= timeMax * (45.0f / 53.0f))
	{
		timerPetal1->SetTexNum(33);
	}
	if (time <= timeMax * (44.0f / 53.0f))
	{
		timerPetal1->SetTexNum(34);
	}
	if (time <= timeMax * (43.0f / 53.0f))
	{
		timerPetal1->SetTexNum(35);
	}
	if (time <= timeMax * (42.0f / 53.0f))
	{
		timerPetal1->SetTexNum(36);
	}
	
	if (time <= timeMax * (41.0f / 53.0f))
	{
		timerPetal2->SetTexNum(31);
	}
	if (time <= timeMax * (40.0f / 53.0f))
	{
		timerPetal2->SetTexNum(32);
	}
	if (time <= timeMax * (39.0f / 53.0f))
	{
		timerPetal2->SetTexNum(33);
	}
	if (time <= timeMax * (38.0f / 53.0f))
	{
		timerPetal2->SetTexNum(34);
	}
	if (time <= timeMax * (37.0f / 53.0f))
	{
		timerPetal2->SetTexNum(35);
	}
	if (time <= timeMax * (36.0f / 53.0f))
	{
		timerPetal2->SetTexNum(36);
	}
	
	if (time <= timeMax * (35.0f / 53.0f))
	{
		timerPetal3->SetTexNum(31);
	}
	if (time <= timeMax * (34.0f / 53.0f))
	{
		timerPetal3->SetTexNum(32);
	}
	if (time <= timeMax * (33.0f / 53.0f))
	{
		timerPetal3->SetTexNum(33);
	}
	if (time <= timeMax * (32.0f / 53.0f))
	{
		timerPetal3->SetTexNum(34);
	}
	if (time <= timeMax * (31.0f / 53.0f))
	{
		timerPetal3->SetTexNum(35);
	}
	if (time <= timeMax * (30.0f / 53.0f))
	{
		timerPetal3->SetTexNum(36);
	}
	
	if (time <= timeMax * (29.0f / 53.0f))
	{
		timerPetal4->SetTexNum(31);
	}
	if (time <= timeMax * (28.0f / 53.0f))
	{
		timerPetal4->SetTexNum(32);
	}
	if (time <= timeMax * (27.0f / 53.0f))
	{
		timerPetal4->SetTexNum(33);
	}
	if (time <= timeMax * (26.0f / 53.0f))
	{
		timerPetal4->SetTexNum(34);
	}
	if (time <= timeMax * (25.0f / 53.0f))
	{
		timerPetal4->SetTexNum(35);
	}
	if (time <= timeMax * (24.0f / 53.0f))
	{
		timerPetal4->SetTexNum(36);
	}
	
	if (time <= timeMax * (23.0f / 53.0f))
	{
		timerPetal5->SetTexNum(31);
	}
	if (time <= timeMax * (22.0f / 53.0f))
	{
		timerPetal5->SetTexNum(32);
	}
	if (time <= timeMax * (21.0f / 53.0f))
	{
		timerPetal5->SetTexNum(33);
	}
	if (time <= timeMax * (20.0f / 53.0f))
	{
		timerPetal5->SetTexNum(34);
	}
	if (time <= timeMax * (19.0f / 53.0f))
	{
		timerPetal5->SetTexNum(35);
	}
	if (time <= timeMax * (18.0f / 53.0f))
	{
		timerPetal5->SetTexNum(36);
	}
	
	if (time <= timeMax * (17.0f / 53.0f))
	{
		timerPetal6->SetTexNum(31);
	}
	if (time <= timeMax * (16.0f / 53.0f))
	{
		timerPetal6->SetTexNum(32);
	}
	if (time <= timeMax * (15.0f / 53.0f))
	{
		timerPetal6->SetTexNum(33);
	}
	if (time <= timeMax * (14.0f / 53.0f))
	{
		timerPetal6->SetTexNum(34);
	}
	if (time <= timeMax * (13.0f / 53.0f))
	{
		timerPetal6->SetTexNum(35);
	}
	if (time <= timeMax * (12.0f / 53.0f))
	{
		timerPetal6->SetTexNum(36);
	}
	
	if (time <= timeMax * (11.0f / 53.0f))
	{
		timerPetal7->SetTexNum(31);
	}
	if (time <= timeMax * (10.0f / 53.0f))
	{
		timerPetal7->SetTexNum(32);
	}
	if (time <= timeMax * (9.0f / 53.0f))
	{
		timerPetal7->SetTexNum(33);
	}
	if (time <= timeMax * (8.0f / 53.0f))
	{
		timerPetal7->SetTexNum(34);
	}
	if (time <= timeMax * (7.0f / 53.0f))
	{
		timerPetal7->SetTexNum(35);
	}	
	if (time <= timeMax * (6.0f / 53.0f))
	{
		timerPetal7->SetTexNum(36);
	}

	if (time <= timeMax * (5.0f / 53.0f))
	{
		timerPetal8->SetTexNum(31);
	}
	if (time <= timeMax * (4.0f / 53.0f))
	{
		timerPetal8->SetTexNum(32);
	}
	if (time <= timeMax * (3.0f / 53.0f))
	{
		timerPetal8->SetTexNum(33);
	}
	if (time <= timeMax * (2.0f / 53.0f))
	{
		timerPetal8->SetTexNum(34);
	}
	if (time <= timeMax * (1.0f / 53.0f))
	{
		timerPetal8->SetTexNum(35);
	}
	if (time <= timeMax * (0.0f / 53.0f))
	{
		timerPetal8->SetTexNum(36);
	}
	
}