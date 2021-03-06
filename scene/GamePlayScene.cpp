
#include "GamePlayScene.h"
#include "SceneManager.h"
#include "Audio.h"
#include "Input.h"
#include "DebugText.h"
#include "DirectXCommon.h"
#include "FbxLoader.h"
#include "FbxObject3d.h"

void GamePlayScene::Initialize()
{
	Audio::GetInstance()->LoadWave("futta-dream.wav");
	Audio::GetInstance()->LoadWave("zaza.wav");
	Audio::GetInstance()->LoadWave("What.wav");

	
	// カメラ生成
	camera = new DebugCamera(WinApp::window_width, WinApp::window_height);
	//camera = new Camera(WinApp::window_width, WinApp::window_height);

	//デバイスのセット
	FbxObject3d::SetDevice(DirectXCommon::GetInstance()->GetDev());

	// カメラセット
	Object3d::SetCamera(camera);
	//dCamera->SetEye({ 0, 0, 100 });			//prinding時
	camera->SetEye({ 0, 0, 300 });		//prin時
	FbxObject3d::SetCamera(camera);

	//グラフィックスパイプライン生成
	FbxObject3d::CreateGraphicsPipeline();

	// テクスチャ読み込み
	Sprite::LoadTexture(1, L"Resources/background.png");
	// 背景スプライト生成
	spriteBG = Sprite::Create(1, { 0.0f,0.0f });

	// オブジェクト生成
	model = Model::LoadFromOBJ("sphere");
	objectX = Object3d::Create();
	//オブジェクトにモデルをひも付ける
	objectX->SetModel(model);

	//モデル名を指定してファイル読み込み
	fbxModel1 = FbxLoader::GetInstance()->LoadModelFromFile("prin");

	//3Dオブジェクト生成とモデルのセット
	fbxObject1 = new FbxObject3d;
	fbxObject1->Initialize();
	fbxObject1->SetModel(fbxModel1);

}

void GamePlayScene::Finalize()
{
	delete model;
	delete fbxObject1;
	//delete fbxModel1;
}

void GamePlayScene::Update()
{
	// ゲームシーンの毎フレーム処理
	Input *input = Input::GetInstance();

	Input::MousePos mpos = input->MousePosLoad();

	//マウスの中ボタンが押されていたらカメラを並行移動させる
	//if (input->PushMouseButton(Mouse_Middle))
	//{
	//	float dx = mpos.lX;
	//	float dy = mpos.lY;

	//	XMFLOAT3 position = camera->GetEye();
	//	position.x += dx;
	//	position.y += dy;
	//	camera->SetEye(position);

	//	position = camera->GetTarget();
	//	position.x += dx;
	//	position.y += dy;
	//	camera->SetTarget(position);
	//}

	//Audio::GetInstance()->PlayWave("What.wav", false);

	//if (input->PushMouseButton(Mouse_Left))
	//{
	//	XMFLOAT3 position = camera->GetEye();
	//	position.y += 1.0f;
	//	camera->SetEye(position);
	//}

	//オブジェクト移動
	if (input->PushKey(DIK_UP) || input->PushKey(DIK_DOWN) || input->PushKey(DIK_RIGHT) || input->PushKey(DIK_LEFT))
	{
		// 現在の座標を取得
		XMFLOAT3 position = objectX->GetPosition();

		// 移動後の座標を計算
		if (input->PushKey(DIK_UP)) { position.y += 1.0f; }
		else if (input->PushKey(DIK_DOWN)) { position.y -= 1.0f; }
		if (input->PushKey(DIK_RIGHT)) { position.x += 1.0f; }
		else if (input->PushKey(DIK_LEFT)) { position.x -= 1.0f; }

		// 座標の変更を反映
		objectX->SetPosition(position);
	}

	if (input->PushKey(DIK_W) || input->PushKey(DIK_S) || input->PushKey(DIK_D) || input->PushKey(DIK_A) || input->PushKey(DIK_E) || input->PushKey(DIK_Z))
	{
		// 現在の座標を取得
		XMFLOAT3 position = camera->GetEye();

		// 移動後の座標を計算
		if (input->PushKey(DIK_W)) { position.y += 1.0f; }
		else if (input->PushKey(DIK_S)) { position.y -= 1.0f; }
		if (input->PushKey(DIK_D)) { position.x += 1.0f; }
		else if (input->PushKey(DIK_A)) { position.x -= 1.0f; }
		if (input->PushKey(DIK_E)) { position.z += 1.0f; }
		else if (input->PushKey(DIK_Z)) { position.z -= 1.0f; }
		// 座標の変更を反映
		camera->SetEye(position);
	}

	DebugText::GetInstance()->Print(50, 30 * 1, 2, "X:%f", camera->GetEye().x);
	DebugText::GetInstance()->Print(50, 30 * 2, 2, "Y:%f", camera->GetEye().y);
	DebugText::GetInstance()->Print(50, 30 * 3, 2, "Z:%f", camera->GetEye().z);
	DebugText::GetInstance()->Print(50, 30 * 4, 2, "X:%f", DebugCamera::dx);
	DebugText::GetInstance()->Print(50, 30 * 5, 2, "Y:%f", DebugCamera::dy);
	DebugText::GetInstance()->Print(50, 30 * 6, 2, "Z:%f", DebugCamera::dz);

	if (input->TriggerKey(DIK_SPACE))
	{
		//BGM止める
		Audio::GetInstance()->SoundStop("zaza.wav");
		Audio::GetInstance()->PlayWave("zaza.wav", false);
		
		//シーン切り替え
		//SceneManager::GetInstance()->ChangeScene("TITLE");
	}	

	fbxObject1->AnimationFlag = true;
	fbxObject1->AnimationNum = 1;
	//アップデート
	camera->Update();
	objectX->Update();
	fbxObject1->Update();
	DebugText::GetInstance()->Print(50, 30 * 3, 2, "%d", fbxObject1->GetisPlay());
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
	objectX->Draw();

	//FBX3Dオブジェクトの描画
	fbxObject1->Draw(cmdList);

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
