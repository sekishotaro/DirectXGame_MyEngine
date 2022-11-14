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
#include "imgui.h"
#include "MyMath.h"
#include "CollisionSet.h"
#include "Enemy.h"

#include "SphereCollider.h"
#include "CollisionManager.h"
#include "Player.h"
#include "UI.h"

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

	MathObject::SetCamera(camera);
	//dCamera->SetEye({ 0, 0, 100 });			//prinding時
	camera->SetEye({ 0, 0, -30 });		//prin時
	FbxObject3d::SetCamera(camera);

	//グラフィックスパイプライン生成
	FbxObject3d::CreateGraphicsPipeline();

	// テクスチャ読み込み
	Sprite::LoadTexture(1, L"Resources/background.png");
	// 背景スプライト生成
	spriteBG = Sprite::Create(1, { 0.0f,0.0f });

	modelFighter = Model::LoadFromOBJ("modelObj");
	
	//json
	JsonLoader::LoadFile("Scene11_01"); //オブジェクトの当たり判定

	JsonLoader::SetObject();

	collisionManager = CollisionManager::GetInstance();
	objFighter = Player::Create(modelFighter);

	UI::Initialize();
}

void GamePlayScene::Finalize()
{
}

void GamePlayScene::Update()
{
	//キーボード、マウスのインプット
	Input *input = Input::GetInstance();
	Input::MousePos mpos = input->MousePosLoad();

	//ゲーム終了処理
	if (objFighter->GetCrystal() == 0 && objFighter->GetGoalFlag() == true)
	{
		SceneManager::GetInstance()->ChangeScene("TITLE");
	}

	//アップデート
	JsonLoader::Update();

	UI::Update();

	camera->Update();
	objFighter->Update();

	//UI更新
	DebugText::GetInstance()->Print(1000, 20, 3, "TIME : %d", (int)objFighter->GetTimeLimit());
	DebugText::GetInstance()->Print(940, 80, 3, "CRYSTAL : %d", objFighter->GetCrystal());

	//全ての衝突をチェック
	collisionManager->CheckAllCollisions();
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
	MathObject::PreDraw(cmdList);

	// 3Dオブクジェクトの描画
	
	objFighter->Draw();
	//json
	JsonLoader::Draw();

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	// 3Dオブジェクト描画後処理
	Object3d::PostDraw();
	ColliderObject::PostDraw();
	MathObject::PostDraw();
	// 前景スプライト描画前処理
	Sprite::PreDraw(cmdList);
	
	UI::Draw();


	// デバッグテキストの描画
	DebugText::GetInstance()->DrawAll(cmdList);

	// スプライト描画後処理
	Sprite::PostDraw();

	//imguiの描画
	imguiManager::PraDraw();
	ImGui::NewFrame();
	ImGui::Begin("config1");//ウィンドウの名前
	ImGui::SetWindowSize(ImVec2(400, 500), ImGuiCond_::ImGuiCond_FirstUseEver);
	
	ImGui::Text("StaminaQ: %.4f", objFighter->GetStaminaQuantity());
	ImGui::Text("TimeLimit: %.4f", objFighter->GetTimeLimit());
	ImGui::Text("PosX    :%.4f", objFighter->GetPosition().x);
	ImGui::Text("PosY    :%.4f", objFighter->GetPosition().y);
	ImGui::Text("PosZ    :%.4f", objFighter->GetPosition().z);
	ImGui::Text("MoveX   :%.4f", objFighter->GetMove().x);
	ImGui::Text("MoveY   :%.4f", objFighter->GetMove().y);
	ImGui::Text("MoveZ   :%.4f", objFighter->GetMove().z);
	ImGui::Text("cameraX :%.4f", camera->GetEye().x);
	ImGui::Text("cameraY :%.4f", camera->GetEye().y);
	ImGui::Text("cameraZ :%.4f", camera->GetEye().z);
	ImGui::Text("crystal :%d", objFighter->GetCrystal());
	ImGui::Checkbox("GoalFlag", &objFighter->GetGoalFlag());
	ImGui::Checkbox("Wall", &objFighter->GetWallHitFlag());
	ImGui::Checkbox("statmina", &objFighter->GetStaminaFlag());
	imguiManager::PosDraw();
}
