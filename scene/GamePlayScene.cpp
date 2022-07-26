#include "GamePlayScene.h"
#include "SceneManager.h"
#include "Audio.h"
#include "Input.h"
#include "DebugText.h"
#include "DirectXCommon.h"
#include "MyMath.h"
#include "Easing.h"

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
	object3 = Object3d::Create();
	object4 = Object3d::Create();
	object5 = Object3d::Create();

	//オブジェクトにモデルをひも付ける
	object1->SetModel(model);
	object2->SetModel(model);
	object3->SetModel(model);
	object4->SetModel(model);
	object5->SetModel(model);
}

void GamePlayScene::Finalize()
{
	delete model;
}

void GamePlayScene::Update()
{
	// ゲームシーンの毎フレーム処理
	
	Input *input = Input::GetInstance();

	sphere1.center = pos1;
	sphere1.radius = 5.0f;

	if (input->PushKey(DIK_RETURN))
	{
		startFlag = true;
	}

	if (startFlag == true)
	{
		time += 1.0;
		pos1.x = (float)Easing::easeIn(time, startPos, lengPos, endTime);
		pos2.x = (float)Easing::easeOut(time, startPos, lengPos, endTime);
	}
	


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
	object3->SetPosition(pos3);
	object4->SetPosition(pos4);
	object5->SetPosition(pos5);

	DebugText::GetInstance()->Print(50, 30 * 1, 2, "        Camera:%f", camera->GetEye().x);
	DebugText::GetInstance()->Print(50, 30 * 2, 2, "        Camera:%f", camera->GetEye().y);
	DebugText::GetInstance()->Print(50, 30 * 3, 2, "          pos2X:%f", object1->GetPosition().x);
	DebugText::GetInstance()->Print(50, 30 * 4, 2, "          pos2Y:%f", object1->GetPosition().y);
	DebugText::GetInstance()->Print(50, 30 * 5, 2, "          pos2Z:%f", object1->GetPosition().z);
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
	object3->Update();
	object4->Update();
	object5->Update();
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
	object3->Draw();
	object4->Draw();
	object5->Draw();
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
