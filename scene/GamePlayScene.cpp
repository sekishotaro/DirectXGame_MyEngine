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

	// �J��������
	camera = new DebugCamera(WinApp::window_width, WinApp::window_height);
	//camera = new Camera(WinApp::window_width, WinApp::window_height);

	//�f�o�C�X�̃Z�b�g
	FbxObject3d::SetDevice(DirectXCommon::GetInstance()->GetDev());

	// �J�����Z�b�g
	Object3d::SetCamera(camera);
	ColliderObject::SetCamera(camera);

	MathObject::SetCamera(camera);
	//dCamera->SetEye({ 0, 0, 100 });			//prinding��
	camera->SetEye({ 0, 0, -30 });		//prin��
	FbxObject3d::SetCamera(camera);

	//�O���t�B�b�N�X�p�C�v���C������
	FbxObject3d::CreateGraphicsPipeline();

	// �e�N�X�`���ǂݍ���
	Sprite::LoadTexture(1, L"Resources/background.png");
	// �w�i�X�v���C�g����
	spriteBG = Sprite::Create(1, { 0.0f,0.0f });

	modelFighter = Model::LoadFromOBJ("modelObj");
	
	//json
	JsonLoader::LoadFile("Scene11_12"); //�I�u�W�F�N�g�̓����蔻��

	JsonLoader::SetObject();

	collisionManager = CollisionManager::GetInstance();
	objFighter = Player::Create(modelFighter);
	
	//�G������
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
	//�L�[�{�[�h�A�}�E�X�̃C���v�b�g
	Input *input = Input::GetInstance();
	Input::MousePos mpos = input->MousePosLoad();

	//�Q�[���I������
	if (objFighter->GetCrystal() == 0 && objFighter->GetGoalFlag() == true)
	{
		SceneManager::GetInstance()->ChangeScene("TITLE");
	}

	//�A�b�v�f�[�g
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


	//test�ǉ��T���G�R���C�_�[
	for (int i = 0; i < enemyColliderObjects.size(); i++)
	{
		enemyColliderObjects[i].get()->SetScale(Enemy::MonitoringCollisionScale());
		enemyColliderObjects[i].get()->Update();
	}

	//UI�X�V
	DebugText::GetInstance()->Print(1000, 20, 3, "TIME : %d", (int)objFighter->GetTimeLimit());
	DebugText::GetInstance()->Print(910, 80, 3, "CRYSTAL : %d/7", objFighter->GetCrystal());

	//�S�Ă̏Փ˂��`�F�b�N
	collisionManager->CheckAllCollisions();


	if (Enemy::GetGameOver() == true)
	{
		SceneManager::GetInstance()->ChangeScene("GAMEOVER");
	}
}

void GamePlayScene::Draw()
{

	// �Q�[���V�[���̕`��

	// �R�}���h���X�g�̎擾
	ID3D12GraphicsCommandList *cmdList = DirectXCommon::GetInstance()->GetCmdList();

	// �w�i�X�v���C�g�`��O����
	Sprite::PreDraw(cmdList);
	// �w�i�X�v���C�g�`��
	spriteBG->Draw();

	/// <summary>
	/// �����ɔw�i�X�v���C�g�̕`�揈����ǉ��ł���
	/// </summary>

	// �X�v���C�g�`��㏈��
	Sprite::PostDraw();
	// �[�x�o�b�t�@�N���A
	DirectXCommon::GetInstance()->ClearDepthBuffer();

	// 3D�I�u�W�F�N�g�`��O����
	Object3d::PreDraw(cmdList);
	ColliderObject::PreDraw(cmdList);
	MathObject::PreDraw(cmdList);

	// 3D�I�u�N�W�F�N�g�̕`��
	
	//test�ǉ��T���G�R���C�_�[
	objFighter->Draw();
	//json
	JsonLoader::Draw();

	OpticalPost::Draw();

	for (int i = 0; i < enemyColliderObjects.size(); i++)
	{
		enemyColliderObjects[i].get()->Draw();
	}
	/// <summary>
	/// ������3D�I�u�W�F�N�g�̕`�揈����ǉ��ł���
	/// </summary>

	// 3D�I�u�W�F�N�g�`��㏈��
	Object3d::PostDraw();
	ColliderObject::PostDraw();
	MathObject::PostDraw();
	// �O�i�X�v���C�g�`��O����
	Sprite::PreDraw(cmdList);
	
	UI::Draw();


	// �f�o�b�O�e�L�X�g�̕`��
	DebugText::GetInstance()->DrawAll(cmdList);

	// �X�v���C�g�`��㏈��
	Sprite::PostDraw();

	//imgui�̕`��
	imguiManager::PraDraw();
	ImGui::NewFrame();
	ImGui::Begin("config1");//�E�B���h�E�̖��O
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
