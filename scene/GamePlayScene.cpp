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

	colliderObject->SetPosition(fbxObject1->GetPosition());
	colliderObject->SetCenter({ 0, 2.5f, 0 });
	colliderObject->SetScale({ 1, 5.0f, 1});

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

	//Box colPlayer;
	//colPlayer.centerPos = colliderObject->GetPosition();
	//colPlayer.size = { colliderObject->GetScale().x /2, colliderObject->GetScale().y / 2, colliderObject->GetScale().z / 2 };
	//colPlayer.MaxPos = { colPlayer.centerPos.x + colPlayer.size.x,colPlayer.centerPos.y + colPlayer.size.y,colPlayer.centerPos.z + colPlayer.size.z };
	//colPlayer.LeastPos = { colPlayer.centerPos.x - colPlayer.size.x,colPlayer.centerPos.y - colPlayer.size.y,colPlayer.centerPos.z - colPlayer.size.z };
	//
	//Box colBox1;
	//colBox1.centerPos = JsonLoader::colliderObjects[0].get()->GetPosition();
	//colBox1.size = { JsonLoader::colliderObjects[0].get()->GetScale().x / 2, JsonLoader::colliderObjects[0].get()->GetScale().y / 2, JsonLoader::colliderObjects[0].get()->GetScale().z / 2 };
	//colBox1.MaxPos = { colBox1.centerPos.x + colBox1.size.x,colBox1.centerPos.y + colBox1.size.y,colBox1.centerPos.z + colBox1.size.z };
	//colBox1.LeastPos = { colBox1.centerPos.x - colBox1.size.x,colBox1.centerPos.y - colBox1.size.y,colBox1.centerPos.z - colBox1.size.z };

	//if (Collision::CheckAABB(colPlayer, colBox1) == true)
	//{
	//	colliderObject->SetColor({ 1,1,0 });
	//	JsonLoader::colliderObjects[0].get()->SetColor({ 1,1,0 });
	//}
	//else
	//{
	//	colliderObject->SetColor({ 1,0,0 });
	//	JsonLoader::colliderObjects[0].get()->SetColor({ 1,0,0 });
	//}
	CollisionSet::CollisionCheck(colliderObject->GetPosition(), colliderObject->GetScale(), groundY);

	fbxObject1->SetPosition(Player::Move(input, groundY));

	DebugText::GetInstance()->Print(50, 30 * 1, 2, "X:%f", camera->GetEye().x);
	DebugText::GetInstance()->Print(50, 30 * 2, 2, "Y:%f", camera->GetEye().y);
	DebugText::GetInstance()->Print(50, 30 * 3, 2, "Z:%f", camera->GetEye().z);
	DebugText::GetInstance()->Print(50, 30 * 4, 2, "Y:%f", groundY);
	DebugText::GetInstance()->Print(50, 30 * 5, 2, "+X:%f", Player::GetMove().y);
	//DebugText::GetInstance()->Print(50, 30 * 6, 2, "-X:%f", colPlayer.MaxPos.x);
	//DebugText::GetInstance()->Print(50, 30 * 7, 2, "-X:%f", colBox1.LeastPos.x);

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
