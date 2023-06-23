#include "UI.h"
#include "Player.h"

UI::XMFLOAT2 UI::staminabarSize;
Sprite* UI::stamina[] = { nullptr, nullptr, nullptr, nullptr };

Sprite* UI::crystalUI[] = { nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };

Sprite* UI::timerCenter = nullptr;
Sprite* UI::timerPetal[] = { nullptr,nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, };

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

	stamina[0] = Sprite::Create(2, { 800.0f,350.0f });
	stamina[0]->SetAnchorPoint({ 1.0f, 1.0f });
	stamina[1] = Sprite::Create(3, { 800.0f,350.0f });
	stamina[1]->SetAnchorPoint({ 1.0f, 1.0f });
	staminabarSize = stamina[1]->GetSize();
	stamina[2] = Sprite::Create(4, { 800.0f,350.0f });
	stamina[2]->SetAnchorPoint({ 1.0f, 1.0f });
	stamina[3] = Sprite::Create(5, { 800.0f,350.0f });
	stamina[3]->SetAnchorPoint({ 1.0f, 1.0f });

	crystalUI[0] = Sprite::Create(20, { 1080.0f , 90.0f });
	crystalUI[0]->SetSize({ 200, 500 });
	crystalUI[1] = Sprite::Create(21, { 1080.0f , 90.0f });
	crystalUI[1]->SetSize({ 200, 500 });
	crystalUI[2] = Sprite::Create(22, { 1080.0f , 90.0f });
	crystalUI[2]->SetSize({ 200, 500 });
	crystalUI[3] = Sprite::Create(23, { 1080.0f , 90.0f });
	crystalUI[3]->SetSize({ 200, 500 });
	crystalUI[4] = Sprite::Create(24, { 1080.0f , 90.0f });
	crystalUI[4]->SetSize({ 200, 500 });
	crystalUI[5] = Sprite::Create(25, { 1080.0f , 90.0f });
	crystalUI[5]->SetSize({ 200, 500 });
	crystalUI[6] = Sprite::Create(26, { 1080.0f , 90.0f });
	crystalUI[6]->SetSize({ 200, 500 });
	crystalUI[7] = Sprite::Create(27, { 1080.0f , 90.0f });
	crystalUI[7]->SetSize({ 200, 500 });
	crystalUI[8] = Sprite::Create(28, { 1080.0f , 90.0f });
	crystalUI[8]->SetSize({ 200, 500 });
	crystalUI[9]= Sprite::Create(29, { 1080.0f , 90.0f });
	crystalUI[9]->SetSize({ 200, 500 });
	crystalUI[10] = Sprite::Create(40, { 1080.0f , 90.0f });
	crystalUI[10]->SetSize({ 200, 500 });
	crystalUI[11] = Sprite::Create(41, { 1080.0f , 90.0f });
	crystalUI[11]->SetSize({ 200, 500 });
	crystalUI[12] = Sprite::Create(42, { 1080.0f , 90.0f });
	crystalUI[12]->SetSize({ 200, 500 });
	crystalUI[13] = Sprite::Create(43, { 1080.0f , 90.0f });
	crystalUI[13]->SetSize({ 200, 500 });
	crystalUI[14] = Sprite::Create(44, { 1080.0f , 90.0f });
	crystalUI[14]->SetSize({ 200, 500 });
	crystalUI[15] = Sprite::Create(45, { 1080.0f , 90.0f });
	crystalUI[15]->SetSize({ 200, 500 });
	crystalUI[16] = Sprite::Create(46, { 1080.0f , 90.0f });
	crystalUI[16]->SetSize({ 200, 500 });
	crystalUI[17] = Sprite::Create(47, { 1080.0f , 90.0f });
	crystalUI[17]->SetSize({ 200, 500 });
	crystalUI[18] = Sprite::Create(48, { 1080.0f , 90.0f });
	crystalUI[18]->SetSize({ 200, 500 });
	crystalUI[19] = Sprite::Create(49, { 1080.0f , 90.0f });
	crystalUI[19]->SetSize({ 200, 500 });
	crystalUI[20] = Sprite::Create(50, { 1080.0f , 90.0f });
	crystalUI[20]->SetSize({ 200, 500 });
	crystalUI[21] = Sprite::Create(51, { 1080.0f , 90.0f });
	crystalUI[21]->SetSize({ 200, 500 });
	crystalUI[22] = Sprite::Create(52, { 1080.0f , 90.0f });
	crystalUI[22]->SetSize({ 200, 500 });
	crystalUI[23] = Sprite::Create(53, { 1080.0f , 90.0f });
	crystalUI[23]->SetSize({ 200, 500 });
	crystalUI[24] = Sprite::Create(54, { 1080.0f , 90.0f });
	crystalUI[24]->SetSize({ 200, 500 });
	crystalUI[25] = Sprite::Create(55, { 1080.0f , 90.0f });
	crystalUI[25]->SetSize({ 200, 500 });

	timerCenter = Sprite::Create(30, { 1174 , 83.0f });
	timerCenter->SetSize({ 32, 32 });

	timerPetal[0] = Sprite::Create(31, { 1190 , 100.0f }, XMFLOAT4{ 1, 1, 1, 1 }, XMFLOAT2{ 0.5f, 1.0f });
	timerPetal[0]->SetSize({ 32, 64 });
	timerPetal[0]->SetRotation(0.0f);
	timerPetal[1] = Sprite::Create(31, { 1190 , 100.0f }, XMFLOAT4{ 1, 1, 1, 1 }, XMFLOAT2{ 0.5f, 1.0f });
	timerPetal[1]->SetSize({ 32, 64 });
	timerPetal[1]->SetRotation(40.0f);
	timerPetal[2] = Sprite::Create(31, { 1190 , 100.0f }, XMFLOAT4{ 1, 1, 1, 1 }, XMFLOAT2{ 0.5f, 1.0f });
	timerPetal[2]->SetSize({ 32, 64 });
	timerPetal[2]->SetRotation(80.0f);
	timerPetal[3] = Sprite::Create(31, { 1190 , 100.0f }, XMFLOAT4{ 1, 1, 1, 1 }, XMFLOAT2{ 0.5f, 1.0f });
	timerPetal[3]->SetSize({ 32, 64 });
	timerPetal[3]->SetRotation(120.0f);
	timerPetal[4] = Sprite::Create(31, { 1190 , 100.0f }, XMFLOAT4{ 1, 1, 1, 1 }, XMFLOAT2{ 0.5f, 1.0f });
	timerPetal[4]->SetSize({ 32, 64 });
	timerPetal[4]->SetRotation(160.0f);
	timerPetal[5] = Sprite::Create(31, { 1190 , 100.0f }, XMFLOAT4{ 1, 1, 1, 1 }, XMFLOAT2{ 0.5f, 1.0f });
	timerPetal[5]->SetSize({ 32, 64 });
	timerPetal[5]->SetRotation(200.0f);
	timerPetal[6] = Sprite::Create(31, { 1190 , 100.0f }, XMFLOAT4{ 1, 1, 1, 1 }, XMFLOAT2{ 0.5f, 1.0f });
	timerPetal[6]->SetSize({ 32, 64 });
	timerPetal[6]->SetRotation(240.0f);
	timerPetal[7] = Sprite::Create(31, { 1190 , 100.0f }, XMFLOAT4{ 1, 1, 1, 1 }, XMFLOAT2{ 0.5f, 1.0f });
	timerPetal[7]->SetSize({ 32, 64 });
	timerPetal[7]->SetRotation(280.0f);
	timerPetal[8] = Sprite::Create(31, { 1190 , 100.0f }, XMFLOAT4{ 1, 1, 1, 1 }, XMFLOAT2{ 0.5f, 1.0f });
	timerPetal[8]->SetSize({ 32, 64 });
	timerPetal[8]->SetRotation(320.0f);

	controllerUI = Sprite::Create(40, { 0.0f , 400.0f });
	controllerUI->SetSize({ 400.0f, 400.0f });
}

