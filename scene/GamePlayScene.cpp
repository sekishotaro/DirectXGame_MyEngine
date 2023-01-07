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
	Sprite::LoadTexture(1, L"Resources/background2.png");
	// �w�i�X�v���C�g����
	spriteBG = Sprite::Create(1, { 0.0f,0.0f });

	Effect::Initialize();

	modelFighter = Model::LoadFromOBJ("modelObj");
	skydomeModel = Model::LoadFromOBJ("skydome");


	//json
	JsonLoader::LoadFile("Scene11_12"); //�I�u�W�F�N�g�̓����蔻��

	JsonLoader::SetObject();

	//���f�������w�肵�ăt�@�C���ǂݍ���
	fbxModel = FbxLoader::GetInstance()->LoadModelFromFile("model");

	collisionManager = CollisionManager::GetInstance();
	objFighter = Player::Create(fbxModel);
	skydomeObject = Object3d::Create();
	skydomeObject->SetModel(skydomeModel);
	skydomeObject->SetScale({ 5.0f, 5.0f, 5.0f });
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

	//���C�g
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
	//�L�[�{�[�h�A�}�E�X�̃C���v�b�g
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
	/// ������3D�I�u�W�F�N�g�̕`�揈����ǉ��ł���
	/// </summary>

	// 3D�I�u�W�F�N�g�`��㏈��
	Object3d::PostDraw();
	ColliderObject::PostDraw();
	MathObject::PostDraw();
	// �O�i�X�v���C�g�`��O����
	Sprite::PreDraw(cmdList);
	Effect2d::PreDraw(cmdList);

	if (moveFlag != true)
	{
		UI::Draw();
	}
	
	Effect::Draw();

	// �f�o�b�O�e�L�X�g�̕`��
	DebugText::GetInstance()->DrawAll(cmdList);

	// �X�v���C�g�`��㏈��
	Sprite::PostDraw();
	Effect2d::PostDraw();
	//imgui�̕`��
	imguiManager::PraDraw();
	ImGui::NewFrame();
	ImGui::Begin("config1");//�E�B���h�E�̖��O
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
	//�v���[���[�I�u�W�F�N�g�̃A�b�v�f�[�g
	objFighter->ObjectUpdate();
	//Json�ǂݍ��݂̃I�u�W�F�N�g�̃A�b�v�f�[�g
	JsonLoader::Update();
	//���̒��I�u�W�F�N�g�̃A�b�v�f�[�g
	OpticalPost::Update(camera->GetEye());
	//�X�J�C�h�[��
	skydomeObject->Update();
	//�J����
	camera->Update();
}

void GamePlayScene::StartStatus()
{
	//�J�n����
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
	//�A�b�v�f�[�g
	Enemy::Update((int)objFighter->GetTimeLimit(), objFighter->GetPos());
	JsonLoader::Update();

	UI::Update();

	OpticalPost::Update(camera->GetEye());

	camera->Update();
	lightGroup->Update();
	objFighter->Update();

	OpticalPost::SetDrawFlag(true);

	//test�ǉ��T���G�R���C�_�[
	for (int i = 0; i < enemyColliderObjects.size(); i++)
	{
		enemyColliderObjects[i].get()->SetScale(Enemy::MonitoringCollisionScale());
		enemyColliderObjects[i].get()->Update();
	}

	skydomeObject->Update();

	//UI�X�V
	DebugText::GetInstance()->Print(1000, 20, 3, "TIME : %d", (int)objFighter->GetTimeLimit());
	//DebugText::GetInstance()->Print(910, 80, 3, "CRYSTAL : %d/7", objFighter->GetCrystal());

	//�S�Ă̏Փ˂��`�F�b�N
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
	//�Q�[���I������
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
