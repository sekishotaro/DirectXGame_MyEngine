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
	//ライト
	lightGroup = LightGroup::Create();
	lightGroup->SetDirLightColor(0, { 1,1,1 });
	Object3d::SetLight(lightGroup);
	lightGroup->SetCircleShadowActive(0, true);
	lightGroup->SetCircleShadowActive(1, true);


	// テクスチャ読み込み
	Sprite::LoadTexture(1, L"Resources/background2.png");
	// 背景スプライト生成
	spriteBG = Sprite::Create(1, { 0.0f,0.0f });
	skydomeModel = Model::LoadFromOBJ("skydome");
	skydomeObject = Object3d::Create();
	skydomeObject->SetModel(skydomeModel);
	skydomeObject->SetScale({ 5.0f, 5.0f, 5.0f });
	//json
	JsonLoader::LoadFile("Scene4_7"); //オブジェクトの当たり判定
	JsonLoader::SetObject();

	////モデル名を指定してファイル読み込み
	fbxModel = FbxLoader::GetInstance()->LoadModelFromFile("model");
	fbxModel2 = FbxLoader::GetInstance()->LoadModelFromFile("model2");
	fbxModel3 = FbxLoader::GetInstance()->LoadModelFromFile("model3");
	fbxModel4 = FbxLoader::GetInstance()->LoadModelFromFile("model4");
	fbxModel5 = FbxLoader::GetInstance()->LoadModelFromFile("model5");
	fbxModel6 = FbxLoader::GetInstance()->LoadModelFromFile("model6");
	fbxModel7 = FbxLoader::GetInstance()->LoadModelFromFile("model7");
	fbxModel8 = FbxLoader::GetInstance()->LoadModelFromFile("model8");
	fbxModel9 = FbxLoader::GetInstance()->LoadModelFromFile("model9");
	fbxModel10 = FbxLoader::GetInstance()->LoadModelFromFile("model10");
	fbxModel11 = FbxLoader::GetInstance()->LoadModelFromFile("model11");
	fbxModel12 = FbxLoader::GetInstance()->LoadModelFromFile("model12");
	fbxModel13 = FbxLoader::GetInstance()->LoadModelFromFile("model13");
	fbxModel14 = FbxLoader::GetInstance()->LoadModelFromFile("model14");
	fbxModel15 = FbxLoader::GetInstance()->LoadModelFromFile("model15");

	collisionManager = CollisionManager::GetInstance();
	objFighter = Player::Create(fbxModel);
	objFighter->SetModel1(fbxModel);
	objFighter->SetModel2(fbxModel2);
	objFighter->SetModel3(fbxModel3);
	objFighter->SetModel4(fbxModel4);
	objFighter->SetModel5(fbxModel5);
	objFighter->SetModel6(fbxModel6);
	objFighter->SetModel7(fbxModel7);
	objFighter->SetModel8(fbxModel8);
	objFighter->SetModel9(fbxModel9);
	objFighter->SetModel10(fbxModel10);
	objFighter->SetModel11(fbxModel11);
	objFighter->SetModel12(fbxModel12);
	objFighter->SetModel13(fbxModel13);
	objFighter->SetModel14(fbxModel14);
	objFighter->SetModel15(fbxModel15);
	////敵初期化
	//Enemy::Initialize();
	
	Effect::Initialize();
	UI::Initialize();

	OpticalPost::Initialize();
}

