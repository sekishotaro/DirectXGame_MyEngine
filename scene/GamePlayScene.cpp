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
#include "Light.h"


void GamePlayScene::Initialize()
{
	Audio::GetInstance()->LoadWave("futta-dream.wav");
	Audio::GetInstance()->LoadWave("zaza.wav");
	Audio::GetInstance()->LoadWave("What.wav");

	// カメラ生成
	camera = new DebugCamera(WinApp::window_width, WinApp::window_height);
	//camera = new Camera(WinApp::window_width, WinApp::window_height);
	//ライト生成
	light = new Light(WinApp::window_width, WinApp::window_height);

	//デバイスのセット
	FbxObject3d::SetDevice(DirectXCommon::GetInstance()->GetDev());

	// カメラセット
	Object3d::SetCamera(camera);
	ColliderObject::SetCamera(camera);

	//ライトのセット
	//light->SetPosition(camera->GetEye());
	//light->SetTarget(camera->GetTarget());
	Object3d::SetLight(light);

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

	modelFighter = Model::LoadFromOBJ("modelOb1");

	//モデル名を指定してファイル読み込み
	//fbxModel1 = FbxLoader::GetInstance()->LoadModelFromFile("model");
	//fbxModel2 = FbxLoader::GetInstance()->LoadModelFromFile("Enemy1");

	//3Dオブジェクト生成とモデルのセット
	//fbxObject1 = new FbxObject3d;
	//fbxObject1->Initialize();
	//fbxObject1->SetModel(fbxModel1);
	//fbxObject1->SetScale({ 0.01f,0.01f,0.01f });

	//fbxObject2 = new FbxObject3d;
	//fbxObject2->Initialize();
	//fbxObject2->SetModel(fbxModel2);
	//fbxObject2->SetPosition(Enemy::GetPos());
	//fbxObject2->SetScale({ 0.01f,0.01f,0.01f });


	//colliderModel = ColliderModel::ColliderModelCreate("BOX");
	//colliderObject = ColliderObject::Create();
	//colliderObject->SetModel(colliderModel);

	//colliderObject->SetCenter({ 0, 2.5f, 0 });
	//colliderObject->SetScale(Player::GetSize());
	
	//json
	JsonLoader::LoadFile("Scene10_18"); //オブジェクトの当たり判定

	//JsonLoader::LoadFile("Scene9_27"); //メッシュコライダー
	
	JsonLoader::SetObject();

	//Enemy::Initialize();
	//mathModel = MathModel::LoadFromOBJ("sphere");
	//enemyCollider1Object = MathObject::Create();
	//enemyCollider2Object = MathObject::Create();

	////オブジェクトにモデルをひも付ける
	//enemyCollider1Object->SetModel(mathModel);
	//enemyCollider1Object->SetPosition(XMFLOAT3(0, 5, 0));

	//enemyCollider2Object->SetModel(mathModel);
	//enemyCollider2Object->SetPosition(XMFLOAT3(0, 5, 0));
	//enemyCollider2Object->SetScale(XMFLOAT3(20, 20, 20));
	//enemyCollider2Object->SetColor(XMFLOAT4(0, 0, 1, 0.2));

	collisionManager = CollisionManager::GetInstance();
	objFighter = Player::Create(modelFighter);
}

void GamePlayScene::Finalize()
{
	//delete model;
	//delete fbxObject1;
	//delete fbxObject2;
}

