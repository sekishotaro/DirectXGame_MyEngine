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
#include "Effect2d.h"

#include "SphereCollider.h"
#include "CollisionManager.h"
#include "Player.h"
#include "SafeDelete.h"

void GamePlayScene::Initialize()
{
	// カメラ生成
	camera = new GameCamera(WinApp::window_width, WinApp::window_height);
	//camera = new Camera(WinApp::window_width, WinApp::window_height);
	//デバイスのセット
	FbxObject3d::SetDevice(DirectXCommon::GetInstance()->GetDev());
	// カメラセット
	Object3d::SetCamera(camera);
	ColliderObject::SetCamera(camera);
	MathObject::SetCamera(camera);
	//dCamera->SetEye({ 0, 0, 100 });			//prinding時
	camera->SetEye({ 0, 0, -30 });
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
	Sprite::LoadTexture(99, L"Resources/smoke.png");
	Sprite::LoadTexture(98, L"Resources/smoke2.png");
	// 背景スプライト生成
	spriteBG = Sprite::Create(1, { 0.0f,0.0f });
	smokes[0] = Sprite::Create(99, { 0.0f,0.0f });
	smokes[0]->SetColor({ 1.0f,1.0f,1.0f, 0.0f });
	smokes[1] = Sprite::Create(98, { 0.0f,0.0f });
	smokes[1]->SetColor({ 1.0f,1.0f,1.0f, 0.0f });
	skydomeModel = Model::LoadFromOBJ("skydome");
	skydomeObject = Object3d::Create();
	skydomeObject->SetModel(skydomeModel);
	skydomeObject->SetScale({ 5.0f, 5.0f, 5.0f });
	//json
	JsonLoader::LoadFile("Scene7_6"); //オブジェクトの当たり判定
	JsonLoader::SetObject();

	////モデル名を指定してファイル読み込み
	fbxModels[0] = FbxLoader::GetInstance()->LoadModelFromFile("model");
	fbxModels[1] = FbxLoader::GetInstance()->LoadModelFromFile("model2");
	fbxModels[2] = FbxLoader::GetInstance()->LoadModelFromFile("model3");
	fbxModels[3] = FbxLoader::GetInstance()->LoadModelFromFile("model4");
	fbxModels[4] = FbxLoader::GetInstance()->LoadModelFromFile("model5");
	fbxModels[5] = FbxLoader::GetInstance()->LoadModelFromFile("model6");
	fbxModels[6] = FbxLoader::GetInstance()->LoadModelFromFile("model7");
	fbxModels[7] = FbxLoader::GetInstance()->LoadModelFromFile("model8");
	fbxModels[8] = FbxLoader::GetInstance()->LoadModelFromFile("model9");
	fbxModels[9] = FbxLoader::GetInstance()->LoadModelFromFile("model10");
	fbxModels[10] = FbxLoader::GetInstance()->LoadModelFromFile("model11");
	fbxModels[11] = FbxLoader::GetInstance()->LoadModelFromFile("model12");
	fbxModels[12] = FbxLoader::GetInstance()->LoadModelFromFile("model13");
	fbxModels[13] = FbxLoader::GetInstance()->LoadModelFromFile("model14");
	fbxModels[14] = FbxLoader::GetInstance()->LoadModelFromFile("model15");

	collisionManager = CollisionManager::GetInstance();
	objFighter = Player::Create(fbxModels[0]);
	objFighter->SetModel1(fbxModels[0]);
	objFighter->SetModel2(fbxModels[1]);
	objFighter->SetModel3(fbxModels[2]);
	objFighter->SetModel4(fbxModels[3]);
	objFighter->SetModel5(fbxModels[4]);
	objFighter->SetModel6(fbxModels[5]);
	objFighter->SetModel7(fbxModels[6]);
	objFighter->SetModel8(fbxModels[7]);
	objFighter->SetModel9(fbxModels[8]);
	objFighter->SetModel10(fbxModels[9]);
	objFighter->SetModel11(fbxModels[10]);
	objFighter->SetModel12(fbxModels[11]);
	objFighter->SetModel13(fbxModels[12]);
	objFighter->SetModel14(fbxModels[13]);
	objFighter->SetModel15(fbxModels[14]);
	
	effect->Effect::Initialize();
	
	staminaUI->StaminaUI::Initialize();
	timeUI->TimeUI::Initialize();
	controllerUI->ControllerUI::Initialize();
	crystalUI->CrystalUI::Initialize();

	opticalPost->OpticalPost::Initialize();
}

void GamePlayScene::Finalize()
{
	safe_delete(camera);
	safe_delete(spriteBG);
	safe_delete(lightGroup);
	JsonLoader::Finalize();
	opticalPost->Finalize();
	safe_delete(opticalPost);
	effect->Finalize();
	safe_delete(effect);
	safe_delete(staminaUI);
	safe_delete(timeUI);
	safe_delete(controllerUI);
	safe_delete(crystalUI);
}

