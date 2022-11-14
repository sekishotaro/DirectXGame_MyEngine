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
	JsonLoader::LoadFile("Scene11_01"); //�I�u�W�F�N�g�̓����蔻��

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
	//�L�[�{�[�h�A�}�E�X�̃C���v�b�g
	Input *input = Input::GetInstance();
	Input::MousePos mpos = input->MousePosLoad();

	//�Q�[���I������
	if (objFighter->GetCrystal() == 0 && objFighter->GetGoalFlag() == true)
	{
		SceneManager::GetInstance()->ChangeScene("TITLE");
	}

	//�A�b�v�f�[�g
	JsonLoader::Update();

	UI::Update();

	camera->Update();
	objFighter->Update();

	//UI�X�V
	DebugText::GetInstance()->Print(1000, 20, 3, "TIME : %d", (int)objFighter->GetTimeLimit());
	DebugText::GetInstance()->Print(940, 80, 3, "CRYSTAL : %d", objFighter->GetCrystal());

	//�S�Ă̏Փ˂��`�F�b�N
	collisionManager->CheckAllCollisions();
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
	
	objFighter->Draw();
	//json
	JsonLoader::Draw();

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
