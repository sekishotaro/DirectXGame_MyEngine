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
#include "Effect2d.h"

#include "SphereCollider.h"
#include "CollisionManager.h"
#include "Player.h"
#include "UI.h"
#include "OpticalPost.h"
#include "Effect.h"

#include "SafeDelete.h"

#include "ClockTime.h"

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
	Sprite::LoadTexture(1, L"Resources/background2.png");
	// 背景スプライト生成
	spriteBG = Sprite::Create(1, { 0.0f,0.0f });

	Effect::Initialize();

	modelFighter = Model::LoadFromOBJ("modelObj");
	skydomeModel = Model::LoadFromOBJ("skydome");


	//json
	JsonLoader::LoadFile("Scene11_12"); //オブジェクトの当たり判定

	JsonLoader::SetObject();

	//モデル名を指定してファイル読み込み
	fbxModel = FbxLoader::GetInstance()->LoadModelFromFile("model");

	collisionManager = CollisionManager::GetInstance();
	objFighter = Player::Create(fbxModel);
	skydomeObject = Object3d::Create();
	skydomeObject->SetModel(skydomeModel);
	skydomeObject->SetScale({ 5.0f, 5.0f, 5.0f });
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

	//ライト
	lightGroup = LightGroup::Create();
	lightGroup->SetDirLightColor(0,{ 1,1,1 });
	Object3d::SetLight(lightGroup);
	lightGroup->SetCircleShadowActive(0, true);
	lightGroup->SetCircleShadowActive(1, true);
}

void GamePlayScene::Finalize()
{
	safe_delete(camera);
	safe_delete(modelFighter);
	safe_delete(spriteBG);
	safe_delete(lightGroup);
	JsonLoader::Finalize();
	OpticalPost::Finalize();
	Effect::Finalize();
	Enemy::Finalize();
}