void GamePlayScene::Finalize()
{
	safe_delete(camera);
	safe_delete(spriteBG);
	safe_delete(lightGroup);
	JsonLoader::Finalize();
	OpticalPost::Finalize();
	Effect::Finalize();
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
	//lightGroup->SetCircleShadowCasterPos(1, XMFLOAT3(JsonLoader::raidEnemyObjects[0].get()->GetPosition()));
	lightGroup->SetCircleShadowAtten(1, XMFLOAT3(circleShadowAtten));
	lightGroup->SetCircleShadowFactorAngle(1, XMFLOAT2(circleShadowFactorAngle2));

	//int state = 2;
	//switch(state)
	//{
	//	 case 1:
	//		 StartStatus();
	//		 return;
	//	 case 2:
	//		 GameStatus();
	//		 return;
	//	 case 3:
	//		 GameOverStatus();
	//		 return;
	//	 case 4:
	//		 ClearStatus();
	//		 return;
	//}

	if (input->TriggerKey(DIK_RETURN))
	{
		//シーン切り替え
		SceneManager::GetInstance()->ChangeScene("GAMEOVER");
	}

	GameStatus();
	GameOverStatus();
	ClearStatus();
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

	//3Dオブジェクト描画前処理
	Object3d::PreDraw(cmdList);
	ColliderObject::PreDraw(cmdList);
	MathObject::PreDraw(cmdList);

	
	//3Dオブクジェクトの描画
	skydomeObject->Draw();
	//json
	JsonLoader::Draw();
	objFighter->Draw(cmdList);
	//for (int i = 0; i < enemyColliderObjects.size(); i++)
	//{
	//	enemyColliderObjects[i].get()->Draw();
	//}
	
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
	//DebugText::GetInstance()->DrawAll(cmdList);

	// スプライト描画後処理
	Sprite::PostDraw();
	Effect2d::PostDraw();
	//imguiの描画
	imguiManager::PraDraw();
	ImGui::NewFrame();
	ImGui::Begin("config1");//ウィンドウの名前
	ImGui::SetWindowSize(ImVec2(400, 500), ImGuiCond_::ImGuiCond_FirstUseEver);
	ImGui::Text("AnimeNum: %d", objFighter->GetAnimeNum());
	ImGui::Text("PlayerPos X: %f", objFighter->GetPos().x);
	ImGui::Text("PlayerPos Y: %f", objFighter->GetPos().y);
	ImGui::Text("PlayerPos Z: %f", objFighter->GetPos().z);
	ImGui::Text("PlayerRot Y: %f", objFighter->GetRotation().y);
	ImGui::Text("Camera    Y: %f",  camera->GetEye().y);
	ImGui::Text("CrystalNum: %d", JsonLoader::crystalObjects.size());
	ImGui::Text("crystal :%d", objFighter->GetCrystal());
	ImGui::Text("moveBoxMax_X :%f", objFighter->moveBoxMax1.x);
	ImGui::Text("moveBoxMax_Z :%f", objFighter->moveBoxMax1.z);
	ImGui::Checkbox("Terrain", &JsonLoader::hitTerrainDrawFlag);
	ImGui::Checkbox("teleport", &objFighter->teleportFlag);
	ImGui::Checkbox("ClimbingCliffFlag", &objFighter->GetClimbingCliffFlag());
	ImGui::Checkbox("Landing", &objFighter->GetLandingFlag());
	ImGui::Checkbox("slop", &objFighter->GetSlopeFlag());
	ImGui::Checkbox("jumpWallHittingFlag", &objFighter->GetJumpWallHitFlag());
	ImGui::Checkbox("wallHittingFlag", &objFighter->GetWallHitFlag());
	ImGui::Checkbox("wallKic", &objFighter->testFlag);
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
	camera->Update();
	lightGroup->Update();
	skydomeObject->Update();
	JsonLoader::Update();
	objFighter->Update();
	OpticalPost::Update(camera->GetEye());
	OpticalPost::SetDrawFlag(true);
	
	UI::Update();
	
	Effect::Update(camera->GetEye());
}

void GamePlayScene::GameOverStatus()
{
	if (objFighter->GetTimeLimit() <= 0.0f)
	{
		SceneManager::GetInstance()->ChangeScene("GAMEOVER");
	}
}

void GamePlayScene::ClearStatus()
{
	//ゲーム終了処理
	if (objFighter->GetCrystal() == 0 && objFighter->GetGoalFlag() == true)
	{
		SceneManager::GetInstance()->ChangeScene("TITLE");
		return;
	}
}
