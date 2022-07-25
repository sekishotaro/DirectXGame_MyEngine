#include "GamePlayScene.h"
#include "SceneManager.h"
#include "Audio.h"
#include "Input.h"
#include "DebugText.h"
#include "DirectXCommon.h"
#include "Collision.h"
#include "MyMath.h"

void GamePlayScene::Initialize()
{
	Audio::GetInstance()->LoadWave("futta-dream.wav");
	Audio::GetInstance()->LoadWave("zaza.wav");

	//Audio::GetInstance()->PlayWave("zaza.wav", true);

	// カメラ生成
	camera = new Camera(WinApp::window_width, WinApp::window_height);

	// カメラセット
	Object3d::SetCamera(camera);
	XMFLOAT3 position = { 0.0f, 0.0f, -100.0f };
	// 座標の変更を反映
	camera->SetEye(position);
	// テクスチャ読み込み

	Sprite::LoadTexture(1, L"Resources/background.png");
	// 背景スプライト生成
	spriteBG = Sprite::Create(1, { 0.0f,0.0f });

	// オブジェクト生成
	model = Model::LoadFromOBJ("sphere");
	model2 = Model::LoadFromOBJ("sphere2");

	object1 = Object3d::Create();
	object2 = Object3d::Create();

	//オブジェクトにモデルをひも付ける
	object1->SetModel(model);
	object2->SetModel(model2);
}

void GamePlayScene::Finalize()
{
	delete model;
}

void GamePlayScene::Update()
{
	// ゲームシーンの毎フレーム処理
	
	Input *input = Input::GetInstance();

	if (input->PushKey(DIK_RETURN))
	{
		startFlag = true;
	}

	if (startFlag == true)
	{
		MyMath::Gravity(pos1, flaggra1);
		//MyMath::Gravity(pos2, flaggra2);
		MyMath::Friction(move1, flaggra1);
		MyMath::AirResistance(move1);
		MyMath::Movement(pos1, move1);

		MyMath::GravityCheckMove(move1, flaggra1);
	}



	//重力確認
	MyMath::GravityCheck(pos1, groundY, flaggra1);
	MyMath::GravityCheck(pos2, groundY, flaggra2);

	//カメラの移動
	if (input->PushKey(DIK_W) || input->PushKey(DIK_S) || input->PushKey(DIK_D) || input->PushKey(DIK_A))
	{
		// 現在の座標を取得
		XMFLOAT3 position = camera->GetEye();

		// 移動後の座標を計算
		if (input->PushKey(DIK_W)) { position.y += 1.0f; }
		else if (input->PushKey(DIK_S)) { position.y -= 1.0f; }
		if (input->PushKey(DIK_D)) { position.x += 1.0f; }
		else if (input->PushKey(DIK_A)) { position.x -= 1.0f; }

		// 座標の変更を反映
		camera->SetEye(position);
	}
	object1->SetPosition(pos1);
	object2->SetPosition(pos2);

	DebugText::GetInstance()->Print(50, 30 * 1, 2, "      Camera:%f", camera->GetEye().x);
	DebugText::GetInstance()->Print(50, 30 * 2, 2, "      Camera:%f", camera->GetEye().y);
	DebugText::GetInstance()->Print(50, 30 * 3, 2, "        posX:%f", object1->GetPosition().x);
	DebugText::GetInstance()->Print(50, 30 * 4, 2, "        posY:%f", object1->GetPosition().y);
	DebugText::GetInstance()->Print(50, 30 * 5, 2, "        posZ:%f", object1->GetPosition().z);
	DebugText::GetInstance()->Print(50, 30 * 6, 2, "       moveX:%f", move1.x);
	DebugText::GetInstance()->Print(50, 30 * 7, 2, "       moveY:%f", move1.y);
	DebugText::GetInstance()->Print(50, 30 * 8, 2, "       moveZ:%f", move1.z);
	if (input->TriggerKey(DIK_SPACE))
	{
		//BGM止める
		//Audio::GetInstance()->SoundStop("zaza.wav");
		
		//シーン切り替え
		SceneManager::GetInstance()->ChangeScene("TITLE");
	}

	//アップデート
	camera->Update();
	object1->Update();
	object2->Update();
}

void GamePlayScene::Draw()
{
	// ゲームシーンの描画

// コマンドリストの取得
	ID3D12GraphicsCommandList *cmdList = DirectXCommon::GetInstance()->GetCmdList();

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

	// 3Dオブクジェクトの描画
	object1->Draw();
	object2->Draw();
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
