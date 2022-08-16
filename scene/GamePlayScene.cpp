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
#include "MyMath.h"
#include "CollisionSet.h"
#include "Enemy.h"

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
	//dCamera->SetEye({ 0, 0, 100 });			//prinding��
	camera->SetEye({ 0, 0, -30 });		//prin��
	FbxObject3d::SetCamera(camera);

	//�O���t�B�b�N�X�p�C�v���C������
	FbxObject3d::CreateGraphicsPipeline();

	// �e�N�X�`���ǂݍ���
	Sprite::LoadTexture(1, L"Resources/background.png");
	// �w�i�X�v���C�g����
	spriteBG = Sprite::Create(1, { 0.0f,0.0f });

	// �I�u�W�F�N�g����
	//model = Model::LoadFromOBJ("sphere");
	//objectX = Object3d::Create();
	//�I�u�W�F�N�g�Ƀ��f�����Ђ��t����
	//objectX->SetModel(model);

	//���f�������w�肵�ăt�@�C���ǂݍ���
	fbxModel1 = FbxLoader::GetInstance()->LoadModelFromFile("model");
	fbxModel2 = FbxLoader::GetInstance()->LoadModelFromFile("Enemy1");

	//3D�I�u�W�F�N�g�����ƃ��f���̃Z�b�g
	fbxObject1 = new FbxObject3d;
	fbxObject1->Initialize();
	fbxObject1->SetModel(fbxModel1);
	fbxObject1->SetScale({ 0.01f,0.01f,0.01f });

	fbxObject2 = new FbxObject3d;
	fbxObject2->Initialize();
	fbxObject2->SetModel(fbxModel2);
	fbxObject2->SetPosition(Enemy::GetPos());
	fbxObject2->SetScale({ 0.01f,0.01f,0.01f });


	colliderModel = ColliderModel::ColliderModelCreate("BOX");
	colliderObject = ColliderObject::Create();
	colliderObject->SetModel(colliderModel);

	colliderObject->SetCenter({ 0, 2.5f, 0 });
	colliderObject->SetScale(Player::GetSize());
	//json
	JsonLoader::LoadFile("Scene2");
	JsonLoader::SetObject();
}

void GamePlayScene::Finalize()
{
	//delete model;
	delete fbxObject1;
	delete fbxObject2;
}

void GamePlayScene::Update()
{
	// �Q�[���V�[���̖��t���[������
	Input *input = Input::GetInstance();

	Input::MousePos mpos = input->MousePosLoad();

	if (input->PushKey(DIK_UP) || input->PushKey(DIK_DOWN) || input->PushKey(DIK_RIGHT) || input->PushKey(DIK_LEFT) || input->PushKey(DIK_E) || input->PushKey(DIK_Z))
	{
		// ���݂̍��W���擾
		XMFLOAT3 position = camera->GetEye();

		// �ړ���̍��W���v�Z
		if (input->PushKey(DIK_UP)) { position.y += 1.0f; }
		else if (input->PushKey(DIK_DOWN)) { position.y -= 1.0f; }
		if (input->PushKey(DIK_RIGHT)) { position.x += 1.0f; }
		else if (input->PushKey(DIK_LEFT)) { position.x -= 1.0f; }
		if (input->PushKey(DIK_E)) { position.z += 1.0f; }
		else if (input->PushKey(DIK_Z)) { position.z -= 1.0f; }
		// ���W�̕ύX�𔽉f
		camera->SetEye(position);
	}

	Player::Move(input, groundY);
	CollisionSet::CollisionCheck(Player::GetPos(), colliderObject->GetScale(), groundY);
	
	CollisionSet::CollisionPushBack(colliderObject->GetScale(), groundY);
	
	MyMath::GravityCheck(Player::GetPos(), groundY, Player::groundFlag);

	if (Player::GetWallColl() == true)
	{
		colliderObject->SetColor({ 1, 1, 0});
	}
	else if (Player::GetWallColl() == false)
	{
		colliderObject->SetColor({ 1, 0, 0});
	}

	if (input->PushKey(DIK_G))
	{
		Enemy::Tracking(Player::GetPos());
	}




	DebugText::GetInstance()->Print(50, 30 * 1, 2, "C:X:%f", camera->GetEye().x);
	DebugText::GetInstance()->Print(50, 30 * 2, 2, "C:Y:%f", camera->GetEye().y);
	DebugText::GetInstance()->Print(50, 30 * 3, 2, "C:Z:%f", camera->GetEye().z);
	DebugText::GetInstance()->Print(50, 30 * 4, 2, "P:X:%f", Player::GetPos().x);
	DebugText::GetInstance()->Print(50, 30 * 5, 2, "P:Y:%f", Player::GetPos().y);
	DebugText::GetInstance()->Print(50, 30 * 6, 2, "P:Z:%f", Player::GetPos().z);
	DebugText::GetInstance()->Print(50, 30 * 7, 2, "M:X:%f", Player::GetMove().x);
	DebugText::GetInstance()->Print(50, 30 * 8, 2, "M:Y:%f", Player::GetMove().y);
	DebugText::GetInstance()->Print(50, 30 * 9, 2, "M:Z:%f", Player::GetMove().z);
	DebugText::GetInstance()->Print(50, 30 * 9, 2, "M:Z:%f", Player::GetMove().z);
	DebugText::GetInstance()->Print(50, 30 * 9, 2, "G:Y:%f", groundY);
	//if (input->TriggerKey(DIK_SPACE))
	//{
	//	//BGM�~�߂�
	//	Audio::GetInstance()->SoundStop("zaza.wav");
	//	Audio::GetInstance()->PlayWave("zaza.wav", false);
	//	
	//	//�V�[���؂�ւ�
	//	//SceneManager::GetInstance()->ChangeScene("TITLE");
	//}	



	fbxObject1->SetPosition(Player::GetPos());
	colliderObject->SetPosition(Player::GetPos());

	fbxObject2->SetPosition(Enemy::GetPos());

	fbxObject1->AnimationFlag = false;
	fbxObject2->AnimationFlag = false;
	//fbxObject1->AnimationNum = 1;
	
	
	//�A�b�v�f�[�g
	camera->Update();
	//objectX->Update();
	fbxObject1->Update();
	fbxObject2->Update();
	colliderObject->Update();
	JsonLoader::Update();
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

	// 3D�I�u�N�W�F�N�g�̕`��
	//objectX->Draw();
	

	//json
	JsonLoader::Draw();

	//FBX3D�I�u�W�F�N�g�̕`��
	fbxObject1->Draw(cmdList);
	fbxObject2->Draw(cmdList);
	colliderObject->Draw();
	/// <summary>
	/// ������3D�I�u�W�F�N�g�̕`�揈����ǉ��ł���
	/// </summary>

	// 3D�I�u�W�F�N�g�`��㏈��
	Object3d::PostDraw();
	ColliderObject::PostDraw();
	// �O�i�X�v���C�g�`��O����
	Sprite::PreDraw(cmdList);

	// �f�o�b�O�e�L�X�g�̕`��
	DebugText::GetInstance()->DrawAll(cmdList);

	// �X�v���C�g�`��㏈��
	Sprite::PostDraw();

	//imgui�̕`��
	//imguiManager::PraDraw();
	//imguiManager::PosDraw();
}