void GamePlayScene::Update()
{
	//キーボード、マウスのインプット
	Input *input = Input::GetInstance();
	Input::MousePos mpos = input->MousePosLoad();

	lightGroup->SetCircleShadowDir(0, XMVECTOR({ circleShadowDir[0], circleShadowDir[1], circleShadowDir[2], 0 }));
	lightGroup->SetCircleShadowCasterPos(0, XMFLOAT3(objFighter->GetPos()));
	lightGroup->SetCircleShadowAtten(0, XMFLOAT3(circleShadowAtten));
	lightGroup->SetCircleShadowFactorAngle(0, XMFLOAT2(circleShadowFactorAngle));

	lightGroup->SetCircleShadowDir(1, XMVECTOR({ circleShadowDir[0], circleShadowDir[1], circleShadowDir[2], 0 }));
	lightGroup->SetCircleShadowCasterPos(1, XMFLOAT3(JsonLoader::raidEnemyObjects[0].get()->GetPosition()));
	lightGroup->SetCircleShadowAtten(1, XMFLOAT3(circleShadowAtten));
	lightGroup->SetCircleShadowFactorAngle(1, XMFLOAT2(circleShadowFactorAngle2));

	int state = 2;
	switch(state)
	{
		 case 1:
			 StartStatus();
			 return;
		 case 2:
			 GameStatus();
			 return;
		 case 3:
			 GameOverStatus();
			 return;
		 case 4:
			 ClearStatus();
			 return;
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
	skydomeObject->Draw();
	
	objFighter->Draw(cmdList);
	//json
	JsonLoader::Draw();
	
	for (int i = 0; i < enemyColliderObjects.size(); i++)
	{
		enemyColliderObjects[i].get()->Draw();
	}

	OpticalPost::Draw();

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	// 3Dオブジェクト描画後処理
	Object3d::PostDraw();
	ColliderObject::PostDraw();
	MathObject::PostDraw();
	// 前景スプライト描画前処理
	Sprite::PreDraw(cmdList);
	Effect2d::PreDraw(cmdList);

	if (moveFlag != true)
	{
		UI::Draw();
	}
	
	Effect::Draw();

	// デバッグテキストの描画
	DebugText::GetInstance()->DrawAll(cmdList);

	// スプライト描画後処理
	Sprite::PostDraw();
	Effect2d::PostDraw();
	//imguiの描画
	imguiManager::PraDraw();
	ImGui::NewFrame();
	ImGui::Begin("config1");//ウィンドウの名前
	ImGui::SetWindowSize(ImVec2(400, 500), ImGuiCond_::ImGuiCond_FirstUseEver);
	
	ImGui::Text("Time: %d", (int)ClockTime::GetSec());
	ImGui::Text("inputX: %d", objFighter->GetInputNumX());
	ImGui::Text("inputY: %d", objFighter->GetInputNumY());
	ImGui::Text("testRota: %.4f", OpticalPost::GetNum());
	ImGui::Text("TimeLimit: %.4f", objFighter->GetTimeLimit());
	ImGui::Text("PosX    :%.4f", objFighter->GetPosition().x);
	ImGui::Text("PosY    :%.4f", objFighter->GetPosition().y);
	ImGui::Text("PosZ    :%.4f", objFighter->GetPosition().z);
	ImGui::Text("RotaX   :%.4f", objFighter->GetRotation().x);
	ImGui::Text("RotaY   :%.4f", objFighter->GetRotation().y);
	ImGui::Text("RotaZ   :%.4f", objFighter->GetRotation().z);
	ImGui::Text("cameraX :%.4f", camera->GetEye().x);
	ImGui::Text("cameraY :%.4f", camera->GetEye().y);
	ImGui::Text("cameraZ :%.4f", camera->GetEye().z);
	ImGui::Text("crystal :%d", objFighter->GetCrystal());
	ImGui::Checkbox("GoalFlag", &objFighter->GetGoalFlag());
	ImGui::Checkbox("Wall", &objFighter->GetWallHitFlag());
	ImGui::Checkbox("statmina", &objFighter->GetStaminaFlag());
	imguiManager::PosDraw();
}

void GamePlayScene::ObjectsUpdate()
{
	//プレーヤーオブジェクトのアップデート
	objFighter->ObjectUpdate();
	//Json読み込みのオブジェクトのアップデート
	JsonLoader::Update();
	//光の柱オブジェクトのアップデート
	OpticalPost::Update(camera->GetEye());
	//スカイドーム
	skydomeObject->Update();
	//カメラ
	camera->Update();
}

void GamePlayScene::StartStatus()
{
	//開始処理
	if(count <= 10)
	{
		moveFlag = true;
		if (ClockTime::GetAddSecFlag() == true)
		{
			count++;
		}
		interpolationCamera.StartInterpolationCamera(camera);
		ObjectsUpdate();
		return;
	}
	else if(count >= 11 && count <= 109)
	{
		count = 110;
		moveFlag = false;
	}
}

void GamePlayScene::GameStatus()
{
	//アップデート
	Enemy::Update((int)objFighter->GetTimeLimit(), objFighter->GetPos());
	JsonLoader::Update();

	UI::Update();

	OpticalPost::Update(camera->GetEye());

	camera->Update();
	lightGroup->Update();
	objFighter->Update();

	OpticalPost::SetDrawFlag(true);

	//test追加探索敵コライダー
	for (int i = 0; i < enemyColliderObjects.size(); i++)
	{
		enemyColliderObjects[i].get()->SetScale(Enemy::MonitoringCollisionScale());
		enemyColliderObjects[i].get()->Update();
	}

	skydomeObject->Update();

	//UI更新
	DebugText::GetInstance()->Print(1000, 20, 3, "TIME : %d", (int)objFighter->GetTimeLimit());
	//DebugText::GetInstance()->Print(910, 80, 3, "CRYSTAL : %d/7", objFighter->GetCrystal());

	//全ての衝突をチェック
	collisionManager->CheckAllCollisions();
	Effect::Update(camera->GetEye());
}

void GamePlayScene::GameOverStatus()
{
	if (Enemy::GetGameOver() == true)
	{
		if (ClockTime::GetAddSecFlag() == true)
		{
			count++;
		}
		moveFlag = true;
		interpolationCamera.EndInterpolationCamera(camera);
		ObjectsUpdate();
		if (count >= 115)
		{
			SceneManager::GetInstance()->ChangeScene("GAMEOVER");
		}
	}
}

void GamePlayScene::ClearStatus()
{
	//ゲーム終了処理
	//static int count = 0;
	if (objFighter->GetCrystal() == 0 && objFighter->GetGoalFlag() == true)
	{
		if (ClockTime::GetAddSecFlag() == true)
		{
			count++;
		}
		moveFlag = true;
		interpolationCamera.EndInterpolationCamera(camera);
		ObjectsUpdate();
		if (count >= 115)
		{
			SceneManager::GetInstance()->ChangeScene("TITLE");
		}
		return;
	}
}
