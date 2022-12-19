#include "GameOver.h"
#include "SceneManager.h"
#include "Audio.h"
#include "Input.h"
#include "DebugText.h"
#include "DirectXCommon.h"

void GameOver::Initialize()
{
	// テクスチャ読み込み
	Sprite::LoadTexture(1, L"Resources/background2.png");
	// 背景スプライト生成
	spriteBG = Sprite::Create(1, { 0.0f,0.0f });
}

void GameOver::Finalize()
{
}

void GameOver::Update()
{
	// ゲームシーンの毎フレーム処理

	Input* input = Input::GetInstance();

	if (input->TriggerKey(DIK_RETURN) || input->PushPadbutton(Button_A))
	{
		//シーン切り替え
		SceneManager::GetInstance()->ChangeScene("GAMEPLAY");
	}

	if (input->TriggerKey(DIK_SPACE) || input->PushPadbutton(Button_Y))
	{
		//シーン切り替え
		SceneManager::GetInstance()->ChangeScene("TITLE");
	}

	DebugText::GetInstance()->Print(50, 20, 2, "Push_Key [ENTER] or Push_Pad [Y] for TITLE_SCENE");
	DebugText::GetInstance()->Print(50, 60, 2, "Push_Key [SPACE] or Push_Pad [A] for GAMEPLAY_SCENE");
}

void GameOver::Draw()
{
	// ゲームシーンの描画

	// コマンドリストの取得
	ID3D12GraphicsCommandList* cmdList = DirectXCommon::GetInstance()->GetCmdList();

	// 背景スプライト描画前処理
	Sprite::PreDraw(cmdList);
	// 背景スプライト描画
	spriteBG->Draw();

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	DirectXCommon::GetInstance()->ClearDepthBuffer();

	// 3Dオブジェクト描画前処理
	Object3d::PreDraw(cmdList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	// 3Dオブジェクト描画後処理
	Object3d::PostDraw();

	// 前景スプライト描画前処理
	Sprite::PreDraw(cmdList);

	// デバッグテキストの描画
	DebugText::GetInstance()->DrawAll(cmdList);

	// スプライト描画後処理
	Sprite::PostDraw();
}