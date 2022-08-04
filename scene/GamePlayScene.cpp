#include "GamePlayScene.h"
#include "SceneManager.h"
#include "Audio.h"
#include "Input.h"
#include "DebugText.h"
#include "DirectXCommon.h"
#include "FbxLoader.h"
#include "FbxObject3d.h"
#include "JsonLoader.h"
#include "imguiManager.h"
#include "MyMath.h"
#include "CollisionSet.h"

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
	ColliderObject::SetCamera(camera);
	//dCamera->SetEye({ 0, 0, 100 });			//prinding時
	camera->SetEye({ 0, 0, -30 });		//prin時
	FbxObject3d::SetCamera(camera);

	//グラフィックスパイプライン生成
	FbxObject3d::CreateGraphicsPipeline();

	// テクスチャ読み込み
	Sprite::LoadTexture(1, L"Resources/background.png");
	// 背景スプライト生成
	spriteBG = Sprite::Create(1, { 0.0f,0.0f });

	// オブジェクト生成
	//model = Model::LoadFromOBJ("sphere");
	//objectX = Object3d::Create();
	//オブジェクトにモデルをひも付ける
	//objectX->SetModel(model);

	//モデル名を指定してファイル読み込み
	fbxModel1 = FbxLoader::GetInstance()->LoadModelFromFile("model");

	//3Dオブジェクト生成とモデルのセット
	fbxObject1 = new FbxObject3d;
	fbxObject1->Initialize();
	fbxObject1->SetModel(fbxModel1);
	fbxObject1->SetScale({ 0.01f,0.01f,0.01f });

	colliderModel = ColliderModel::ColliderModelCreate("BOX");
	colliderObject = ColliderObject::Create();
	colliderObject->SetModel(colliderModel);

	colliderObject->SetCenter({ 0, 2.5f, 0 });
	colliderObject->SetScale({ 1, 5.0f, 1 });
	//json
	JsonLoader::LoadFile("Scene");
	JsonLoader::SetObject();
}

void GamePlayScene::Finalize()
{
	//delete model;
	delete fbxObject1;
}

void GamePlayScene::Update()
{
	// ゲームシーンの毎フレーム処理
	Input *input = Input::GetInstance();

	Input::MousePos mpos = input->MousePosLoad();

	if (input->PushKey(DIK_UP) || input->PushKey(DIK_DOWN) || input->PushKey(DIK_RIGHT) || input->PushKey(DIK_LEFT) || input->PushKey(DIK_E) || input->PushKey(DIK_Z))
	{
		// 現在の座標を取得
		XMFLOAT3 position = camera->GetEye();

		// 移動後の座標を計算
		if (input->PushKey(DIK_UP)) { position.y += 1.0f; }
		else if (input->PushKey(DIK_DOWN)) { position.y -= 1.0f; }
		if (input->PushKey(DIK_RIGHT)) { position.x += 1.0f; }
		else if (input->PushKey(DIK_LEFT)) { position.x -= 1.0f; }
		if (input->PushKey(DIK_E)) { position.z += 1.0f; }
		else if (input->PushKey(DIK_Z)) { position.z -= 1.0f; }
		// 座標の変更を反映
		camera->SetEye(position);
	}

	Player::Move(input, groundY);
	CollisionSet::CollisionCheck(Player::GetPos(), colliderObject->GetScale(), groundY);
	CollisionSet::CollisionPushBack(colliderObject->GetScale());

	fbxObject1->SetPosition(Player::GetPos());
	colliderObject->SetPosition(Player::GetPos());

	if (Player::GetWallColl() == true)
	{
		colliderObject->SetColor({ 1, 1, 0});
	}
	else if (Player::GetWallColl() == false)
	{
		colliderObject->SetColor({ 1, 0, 0});
	}

	DebugText::GetInstance()->Print(50, 30 * 1, 2, "X:%f", camera->GetEye().x);
	DebugText::GetInstance()->Print(50, 30 * 2, 2, "Y:%f", camera->GetEye().y);
	DebugText::GetInstance()->Print(50, 30 * 3, 2, "Z:%f", camera->GetEye().z);
	DebugText::GetInstance()->Print(50, 30 * 4, 2, "X:%f", Player::GetPos().x);
	DebugText::GetInstance()->Print(50, 30 * 5, 2, "Y:%f", Player::GetPos().y);
	DebugText::GetInstance()->Print(50, 30 * 6, 2, "Z:%f", Player::GetPos().z);
	DebugText::GetInstance()->Print(50, 30 * 7, 2, "X:%f", Player::GetMove().x);
	DebugText::GetInstance()->Print(50, 30 * 8, 2, "Y:%f", Player::GetMove().y);
	DebugText::GetInstance()->Print(50, 30 * 9, 2, "Z:%f", Player::GetMove().z);


	//if (input->TriggerKey(DIK_SPACE))
	//{
	//	//BGM止める
	//	Audio::GetInstance()->SoundStop("zaza.wav");
	//	Audio::GetInstance()->PlayWave("zaza.wav", false);
	//	
	//	//シーン切り替え
	//	//SceneManager::GetInstance()->ChangeScene("TITLE");
	//}	

	fbxObject1->AnimationFlag = false;
	fbxObject1->AnimationNum = 1;
	//アップデート
	camera->Update();
	//objectX->Update();
	fbxObject1->Update();
	colliderObject->Update();
	JsonLoader::Update();
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
	ColliderObject::PreDraw(cmdList);

	// 3Dオブクジェクトの描画
	//objectX->Draw();
	

	//json
	JsonLoader::Draw();

	//FBX3Dオブジェクトの描画
	fbxObject1->Draw(cmdList);
	colliderObject->Draw();
	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	// 3Dオブジェクト描画後処理
	Object3d::PostDraw();
	ColliderObject::PostDraw();
	// 前景スプライト描画前処理
	Sprite::PreDraw(cmdList);

	// デバッグテキストの描画
	DebugText::GetInstance()->DrawAll(cmdList);

	// スプライト描画後処理
	Sprite::PostDraw();

	//imguiの描画
	//imguiManager::PraDraw();
	//imguiManager::PosDraw();
}