void GamePlayScene::Update()
{
	//キーボード、マウスのインプット
	Input *input = Input::GetInstance();
	Input::MousePos mpos = input->MousePosLoad();


	XMFLOAT3 cameraPos = objFighter->GetPosition();
	static XMFLOAT3 pos = {0, 5.0f, -10.0f};

	cameraPos.x += pos.x;
	cameraPos.y += pos.y;
	cameraPos.z += pos.z;

	camera->SetEye(cameraPos);
	//カメラの移動
	if (input->PushKey(DIK_UP) || input->PushKey(DIK_DOWN) || input->PushKey(DIK_RIGHT) || input->PushKey(DIK_LEFT) || input->PushKey(DIK_E) || input->PushKey(DIK_Z))
	{
		// 現在の座標を取得
		//XMFLOAT3 position = camera->GetEye();

		// 移動後の座標を計算
		if (input->PushKey(DIK_UP)) { pos.y += 1.0f; }
		else if (input->PushKey(DIK_DOWN)) { pos.y -= 1.0f; }
		if (input->PushKey(DIK_RIGHT)) { pos.x += 1.0f; }
		else if (input->PushKey(DIK_LEFT)) { pos.x -= 1.0f; }
		if (input->PushKey(DIK_E)) { pos.z += 1.0f; }
		else if (input->PushKey(DIK_Z)) { pos.z -= 1.0f; }
	}

	//プレイヤーの移動

	//プレイヤーと地面の当たり判定処理
	//if (Player::GetWallColl() == true)
	//{
	//	colliderObject->SetColor({ 1, 1, 0});
	//}
	//else if (Player::GetWallColl() == false)
	//{
	//	colliderObject->SetColor({ 1, 0, 0});
	//}

	//if (input->PushKey(DIK_G))
	//{
	//	Enemy::Move(Player::GetPos());
	//}

	//if (input->PushKey(DIK_I))
	//{
	//	XMFLOAT3 pos = Enemy::GetPos();
	//	pos.y += 1.0f;
	//	Enemy::SetPos(pos);
	//}

	////自機の当たり判定用BOXの設定
	//Pbox.centerPos = Player::GetPos();
	//Pbox.LeastPos = XMFLOAT3(Player::GetPos().x - Player::GetSize().x /2, Player::GetPos().y, Player::GetPos().z - Player::GetSize().z / 2);
	//Pbox.MaxPos = XMFLOAT3(Player::GetPos().x + Player::GetSize().x / 2, Player::GetPos().y + Player::GetSize().y, Player::GetPos().z + Player::GetSize().z / 2);

	////敵の当たり判定計算用の球の初期化
	//SphereF Esphere;
	//Esphere.center = Enemy::GetPos();
	//Esphere.radius = 2;

	//SphereF E2sphere;
	//E2sphere.center = Enemy::GetPos();
	//E2sphere.radius = 20;

	////自機と敵の線分
	//LineSegment line;
	//line.start = Enemy::GetPos();
	//line.end = Player::GetPos();

	////壁との当たり判定計算用のBoxの初期化
	//Box wall;
	//wall.centerPos = JsonLoader::colliderObjects[0].get()->GetPosition();
	//wall.size = JsonLoader::colliderObjects[0].get()->GetScale();
	//wall.LeastPos = { wall.centerPos.x - wall.size.x / 2,wall.centerPos.y - wall.size.y / 2, wall.centerPos.z - wall.size.z / 2};
	//wall.MaxPos = { wall.centerPos.x + wall.size.x / 2,wall.centerPos.y + wall.size.y / 2, wall.centerPos.z + wall.size.z /2 };

	//if (Collision::CheckLineSegmentBox(line, wall) == true)
	//{
	//	enemyCollider1Object->SetColor({ 1, 1, 0, 0.5f });
	//}
	//else
	//{
	//	enemyCollider1Object->SetColor({ 1, 1, 1, 0.5f });
	//}

	//if (Collision::CheckSphereBox(E2sphere, Pbox) == true)
	//{
	//	Enemy::Move(Player::GetPos());
	//}


	//if (Collision::CheckSphereBox(Esphere, Pbox) == true)
	//{
	//	colliderObject->SetColor({ 1, 0, 0 });
	//}
	//else
	//{
	//	colliderObject->SetColor({ 1, 1, 1 });
	//}
	//DebugText::GetInstance()->Print(50, 30 * 8, 2, "M:Y:%f", Player::GetMove().y);
	//DebugText::GetInstance()->Print(50, 30 * 9, 2, "M:Z:%f", Player::GetMove().z);
	
	//if (input->TriggerKey(DIK_SPACE))
	//{
	//	//BGM止める
	//	Audio::GetInstance()->SoundStop("zaza.wav");
	//	Audio::GetInstance()->PlayWave("zaza.wav", false);
	//	
	//	//シーン切り替え
	//	//SceneManager::GetInstance()->ChangeScene("TITLE");
	//}	
	//DebugText::GetInstance()->Print(50, 30 * 1, 2, "Camera:X:%f", camera->GetEye().x);
	//DebugText::GetInstance()->Print(50, 30 * 2, 2, "Camera:Y:%f", camera->GetEye().y);
	//DebugText::GetInstance()->Print(50, 30 * 3, 2, "Camera:Z:%f", camera->GetEye().z);
	//DebugText::GetInstance()->Print(50, 30 * 4, 2, "Player:X:%f", objFighter->GetPosition().x);
	//DebugText::GetInstance()->Print(50, 30 * 5, 2, "Player:Y:%f", objFighter->GetPosition().y);
	//DebugText::GetInstance()->Print(50, 30 * 6, 2, "Player:Z:%f", objFighter->GetPosition().z);
	
	//Ray ray;
	//ray.start = { 10.0f, 0.5f, 0.0f, 1 };
	//ray.dir = { 0,-1,0,0 };
	//RaycastHit raycastHit;

	//if (collisionManager->Raycast(ray, &raycastHit)) {
	//	DebugText::GetInstance()->Print(50, 30 * 4, 2, "Raycast Hit.");
	//}


	//描画オブジェクト関連の更新
	//enemyCollider1Object->SetPosition(Enemy::GetPos());
	//enemyCollider2Object->SetPosition(Enemy::GetPos());
	//fbxObject1->SetPosition(Player::GetPos());
	//colliderObject->SetPosition(Player::GetPos());
	//fbxObject2->SetPosition(Enemy::GetPos());
	//fbxObject1->AnimationFlag = false;
	//fbxObject2->AnimationFlag = false;
	//fbxObject1->AnimationNum = 1;

	//ゲーム終了処理
	if (objFighter->GetCrystal() == 0 && objFighter->GetGoalFlag() == true)
	{
		SceneManager::GetInstance()->ChangeScene("TITLE");
	}
	
	//アップデート
	JsonLoader::Update();

	camera->Update();
	light->Update();
	objFighter->Update();

	//全ての衝突をチェック
	collisionManager->CheckAllCollisions();

	//enemyCollider1Object->Update();
	//enemyCollider2Object->Update();
	//fbxObject1->Update();
	//fbxObject2->Update();
	//colliderObject->Update();
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

	//FBX3Dオブジェクトの描画
	//fbxObject1->Draw(cmdList);
	//fbxObject2->Draw(cmdList);
	//colliderObject->Draw();
	//enemyCollider1Object->Draw();
	//enemyCollider2Object->Draw();
	
	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	// 3Dオブジェクト描画後処理
	Object3d::PostDraw();
	ColliderObject::PostDraw();
	MathObject::PostDraw();
	// 前景スプライト描画前処理
	Sprite::PreDraw(cmdList);

	// デバッグテキストの描画
	DebugText::GetInstance()->DrawAll(cmdList);

	//ポストエフェクト
	// スプライト描画後処理
	Sprite::PostDraw();

	//imguiの描画
	imguiManager::PraDraw();
	ImGui::NewFrame();
	ImGui::Begin("config1");//ウィンドウの名前
	ImGui::SetWindowSize(ImVec2(400, 500), ImGuiCond_::ImGuiCond_FirstUseEver);
	
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
	imguiManager::PosDraw();
}

void GamePlayScene::ShadowDraw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* cmdList = DirectXCommon::GetInstance()->GetCmdList();
	
	// 深度バッファクリア
	DirectXCommon::GetInstance()->ClearDepthBuffer();

	// 3Dオブジェクト描画前処理
	Object3d::PreDraw(cmdList);
	ColliderObject::PreDraw(cmdList);
	MathObject::PreDraw(cmdList);

	// 3Dオブクジェクトの描画
	objFighter->ShadowDraw();
	//json
	JsonLoader::ShadowDraw();
	//FBX3Dオブジェクトの描画

	// 3Dオブジェクト描画後処理
	Object3d::PostDraw();
	ColliderObject::PostDraw();
	MathObject::PostDraw();
}
