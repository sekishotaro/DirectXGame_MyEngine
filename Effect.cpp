#include "Effect.h"
#include "SafeDelete.h"
#include "WinApp.h"

#include "Enemy.h"
#include "Player.h"
#include "DebugCamera.h"

Sprite* Effect::EnemyAccessEffect = nullptr;
Effect2d* Effect::crystalEffect = nullptr;
std::vector<Effect2d*> Effect::crystalEffects;
float Effect::EnemyAccessEffectSize = 100.0f;
float Effect::EnemyAccessDisSpeed = 1.0f;
float Effect::EnemyPlayerDis = 0.0f;
bool Effect::oldRaidFlag = false;

float Effect::crystalMoveTime = 0.0f;

void Effect::Initialize()
{
	Sprite::LoadTexture(12, L"Resources/EnemyAccessEffect.png");
	Effect2d::LoadTexture(13, L"Resources/particle.png");
	float width = (float)WinApp::window_width / 2.0f;
	float height = (float)WinApp::window_height / 2.0f;
	EnemyAccessEffect = Sprite::Create(12, { width, height });
	EnemyAccessEffect->SetSize({ EnemyAccessEffectSize, EnemyAccessEffectSize });
	EnemyAccessEffect->SetAnchorPoint({ 0.5f, 0.5f });
	
	
	//crystalEffect = Effect2d::Create(13, { width, height });
	//crystalEffect->SetAnchorPoint({ 0.5f, 0.5f });
	//crystalEffect->SetSize({ 50.0f, 50.0f });

	//for (int i = 0; i < 10; i++)
	//{
	//	crystalEffects.push_back(std::move(crystalEffect));
	//	crystalEffects[i]->SetPosition({ width + 10.0f * i, height });
	//}
}

void Effect::Finalize()
{
	safe_delete(EnemyAccessEffect);
}

void Effect::Update(XMFLOAT3 cameraPos)
{
	EnemyAccessEffectUpdate(cameraPos);

	ClystalEffectUpdate();
}

void Effect::Draw()
{
	if (Enemy::GetRaidFlag() == true)
	{
		EnemyAccessEffect->Draw();
	}

	/*crystalEffect->Draw();*/

	for (int i = 1; i < crystalEffects.size(); i++)
	{
		crystalEffects[i]->Draw();
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

void Effect::ClystalEffectUpdate()
{
	//取得アイテムを取得した時から
	if (Player::GetCrystalGetFlag() != true && crystalMoveTime == 0.0f) return;

	if (crystalMoveTime <= 5.0f)
	{
		//エフェクトの時間を進める
		crystalMoveTime += 1.0f / 60.0f;
	}
	else
	{
		if (crystalEffects.size() == 1)
		{
			crystalMoveTime = 0.0f;
		}
	}

	const float MoveMaxTime = 3.0f; //移動にかかる時間
	float timeRatio = crystalMoveTime / MoveMaxTime;

	//取得したアイテムのカメラ座標 (画面中央) :Todo
	float width = (float)WinApp::window_width / 2.0f;
	float height = (float)WinApp::window_height / 2.0f;
	XMFLOAT2 startPos = {width, height};
	XMFLOAT2 endPos = { (float)WinApp::window_width - 50.0f, 50.0f };


	XMFLOAT2 nowPos = Effect::leap(startPos, endPos, timeRatio);

	if (timeRatio <= 1.0f)
	{
		Effect2d* Effect = nullptr;
		Effect = Effect2d::Create(13, nowPos);
		Effect->SetSize({ 50.0f, 50.0f });
		Effect->SetAnchorPoint({ 0.5f, 0.5f });
		crystalEffects.push_back(std::move(Effect));
	}


	for (int i = 0; i < crystalEffects.size(); i++)
	{
		XMFLOAT2 size = crystalEffects[i]->GetSize();
		size.x -= 1.0f;
		size.y -= 1.0f;
		crystalEffects[i]->SetSize(size);

		if (size.x <= 0.0f)
		{
			crystalEffects.erase(crystalEffects.begin() + i);
		}
	}
}

Effect::XMFLOAT2 Effect::leap(XMFLOAT2 start, XMFLOAT2 end, float time)
{
	XMFLOAT2 result = {};
	result.x = start.x * (1.0f - time) + end.x * time;
	result.y = start.y * (1.0f - time) + end.y * time;
	//result.z = start.z * (1.0f * time) + end.z * time;
	return result;
}
