#include "TitleScene.h"
#include "SceneManager.h"
#include "Audio.h"
#include "Input.h"
#include "DebugText.h"
#include "DirectXCommon.h"
#include "SafeDelete.h"

void TitleScene::Initialize()
{
	// テクスチャ読み込み
	Sprite::LoadTexture(1, L"Resources/TitleBackground.png");
	Sprite::LoadTexture(2, L"Resources/UI_StartKey.png");
	// 背景スプライト生成
	spriteBG = Sprite::Create(1, { 0.0f,0.0f });
	spriteUI = Sprite::Create(2, { (float)WinApp::window_width / 2.0f,(float)WinApp::window_height * 2.5f / 3.0f });
	spriteUI->SetAnchorPoint({ 0.5f, 0.5f });
	spriteUI->SetPosition({ (float)WinApp::window_width / 2.0f,(float)WinApp::window_height * 2.5f / 3.0f });

	// カメラ生成
	camera = new DebugCamera(WinApp::window_width, WinApp::window_height);
	// カメラセット
	Object3d::SetCamera(camera);
	camera->SetEye({ 0, 100, -30 });

	//デバイスのセット
	FbxObject3d::SetDevice(DirectXCommon::GetInstance()->GetDev());
	FbxObject3d::SetCamera(camera);
	//グラフィックスパイプライン生成
	FbxObject3d::CreateGraphicsPipeline();

	//ライト
	lightGroup = LightGroup::Create();
	lightGroup->SetDirLightColor(0, { 1,1,1 });
	Object3d::SetLight(lightGroup);
	lightGroup->SetCircleShadowActive(0, true);
	lightGroup->SetCircleShadowActive(1, true);

	skydomeModel = Model::LoadFromOBJ("skydome");
	skydomeObject = Object3d::Create();
	skydomeObject->SetModel(skydomeModel);
	skydomeObject->SetScale({ 5.0f, 5.0f, 5.0f });

	terrainModel = Model::LoadFromOBJ("terrain");
	terrainObject = Object3d::Create();
	terrainObject->SetModel(terrainModel);
	terrainObject->SetScale({ 1.0f, 1.0f, 1.0f });


	fbxModel = FbxLoader::GetInstance()->LoadModelFromFile("model");
	fbxObject = new FbxObject3d();
	fbxObject->Initialize();
	fbxObject->SetModel(fbxModel);
	fbxObject->SetScale({ 0.01f ,0.01f ,0.01f });
}

void TitleScene::Finalize()
{
	safe_delete(camera);
	safe_delete(spriteBG);
	safe_delete(lightGroup);
}

void TitleScene::Update()
{
	// ゲームシーンの毎フレーム処理

	Input *input = Input::GetInstance();
	lightGroup->SetCircleShadowDir(0, XMVECTOR({ circleShadowDir[0], circleShadowDir[1], circleShadowDir[2], 0 }));
	lightGroup->SetCircleShadowAtten(0, XMFLOAT3(circleShadowAtten));
	lightGroup->SetCircleShadowFactorAngle(0, XMFLOAT2(circleShadowFactorAngle));

	lightGroup->SetCircleShadowDir(1, XMVECTOR({ circleShadowDir[0], circleShadowDir[1], circleShadowDir[2], 0 }));
	lightGroup->SetCircleShadowAtten(1, XMFLOAT3(circleShadowAtten));
	lightGroup->SetCircleShadowFactorAngle(1, XMFLOAT2(circleShadowFactorAngle2));

	skydomeObject->Update();
	terrainObject->Update();

	camera->rotaX += 1.0f;
	camera->Update();
	fbxObject->Update();

	if (input->TriggerKey(DIK_RETURN) || input->PushPadbutton(Button_A))
	{
		//シーン切り替え
		SceneManager::GetInstance()->ChangeScene("GAMEPLAY");
	}
}

void TitleScene::Draw()
{
	// ゲームシーンの描画

// コマンドリストの取得
	ID3D12GraphicsCommandList *cmdList = DirectXCommon::GetInstance()->GetCmdList();

	// 背景スプライト描画前処理
	Sprite::PreDraw(cmdList);
	// 背景スプライト描画
	spriteUI->Draw();
	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	DirectXCommon::GetInstance()->ClearDepthBuffer();

	// 3Dオブジェクト描画前処理
	Object3d::PreDraw(cmdList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	skydomeObject->Draw();
	terrainObject->Draw();

	fbxObject->Draw(cmdList);

	// 3Dオブジェクト描画後処理
	Object3d::PostDraw();

	// 前景スプライト描画前処理
	Sprite::PreDraw(cmdList);

	spriteBG->Draw();
	// デバッグテキストの描画
	DebugText::GetInstance()->DrawAll(cmdList);

	// スプライト描画後処理
	Sprite::PostDraw();
}
