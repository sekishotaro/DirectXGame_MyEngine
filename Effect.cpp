#include "Effect.h"
#include "SafeDelete.h"
#include "WinApp.h"

#include "Enemy.h"
#include "Player.h"
#include "DebugCamera.h"

Sprite* Effect::EnemyAccessEffect = nullptr;
float Effect::EnemyAccessEffectSize = 100.0f;
float Effect::EnemyAccessDisSpeed = 1.0f;
float Effect::EnemyPlayerDis = 0.0f;
bool Effect::oldRaidFlag = false;

void Effect::Initialize()
{
	Sprite::LoadTexture(12, L"Resources/EnemyAccessEffect.png");
	float width = (float)WinApp::window_width / 2.0f;
	float height = (float)WinApp::window_height * 2.0f/ 3.0f;
	EnemyAccessEffect = Sprite::Create(12, { width, height });
	EnemyAccessEffect->SetSize({ EnemyAccessEffectSize, EnemyAccessEffectSize });
	EnemyAccessEffect->SetAnchorPoint({ 0.5f, 0.5f });
}

void Effect::Finalize()
{
	safe_delete(EnemyAccessEffect);
}

void Effect::Update(XMFLOAT3 cameraPos)
{
	EnemyAccessEffectUpdate(cameraPos);
}

void Effect::Draw()
{
	if (Enemy::GetRaidFlag() == true)
	{
		EnemyAccessEffect->Draw();
	}
	
}

void Effect::EnemyAccessEffectUpdate(XMFLOAT3 cameraPos)
{
	if (Enemy::GetRaidFlag() != true)
	{
		oldRaidFlag = Enemy::GetRaidFlag();
		return;
	}

	//サイズの変化量
	//敵の近くに来た時変化量を増やす
	if (oldRaidFlag == false)
	{
		EnemyPlayerDis = sqrt(Enemy::GetPos().x * Player::GetPos().x + Enemy::GetPos().z * Player::GetPos().z);

		oldRaidFlag = true;
	}


	float rad = 250;
	float NowDis = sqrt(Enemy::GetPos().x * Player::GetPos().x + Enemy::GetPos().z * Player::GetPos().z);
	
	if (NowDis < EnemyPlayerDis)
	{
		float p = NowDis / EnemyPlayerDis;
		rad = rad * p;
	}

	//位置の変化
	float rot = 0.0f;
	float sita = 0.0f;

	XMFLOAT2 player = {};
	XMFLOAT2 enemy  = {};

	player = { cameraPos.x - Player::GetPos().x, cameraPos.z - Player::GetPos().z };
	enemy = { cameraPos.x - Enemy::GetPos().x, cameraPos.z - Enemy::GetPos().z };

	float ab = player.x * enemy.x + player.y * enemy.y;
	float abLong = sqrtf((player.x * player.x) + (player.y * player.y)) * sqrtf((enemy.x * enemy.x) + (enemy.y * enemy.y));

	float cosSita = ab / abLong;
	sita = acos(cosSita);

	float cross = player.x * enemy.y - player.y * enemy.x;

	if (cross >= 0)
	{
		sita = 270.0f - sita;
	}
	sita += 90.0f;
	XMFLOAT2 pos = {};
	pos.x = ((float)WinApp::window_width / 2.0f) - rad * cos(sita);
	pos.y = ((float)WinApp::window_height * 2.5f / 3.0f) + rad * sin(sita);

	EnemyAccessEffect->SetPosition(pos);



	//サイズ反転
	if (EnemyAccessEffectSize > 250.0f)
	{
		EnemyAccessDisSpeed *= -1.0f;
	}
	
	if (EnemyAccessEffectSize < 100.0f)
	{
		EnemyAccessDisSpeed *= -1.0f;
	}

	EnemyAccessEffectSize += EnemyAccessDisSpeed;

	EnemyAccessEffect->SetSize({ EnemyAccessEffectSize, EnemyAccessEffectSize });
}
