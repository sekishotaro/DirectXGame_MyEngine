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
#include "OpticalPost.h"

#include "SafeDelete.h"

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
	JsonLoader::LoadFile("Scene11_12"); //オブジェクトの当たり判定

	JsonLoader::SetObject();

	collisionManager = CollisionManager::GetInstance();
	objFighter = Player::Create(modelFighter);
	
	//敵初期化
	Enemy::Initialize();

	modelEnemyCollider = MathModel::LoadFromOBJ("sphere2");
	
	
	for (int i = 0; i < JsonLoader::enemyObjects.size(); i++)
	{
		std::unique_ptr<MathObject> objectEnemyCollider;
		objectEnemyCollider = MathObject::Create();
		objectEnemyCollider->SetModel(modelEnemyCollider);
		objectEnemyCollider->SetPosition(JsonLoader::enemyObjects[i].get()->GetPosition());
		objectEnemyCollider->SetScale({ 2.0f, 2.0f, 2.0f });
		objectEnemyCollider->SetColor({ 1.0f, 1.0f, 1.0f, 0.5f });
		enemyColliderObjects.push_back(std::move(objectEnemyCollider));
	}

	UI::Initialize();

	OpticalPost::Initialize();
}

void GamePlayScene::Finalize()
{
	safe_delete(camera);
	safe_delete(modelFighter);
	safe_delete(spriteBG);

	JsonLoader::Finalize();
	OpticalPost::Finalize();
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
	Enemy::Update((int)objFighter->GetTimeLimit(), objFighter->GetPos());
	JsonLoader::Update();

	UI::Update();
	OpticalPost::Update();

	camera->Update();
	objFighter->Update();

	if (input->PushKey(DIK_1))
	{
		OpticalPost::SetDrawFlag(true);
	}
	else
	{
		OpticalPost::SetDrawFlag(false);
	}


	//test追加探索敵コライダー
	for (int i = 0; i < enemyColliderObjects.size(); i++)
	{
		enemyColliderObjects[i].get()->SetScale(Enemy::MonitoringCollisionScale());
		enemyColliderObjects[i].get()->Update();
	}

	//UI更新
	DebugText::GetInstance()->Print(1000, 20, 3, "TIME : %d", (int)objFighter->GetTimeLimit());
	DebugText::GetInstance()->Print(910, 80, 3, "CRYSTAL : %d/7", objFighter->GetCrystal());

	//全ての衝突をチェック
	collisionManager->CheckAllCollisions();


	if (Enemy::GetGameOver() == true)
	{
		SceneManager::GetInstance()->ChangeScene("GAMEOVER");
	}
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
	
	//test追加探索敵コライダー
	objFighter->Draw();
	//json
	JsonLoader::Draw();

	OpticalPost::Draw();

	for (int i = 0; i < enemyColliderObjects.size(); i++)
	{
		enemyColliderObjects[i].get()->Draw();
	}
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
	
	ImGui::Text("cameraRotX: %.4f", camera->rotaX);
	ImGui::Text("cameraRotY: %.4f", camera->rotaY);
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
