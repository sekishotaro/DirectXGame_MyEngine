#include "UI.h"
#include "Player.h"

void StaminaUI::Initialize()
{
	Sprite::LoadTexture(2, L"Resources/staminaBar/staminaBar_0.png");
	Sprite::LoadTexture(3, L"Resources/staminaBar/staminaBar_1.png");
	Sprite::LoadTexture(4, L"Resources/staminaBar/staminaBar_2.png");
	Sprite::LoadTexture(5, L"Resources/staminaBar/staminaBar_3.png");
	stamina[0] = Sprite::Create(2, { 800.0f,350.0f });
	stamina[0]->SetAnchorPoint({ 1.0f, 1.0f });
	stamina[1] = Sprite::Create(3, { 800.0f,350.0f });
	stamina[1]->SetAnchorPoint({ 1.0f, 1.0f });
	staminabarSize = stamina[1]->GetSize();
	stamina[2] = Sprite::Create(4, { 800.0f,350.0f });
	stamina[2]->SetAnchorPoint({ 1.0f, 1.0f });
	stamina[3] = Sprite::Create(5, { 800.0f,350.0f });
	stamina[3]->SetAnchorPoint({ 1.0f, 1.0f });
}

void StaminaUI::Update(Player* player)
{
	float staminaSizeY = staminabarSize.y * player->GetStaminaQuantity() / 100.0f;
	stamina[1]->SetSize({ staminabarSize.x, staminaSizeY });

	if (player->GetStaminaCutFlag() != true)
	{
		if (player->GetStaminaQuantity() >= 30.0f)
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
}

void StaminaUI::Draw(Player* player)
{
	if (player->GetStaminaBoostFlag() == true || player->GetStaminaQuantity() != 100.0f)
	{
		stamina[0]->Draw();
		stamina[1]->Draw();
		stamina[2]->Draw();
		stamina[3]->Draw();
	}
}

void TimeUI::Initialize()
{
	Sprite::LoadTexture(20, L"Resources/UI/timer/timerCenter.png");
	Sprite::LoadTexture(21, L"Resources/UI/timer/timerPetal0.png");
	Sprite::LoadTexture(22, L"Resources/UI/timer/timerPetal1.png");
	Sprite::LoadTexture(23, L"Resources/UI/timer/timerPetal2.png");
	Sprite::LoadTexture(24, L"Resources/UI/timer/timerPetal3.png");
	Sprite::LoadTexture(25, L"Resources/UI/timer/timerPetal4.png");
	Sprite::LoadTexture(26, L"Resources/UI/timer/timerPetal5.png");

	timerCenter = Sprite::Create(20, { 1174 , 83.0f });
	timerCenter->SetSize({ 32, 32 });

	for (int i = 0; i < 9; i++)
	{
		timerPetal[i] = Sprite::Create(21, { 1190 , 100.0f }, XMFLOAT4{ 1, 1, 1, 1 }, XMFLOAT2{ 0.5f, 1.0f });
		timerPetal[i]->SetSize({ 32, 64 });
		timerPetal[i]->SetRotation(i * 40.0f);
	}
}

void TimeUI::Update(Player* player)
{
	const int timeMax = (int)Player::GetTimeMax();
	const int time = (int)Player::GetTimeLimit();
	const int seconds_between = 2;

	if (Player::GetCrystalGetFlag() || time == timeMax)
	{
		timerPetal[0]->SetTexNum(21);
		timerPetal[1]->SetTexNum(21);
		timerPetal[2]->SetTexNum(21);
		timerPetal[3]->SetTexNum(21);
		timerPetal[4]->SetTexNum(21);
		timerPetal[5]->SetTexNum(21);
		timerPetal[6]->SetTexNum(21);
		timerPetal[7]->SetTexNum(21);
		timerPetal[8]->SetTexNum(21);
	}

	if (time <= timeMax * (53.0f / 53.0f))
	{
		timerPetal[0]->SetTexNum(21);
	}
	if (time <= timeMax * (52.0f / 53.0f))
	{
		timerPetal[0]->SetTexNum(22);
	}
	if (time <= timeMax * (51.0f / 53.0f))
	{
		timerPetal[0]->SetTexNum(23);
	}
	if (time <= timeMax * (50.0f / 53.0f))
	{
		timerPetal[0]->SetTexNum(24);
	}
	if (time <= timeMax * (49.0f / 53.0f))
	{
		timerPetal[0]->SetTexNum(25);
	}
	if (time <= timeMax * (48.0f / 53.0f))
	{
		timerPetal[0]->SetTexNum(26);
	}

	if (time <= timeMax * (47.0f / 53.0f))
	{
		timerPetal[1]->SetTexNum(21);
	}
	if (time <= timeMax * (46.0f / 53.0f))
	{
		timerPetal[1]->SetTexNum(22);
	}
	if (time <= timeMax * (45.0f / 53.0f))
	{
		timerPetal[1]->SetTexNum(23);
	}
	if (time <= timeMax * (44.0f / 53.0f))
	{
		timerPetal[1]->SetTexNum(24);
	}
	if (time <= timeMax * (43.0f / 53.0f))
	{
		timerPetal[1]->SetTexNum(25);
	}
	if (time <= timeMax * (42.0f / 53.0f))
	{
		timerPetal[1]->SetTexNum(26);
	}

	if (time <= timeMax * (41.0f / 53.0f))
	{
		timerPetal[2]->SetTexNum(21);
	}
	if (time <= timeMax * (40.0f / 53.0f))
	{
		timerPetal[2]->SetTexNum(22);
	}
	if (time <= timeMax * (39.0f / 53.0f))
	{
		timerPetal[2]->SetTexNum(23);
	}
	if (time <= timeMax * (38.0f / 53.0f))
	{
		timerPetal[2]->SetTexNum(24);
	}
	if (time <= timeMax * (37.0f / 53.0f))
	{
		timerPetal[2]->SetTexNum(25);
	}
	if (time <= timeMax * (36.0f / 53.0f))
	{
		timerPetal[2]->SetTexNum(26);
	}

	if (time <= timeMax * (35.0f / 53.0f))
	{
		timerPetal[3]->SetTexNum(21);
	}
	if (time <= timeMax * (34.0f / 53.0f))
	{
		timerPetal[3]->SetTexNum(22);
	}
	if (time <= timeMax * (33.0f / 53.0f))
	{
		timerPetal[3]->SetTexNum(23);
	}
	if (time <= timeMax * (32.0f / 53.0f))
	{
		timerPetal[3]->SetTexNum(24);
	}
	if (time <= timeMax * (31.0f / 53.0f))
	{
		timerPetal[3]->SetTexNum(25);
	}
	if (time <= timeMax * (30.0f / 53.0f))
	{
		timerPetal[3]->SetTexNum(26);
	}

	if (time <= timeMax * (29.0f / 53.0f))
	{
		timerPetal[4]->SetTexNum(21);
	}
	if (time <= timeMax * (28.0f / 53.0f))
	{
		timerPetal[4]->SetTexNum(22);
	}
	if (time <= timeMax * (27.0f / 53.0f))
	{
		timerPetal[4]->SetTexNum(23);
	}
	if (time <= timeMax * (26.0f / 53.0f))
	{
		timerPetal[4]->SetTexNum(24);
	}
	if (time <= timeMax * (25.0f / 53.0f))
	{
		timerPetal[4]->SetTexNum(25);
	}
	if (time <= timeMax * (24.0f / 53.0f))
	{
		timerPetal[4]->SetTexNum(26);
	}

	if (time <= timeMax * (23.0f / 53.0f))
	{
		timerPetal[5]->SetTexNum(21);
	}
	if (time <= timeMax * (22.0f / 53.0f))
	{
		timerPetal[5]->SetTexNum(22);
	}
	if (time <= timeMax * (21.0f / 53.0f))
	{
		timerPetal[5]->SetTexNum(23);
	}
	if (time <= timeMax * (20.0f / 53.0f))
	{
		timerPetal[5]->SetTexNum(24);
	}
	if (time <= timeMax * (19.0f / 53.0f))
	{
		timerPetal[5]->SetTexNum(25);
	}
	if (time <= timeMax * (18.0f / 53.0f))
	{
		timerPetal[5]->SetTexNum(26);
	}

	if (time <= timeMax * (17.0f / 53.0f))
	{
		timerPetal[6]->SetTexNum(21);
	}
	if (time <= timeMax * (16.0f / 53.0f))
	{
		timerPetal[6]->SetTexNum(22);
	}
	if (time <= timeMax * (15.0f / 53.0f))
	{
		timerPetal[6]->SetTexNum(23);
	}
	if (time <= timeMax * (14.0f / 53.0f))
	{
		timerPetal[6]->SetTexNum(24);
	}
	if (time <= timeMax * (13.0f / 53.0f))
	{
		timerPetal[6]->SetTexNum(25);
	}
	if (time <= timeMax * (12.0f / 53.0f))
	{
		timerPetal[6]->SetTexNum(26);
	}

	if (time <= timeMax * (11.0f / 53.0f))
	{
		timerPetal[7]->SetTexNum(21);
	}
	if (time <= timeMax * (10.0f / 53.0f))
	{
		timerPetal[7]->SetTexNum(22);
	}
	if (time <= timeMax * (9.0f / 53.0f))
	{
		timerPetal[7]->SetTexNum(23);
	}
	if (time <= timeMax * (8.0f / 53.0f))
	{
		timerPetal[7]->SetTexNum(24);
	}
	if (time <= timeMax * (7.0f / 53.0f))
	{
		timerPetal[7]->SetTexNum(25);
	}
	if (time <= timeMax * (6.0f / 53.0f))
	{
		timerPetal[7]->SetTexNum(26);
	}

	if (time <= timeMax * (5.0f / 53.0f))
	{
		timerPetal[8]->SetTexNum(21);
	}
	if (time <= timeMax * (4.0f / 53.0f))
	{
		timerPetal[8]->SetTexNum(22);
	}
	if (time <= timeMax * (3.0f / 53.0f))
	{
		timerPetal[8]->SetTexNum(23);
	}
	if (time <= timeMax * (2.0f / 53.0f))
	{
		timerPetal[8]->SetTexNum(24);
	}
	if (time <= timeMax * (1.0f / 53.0f))
	{
		timerPetal[8]->SetTexNum(25);
	}
	if (time <= timeMax * (0.0f / 53.0f))
	{
		timerPetal[8]->SetTexNum(26);
	}
}

void TimeUI::Draw(Player* player)
{
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
}

void ControllerUI::Initialize()
{
	Sprite::LoadTexture(60, L"Resources/UI/controller/controllerUI.png");
	Sprite::LoadTexture(61, L"Resources/UI/controller/controllerUI2.png");
	Sprite::LoadTexture(62, L"Resources/UI/controller/controllerUI3.png");
	Sprite::LoadTexture(63, L"Resources/UI/controller/controllerUI4.png");

	controllerUI = Sprite::Create(40, { 0.0f , 400.0f });
	controllerUI->SetSize({ 400.0f, 400.0f });
}

void ControllerUI::Update(Player* player)
{
	if (player->GetStatus() == 6)
	{
		controllerUI->SetTexNum(62);
	}
	else if (Player::GetMoveBoxHitFlag())
	{
		controllerUI->SetTexNum(63);
	}
	else if (player->GetStatus() == 8)
	{
		controllerUI->SetTexNum(61);
	}
	else
	{
		controllerUI->SetTexNum(60);
	}
}

void ControllerUI::Draw(Player* player)
{
	controllerUI->Draw();
}

void CrystalUI::Initialize()
{
	Sprite::LoadTexture(30, L"Resources/UI/CrystalUI/Crystal_Null.png");
	Sprite::LoadTexture(31, L"Resources/UI/CrystalUI/Crystal_1.png");
	Sprite::LoadTexture(32, L"Resources/UI/CrystalUI/Crystal_2.png");
	Sprite::LoadTexture(33, L"Resources/UI/CrystalUI/Crystal_3.png");
	Sprite::LoadTexture(34, L"Resources/UI/CrystalUI/Crystal_4.png");
	Sprite::LoadTexture(35, L"Resources/UI/CrystalUI/Crystal_5.png");
	Sprite::LoadTexture(36, L"Resources/UI/CrystalUI/Crystal_6.png");
	Sprite::LoadTexture(37, L"Resources/UI/CrystalUI/Crystal_7.png");
	Sprite::LoadTexture(38, L"Resources/UI/CrystalUI/Crystal_8.png");
	Sprite::LoadTexture(39, L"Resources/UI/CrystalUI/Crystal_9.png");
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

	for (int i = 0; i < 26; i++)
	{
		crystalUI[i] = Sprite::Create(30 + i, { 1080.0f , 90.0f });
		crystalUI[i]->SetSize({ 200, 500 });
	}
}

void CrystalUI::Update(Player* player)
{

}

void CrystalUI::Draw(Player* player)
{
	int crystalNum = player->GetCrystal();
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

}