void GamePlayScene::Update()
{
	//キーボード、マウスのインプット
	Input *input = Input::GetInstance();
	Input::MousePos mpos = input->MousePosLoad();

	lightGroup->SetCircleShadowDir(0, XMVECTOR({ circleShadowDir[0], circleShadowDir[1], circleShadowDir[2], 0 }));
	lightGroup->SetCircleShadowCasterPos(0, XMFLOAT3(objFighter->GetPosition()));
	lightGroup->SetCircleShadowAtten(0, XMFLOAT3(circleShadowAtten));
	lightGroup->SetCircleShadowFactorAngle(0, XMFLOAT2(circleShadowFactorAngle));

	lightGroup->SetCircleShadowDir(1, XMVECTOR({ circleShadowDir[0], circleShadowDir[1], circleShadowDir[2], 0 }));
	lightGroup->SetCircleShadowAtten(1, XMFLOAT3(circleShadowAtten));
	lightGroup->SetCircleShadowFactorAngle(1, XMFLOAT2(circleShadowFactorAngle2));

	SmokeUpdate();
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
	
	opticalPost->Draw();

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
	smokes[0]->Draw();
	smokes[1]->Draw();
	if (moveFlag != true)
	{
		staminaUI->StaminaUI::Draw(objFighter);
		timeUI->TimeUI::Draw(objFighter);
		controllerUI->ControllerUI::Draw(objFighter);
		crystalUI->CrystalUI::Draw(objFighter);
	}
	
	effect->Draw();
	
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
	ImGui::Text("rotY    :%f", objFighter->GetRotation().y);
	ImGui::Text("cameraY :%f", camera->rota.x);
	ImGui::Text("playerState :%d", static_cast<int>(objFighter->GetStatus()));
	ImGui::Checkbox("cameraAutoX", &camera->rotaAuto);
	ImGui::Checkbox("teleport", &objFighter->teleportFlag);
	ImGui::Checkbox("TimeLimitCancel", &objFighter->timeLimitcancel);
	imguiManager::PosDraw();
}

void GamePlayScene::ObjectsUpdate()
{
	//プレーヤーオブジェクトのアップデート
	objFighter->ObjectUpdate();
	//Json読み込みのオブジェクトのアップデート
	JsonLoader::Update();
	//光の柱オブジェクトのアップデート
	opticalPost->Update(camera->GetEye(), objFighter);
	//スカイドーム
	skydomeObject->Update();
	//カメラ
	camera->Update(objFighter);
}

void GamePlayScene::StartStatus()
{
}

void GamePlayScene::GameStatus()
{
	//アップデート
	objFighter->Update();
	camera->Update(objFighter);
	lightGroup->Update();
	skydomeObject->Update();
	JsonLoader::Update();
	opticalPost->Erase(objFighter->GetCrystalGetFlag(), objFighter->GetCrystalGetNum());
	opticalPost->Update(camera->GetEye(), objFighter);
	opticalPost->SetDrawFlag(true);
	
	staminaUI->StaminaUI::Update(objFighter);
	timeUI->TimeUI::Update(objFighter);
	controllerUI->ControllerUI::Update(objFighter);
	crystalUI->CrystalUI::Update(objFighter);

	effect->Update(camera->GetEye(), objFighter);
}

void GamePlayScene::GameOverStatus()
{
	const float timeMax = 3.0f;
	static float time = timeMax;
	float alpha = (timeMax - time) / timeMax;
	if (objFighter->GetTimeLimit() <= 0.0f)
	{
		objFighter->moveLimitFlag = true;
		if (time >= 0.0f)
		{
			
			smokes[1]->SetColor({ 1.0f,1.0f,1.0f, alpha });
			time -= 1.0f / 60.0f;		
			return;
		}

		state = reStart;
		//ステージの復旧
		JsonLoader::ClystalSetObject();
		opticalPost->Restart();
		
		//自機の初期化
		objFighter->ReStart();
		camera->rota.x = 180.0f;
		time = timeMax;
	}
}

void GamePlayScene::ClearStatus()
{
	//ゲーム終了処理
	if (objFighter->GetCrystalNum() == 0 && objFighter->GetGoalFlag() == true)
	{
		SceneManager::GetInstance()->ChangeScene("TITLE");
		return;
	}
}

void GamePlayScene::SmokeUpdate()
{
	if (smokeFlag == false)
	{
		smokes[0]->SetColor({1.0f,1.0f,1.0f,0.0f});
		return;
	}
	
	static float reStartNum = 1.0f;
	if (state == reStart)
	{
		if (reStartNum <= 0.0f)
		{
			state = play;
			reStartNum = 1.0f;
			objFighter->moveLimitFlag = false;
			return;
		}

		reStartNum -= 1.0f / 180.0f;

		smokes[0]->SetColor({ 1.0f,1.0f,1.0f, reStartNum });
		smokes[1]->SetColor({ 1.0f,1.0f,1.0f, reStartNum });
		return;
	}


	//制限時間が半分を切ったら煙が出てくる
	float halfTime = objFighter->GetTimeMax() / 2;
	if (objFighter->GetTimeLimit() > halfTime)
	{
		smokes[0]->SetColor({ 1.0f,1.0f,1.0f,0.0f });
		return;
	}

	//煙は制限時間半分から0になるまで
	float a = halfTime - objFighter->GetTimeLimit();

	float Rate = a / halfTime;

	smokes[0]->SetColor({ 1.0f,1.0f,1.0f, Rate });
}
