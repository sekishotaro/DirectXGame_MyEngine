#include "Framework.h"

#include "SafeDelete.h"
#include "FbxLoader.h"
#include "imguiManager.h"


void Framework::Initialize()
{
	//WindowsAPIの初期化
	winApp = new WinApp();
	winApp->Initializa();

	//DirectXの初期化
	dxCommon = DirectXCommon::GetInstance();
	dxCommon->Initialize(winApp);

	//入力の初期化
	input = Input::GetInstance();
	input->Initialize(winApp);

	//オーディオの初期化
	audio = Audio::GetInstance();
	audio->Initialize();

	//imgui初期化
	imguiManager::Initialize();

	//スプライト静的初期化
	if (!Sprite::StaticInitialize(dxCommon->GetDev(), WinApp::window_width, WinApp::window_height))
	{
		assert(0);
	}

	// デバックテキスト
	Sprite::LoadTexture(debugTextTexNumber, L"Resources/ASCII.png");
	DebugText::GetInstance()->Initialize(debugTextTexNumber);

	camera = new Camera(WinApp::window_width, WinApp::window_height);

	//3Dオブジェクト静的初期化
	if (!Object3d::StaticInitialize(dxCommon->GetDev(), camera))
	{
		assert(0);
	}

	if (!MathObject::StaticInitialize(dxCommon->GetDev(), camera))
	{
		assert(0);
	}

	//3Dオブジェクト静的初期化
	if (!ColliderObject::StaticInitialize(dxCommon->GetDev(), camera))
	{
		assert(0);
	}

	FbxLoader::GetInstance()->Initialize(DirectXCommon::GetInstance()->GetDev());


	//シャドウマップ
	shadowMap = new ShadowMap();
	shadowMap->Initialize();
}

void Framework::Finalize()
{
	//オーディオ開放
	audio->Finalize();
	//デバックテキスト開放

	// ゲームウィンドウの破棄
	winApp->Finalize();
	safe_delete(winApp);

	FbxLoader::GetInstance()->Finalize();

	delete shadowMap;
}

void Framework::Update()
{
	// メッセージ処理
	if (winApp->ProcessMessage())
	{
		// ゲームループを抜ける
		endRequest_ = true;
		return;
	}

	// 入力関連の毎フレーム処理
	input->Update();

	//シーンの更新
	SceneManager::GetInstance()->Update();
}

void Framework::Draw()
{
	//レンダリング
	shadowMap->PreDrawScene(dxCommon->GetCmdList());
	SceneManager::GetInstance()->ShadowDraw();
	shadowMap->PostDrawScene(dxCommon->GetCmdList());

	shadowMap->rootSignature->GetDevice();

	// 描画開始
	dxCommon->PreDraw();
	// コマンドリストの取得
	//postEffect->Draw(dxCommon->GetCmdList()); //レンダリングの描画処理
	
	//SceneManager::GetInstance()->Draw();
	shadowMap->Draw(dxCommon->GetCmdList());

	//SceneManager::GetInstance()->Draw();

	// 描画終了
	dxCommon->PostDraw();
}

void Framework::Run()
{
	//ゲームの初期化
	Initialize();

	// メインループ
	while (true)
	{
		//毎フレーム更新
		Update();
		//終了リクエストが来たら抜ける
		if (IsEndRequest())
		{
			break;
		}

		//描画
		Draw();
	}

	//ゲームの終了
	Finalize();
}