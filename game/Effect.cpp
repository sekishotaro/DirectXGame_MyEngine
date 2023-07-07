#include "Effect.h"
#include "SafeDelete.h"
#include "WinApp.h"
#include "Player.h"
#include "GameCamera.h"

//Effect2d* Effect::crystalEffect = nullptr;
//std::vector<Effect2d*> Effect::crystalEffects;
//float Effect::crystalMoveTime = 0.0f;
//const Effect::XMFLOAT2 Effect::crystalUIPos[8] = { { 1190.0f, 165.0f },{ 1250.0f, 180.0f },{ 1230.0f, 290.0f },{ 1200.0f, 300.0f },
//	{ 1130.0f, 300.0f },{ 1140.0f, 390.0f },{ 1145.0f, 440.0f },{ 1210.0f, 460.0f }};


void Effect::Initialize()
{
	Effect2d::LoadTexture(13, L"Resources/particle.png");
	float width = 3000.0f;		//画面外
	float height = 3000.0f;		//画面外
	
	crystalEffect = Effect2d::Create(13, { width, height });
	crystalEffect->SetAnchorPoint({ 0.5f, 0.5f });
	crystalEffect->SetSize({ 20.0f, 20.0f });
	for (int i = 0; i < 10; i++)
	{
		crystalEffects.push_back(std::move(crystalEffect));
		crystalEffects[i]->SetPosition({ width + 10.0f * i, height });
	}
}

void Effect::Finalize()
{
}

void Effect::Update(XMFLOAT3 cameraPos, int crystalNum)
{

	ClystalEffectUpdate(crystalNum);
}

void Effect::Draw()
{
	for (int i = 1; i < crystalEffects.size(); i++)
	{
		crystalEffects[i]->Draw();
	}
}

void Effect::ClystalEffectUpdate(int crystalNum)
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
		if (crystalEffects.size() == 0)
		{
			crystalMoveTime = 0.0f;
		}
	}

	const float MoveMaxTime = 1.0f; //移動にかかる時間
	float timeRatio = crystalMoveTime / MoveMaxTime;

	//取得したアイテムのカメラ座標 (画面中央) :Todo
	float width = (float)WinApp::window_width / 2.0f;
	float height = (float)WinApp::window_height / 2.0f;
	XMFLOAT2 startPos = {width, height};

	int crystalNumS = 7;

	crystalNumS -= crystalNum;

	XMFLOAT2 endPos = { 1190.0f, 50.0f };

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
