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
	const int timeMax = (int)Player::GetTimeMax();	//êßå¿éûä‘ÇÃç≈ëÂêî
	const int time = (int)Player::GetTimeLimit();	//åªç›ÇÃêßå¿éûä‘êî
	const int petalNum = 53;						//â‘Ç—ÇÁÇÃÉpÉ^Å[Éìêî

	//êßå¿éûä‘âÒïú
	if (Player::GetCrystalGetFlag() || time == timeMax)
	{
		for (int i = 0; i < 9; i++)
		{
			timerPetal[i]->SetTexNum(21);
		}
		return;
	}

	int a = petalNum;

	for (int i = 0; i < timePetalNum; i++)		//â‘Ç—ÇÁÇÃñáêî
	{
		for (int j = 0; j < timePetalPattern; j++)	//â‘Ç—ÇÁÇÃéÌóﬁ
		{
			if (time <= timeMax * a / petalNum)
			{
				timerPetal[i]->SetTexNum(21 + j);
			}
			a--;
		}
	}
}

void TimeUI::Draw(Player* player)
{
	for (int i = 0; i < 9; i++)
	{
		timerPetal[i]->Draw();
	}
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
	int crystalNum = player->GetCrystalMaxNum() - player->GetCrystal();

	crystalUI[crystalNum]->Draw();
}