void UI::Update()
{
	float staminaSizeY = staminabarSize.y * Player::GetStaminaQuantity() / 100.0f;
	stamina[1]->SetSize({ staminabarSize.x, staminaSizeY });

	if (Player::GetStaminaCut() != true)
	{
		if (Player::GetStaminaQuantity() >= 30.0f)
		{
			stamina[1]->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
		}
		else
		{
			stamina[1]->SetColor({ 1.0f, 0.1f, 0.1f, 1.0f });
		}
	}
	else
	{
		stamina[1]->SetColor({ 1.0f, 0.1f, 0.1f, 1.0f });
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
	else if (Player::GetStatus() == 8)
	{
		controllerUI->SetTexNum(61);
	}
	else
	{
		controllerUI->SetTexNum(60);
	}
}

void UI::Draw(int crystalNum)
{
	if (Player::GetStaminaFlag() == true || Player::GetStaminaQuantity() != 100.0f)
	{
		stamina[0]->Draw();
		stamina[1]->Draw();
		stamina[2]->Draw();
		stamina[3]->Draw();
	}

	if (crystalNum == 0)
	{
		crystalUI[25]->Draw();
	}
	else if (crystalNum == 1)
	{
		crystalUI[24]->Draw();
	}
	else if (crystalNum == 2)
	{
		crystalUI[23]->Draw();
	}
	else if (crystalNum == 3)
	{
		crystalUI[22]->Draw();
	}
	else if (crystalNum == 4)
	{
		crystalUI[21]->Draw();
	}
	else if (crystalNum == 5)
	{
		crystalUI[20]->Draw();
	}
	else if (crystalNum == 6)
	{
		crystalUI[19]->Draw();
	}
	else if (crystalNum == 7)
	{
		crystalUI[18]->Draw();
	}
	else if (crystalNum == 8)
	{
		crystalUI[17]->Draw();
	}
	else if (crystalNum == 9)
	{
		crystalUI[16]->Draw();
	}
	else if (crystalNum == 10)
	{
		crystalUI[15]->Draw();
	}
	else if (crystalNum == 11)
	{
		crystalUI[14]->Draw();
	}
	else if (crystalNum == 12)
	{
		crystalUI[13]->Draw();
	}
	else if (crystalNum == 13)
	{
		crystalUI[12]->Draw();
	}
	else if (crystalNum == 14)
	{
		crystalUI[11]->Draw();
	}
	else if (crystalNum == 15)
	{
		crystalUI[10]->Draw();
	}
	else if (crystalNum == 16)
	{
		crystalUI[9]->Draw();
	}
	else if (crystalNum == 17)
	{
		crystalUI[8]->Draw();
	}
	else if (crystalNum == 18)
	{
		crystalUI[7]->Draw();
	}
	else if (crystalNum == 19)
	{
		crystalUI[6]->Draw();
	}
	else if (crystalNum == 20)
	{
		crystalUI[5]->Draw();
	}
	else if (crystalNum == 21)
	{
		crystalUI[4]->Draw();
	}
	else if (crystalNum == 22)
	{
		crystalUI[3]->Draw();
	}
	else if (crystalNum == 23)
	{
		crystalUI[2]->Draw();
	}
	else if (crystalNum == 24)
	{
		crystalUI[1]->Draw();
	}
	else if (crystalNum == 25)
	{
		crystalUI[0]->Draw();
	}
	

	
	timerPetal[0]->Draw();
	timerPetal[1]->Draw();
	timerPetal[2]->Draw();
	timerPetal[3]->Draw();
	timerPetal[4]->Draw();
	timerPetal[5]->Draw();
	timerPetal[6]->Draw();
	timerPetal[7]->Draw();
	timerPetal[8]->Draw();
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
		timerPetal[0]->SetTexNum(31);
		timerPetal[1]->SetTexNum(31);
		timerPetal[2]->SetTexNum(31);
		timerPetal[3]->SetTexNum(31);
		timerPetal[4]->SetTexNum(31);
		timerPetal[5]->SetTexNum(31);
		timerPetal[6]->SetTexNum(31);
		timerPetal[7]->SetTexNum(31);
		timerPetal[8]->SetTexNum(31);
	}

	
	if (time <= timeMax * (53.0f / 53.0f))
	{
		timerPetal[0]->SetTexNum(31);
	}
	if (time <= timeMax * (52.0f / 53.0f))
	{
		timerPetal[0]->SetTexNum(32);
	}
	if (time <= timeMax * (51.0f / 53.0f))
	{
		timerPetal[0]->SetTexNum(33);
	}
	if (time <= timeMax * (50.0f / 53.0f))
	{
		timerPetal[0]->SetTexNum(34);
	}
	if (time <= timeMax * (49.0f / 53.0f))
	{
		timerPetal[0]->SetTexNum(35);
	}
	if (time <= timeMax * (48.0f / 53.0f))
	{
		timerPetal[0]->SetTexNum(36);
	}
	
	if (time <= timeMax * (47.0f / 53.0f))
	{
		timerPetal[1]->SetTexNum(31);
	}
	if (time <= timeMax * (46.0f / 53.0f))
	{
		timerPetal[1]->SetTexNum(32);
	}
	if (time <= timeMax * (45.0f / 53.0f))
	{
		timerPetal[1]->SetTexNum(33);
	}
	if (time <= timeMax * (44.0f / 53.0f))
	{
		timerPetal[1]->SetTexNum(34);
	}
	if (time <= timeMax * (43.0f / 53.0f))
	{
		timerPetal[1]->SetTexNum(35);
	}
	if (time <= timeMax * (42.0f / 53.0f))
	{
		timerPetal[1]->SetTexNum(36);
	}
	
	if (time <= timeMax * (41.0f / 53.0f))
	{
		timerPetal[2]->SetTexNum(31);
	}
	if (time <= timeMax * (40.0f / 53.0f))
	{
		timerPetal[2]->SetTexNum(32);
	}
	if (time <= timeMax * (39.0f / 53.0f))
	{
		timerPetal[2]->SetTexNum(33);
	}
	if (time <= timeMax * (38.0f / 53.0f))
	{
		timerPetal[2]->SetTexNum(34);
	}
	if (time <= timeMax * (37.0f / 53.0f))
	{
		timerPetal[2]->SetTexNum(35);
	}
	if (time <= timeMax * (36.0f / 53.0f))
	{
		timerPetal[2]->SetTexNum(36);
	}
	
	if (time <= timeMax * (35.0f / 53.0f))
	{
		timerPetal[3]->SetTexNum(31);
	}
	if (time <= timeMax * (34.0f / 53.0f))
	{
		timerPetal[3]->SetTexNum(32);
	}
	if (time <= timeMax * (33.0f / 53.0f))
	{
		timerPetal[3]->SetTexNum(33);
	}
	if (time <= timeMax * (32.0f / 53.0f))
	{
		timerPetal[3]->SetTexNum(34);
	}
	if (time <= timeMax * (31.0f / 53.0f))
	{
		timerPetal[3]->SetTexNum(35);
	}
	if (time <= timeMax * (30.0f / 53.0f))
	{
		timerPetal[3]->SetTexNum(36);
	}
	
	if (time <= timeMax * (29.0f / 53.0f))
	{
		timerPetal[4]->SetTexNum(31);
	}
	if (time <= timeMax * (28.0f / 53.0f))
	{
		timerPetal[4]->SetTexNum(32);
	}
	if (time <= timeMax * (27.0f / 53.0f))
	{
		timerPetal[4]->SetTexNum(33);
	}
	if (time <= timeMax * (26.0f / 53.0f))
	{
		timerPetal[4]->SetTexNum(34);
	}
	if (time <= timeMax * (25.0f / 53.0f))
	{
		timerPetal[4]->SetTexNum(35);
	}
	if (time <= timeMax * (24.0f / 53.0f))
	{
		timerPetal[4]->SetTexNum(36);
	}
	
	if (time <= timeMax * (23.0f / 53.0f))
	{
		timerPetal[5]->SetTexNum(31);
	}
	if (time <= timeMax * (22.0f / 53.0f))
	{
		timerPetal[5]->SetTexNum(32);
	}
	if (time <= timeMax * (21.0f / 53.0f))
	{
		timerPetal[5]->SetTexNum(33);
	}
	if (time <= timeMax * (20.0f / 53.0f))
	{
		timerPetal[5]->SetTexNum(34);
	}
	if (time <= timeMax * (19.0f / 53.0f))
	{
		timerPetal[5]->SetTexNum(35);
	}
	if (time <= timeMax * (18.0f / 53.0f))
	{
		timerPetal[5]->SetTexNum(36);
	}
	
	if (time <= timeMax * (17.0f / 53.0f))
	{
		timerPetal[6]->SetTexNum(31);
	}
	if (time <= timeMax * (16.0f / 53.0f))
	{
		timerPetal[6]->SetTexNum(32);
	}
	if (time <= timeMax * (15.0f / 53.0f))
	{
		timerPetal[6]->SetTexNum(33);
	}
	if (time <= timeMax * (14.0f / 53.0f))
	{
		timerPetal[6]->SetTexNum(34);
	}
	if (time <= timeMax * (13.0f / 53.0f))
	{
		timerPetal[6]->SetTexNum(35);
	}
	if (time <= timeMax * (12.0f / 53.0f))
	{
		timerPetal[6]->SetTexNum(36);
	}
	
	if (time <= timeMax * (11.0f / 53.0f))
	{
		timerPetal[7]->SetTexNum(31);
	}
	if (time <= timeMax * (10.0f / 53.0f))
	{
		timerPetal[7]->SetTexNum(32);
	}
	if (time <= timeMax * (9.0f / 53.0f))
	{
		timerPetal[7]->SetTexNum(33);
	}
	if (time <= timeMax * (8.0f / 53.0f))
	{
		timerPetal[7]->SetTexNum(34);
	}
	if (time <= timeMax * (7.0f / 53.0f))
	{
		timerPetal[7]->SetTexNum(35);
	}	
	if (time <= timeMax * (6.0f / 53.0f))
	{
		timerPetal[7]->SetTexNum(36);
	}

	if (time <= timeMax * (5.0f / 53.0f))
	{
		timerPetal[8]->SetTexNum(31);
	}
	if (time <= timeMax * (4.0f / 53.0f))
	{
		timerPetal[8]->SetTexNum(32);
	}
	if (time <= timeMax * (3.0f / 53.0f))
	{
		timerPetal[8]->SetTexNum(33);
	}
	if (time <= timeMax * (2.0f / 53.0f))
	{
		timerPetal[8]->SetTexNum(34);
	}
	if (time <= timeMax * (1.0f / 53.0f))
	{
		timerPetal[8]->SetTexNum(35);
	}
	if (time <= timeMax * (0.0f / 53.0f))
	{
		timerPetal[8]->SetTexNum(36);
	}
	
}