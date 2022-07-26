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
	camera->SetEye({ 0, 30, -70 });		//prin��
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

	//3D�I�u�W�F�N�g�����ƃ��f���̃Z�b�g
	fbxObject1 = new FbxObject3d;
	fbxObject1->Initialize();
	fbxObject1->SetModel(fbxModel1);
	fbxObject1->SetScale({ 0.01f,0.01f,0.01f });

	colliderModel = ColliderModel::ColliderModelCreate("BOX");
	colliderObject = ColliderObject::Create();
	colliderObject->SetModel(colliderModel);


	//json
	JsonLoader::LoadFile("Scene1");
	JsonLoader::SetObject();
}

void GamePlayScene::Finalize()
{
	//delete model;
	delete fbxObject1;
}

void GamePlayScene::Update()
{
	// �Q�[���V�[���̖��t���[������
	Input *input = Input::GetInstance();

	Input::MousePos mpos = input->MousePosLoad();

	fbxObject1->SetPosition(Player::Move(input, graFlag));
	colliderObject->SetPosition(fbxObject1->GetPosition());
	colliderObject->SetCenter({ 0, 2.5f, 0 });
	colliderObject->SetScale({ 1, 5.0f, 1});

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

	DebugText::GetInstance()->Print(50, 30 * 1, 2, "X:%f", camera->GetEye().x);
	DebugText::GetInstance()->Print(50, 30 * 2, 2, "Y:%f", camera->GetEye().y);
	DebugText::GetInstance()->Print(50, 30 * 3, 2, "Z:%f", camera->GetEye().z);
	DebugText::GetInstance()->Print(50, 30 * 4, 2, "X:%f", DebugCamera::dx);
	DebugText::GetInstance()->Print(50, 30 * 5, 2, "Y:%f", DebugCamera::dy);
	DebugText::GetInstance()->Print(50, 30 * 6, 2, "Z:%f", DebugCamera::dz);

	if (input->TriggerKey(DIK_SPACE))
	{
		//BGM�~�߂�
		Audio::GetInstance()->SoundStop("zaza.wav");
		Audio::GetInstance()->PlayWave("zaza.wav", false);
		
		//�V�[���؂�ւ�
		//SceneManager::GetInstance()->ChangeScene("TITLE");
	}	

	fbxObject1->AnimationFlag = false;
	fbxObject1->AnimationNum = 1;
	//�A�b�v�f�[�g
	camera->Update();
	//objectX->Update();
	fbxObject1->Update();
	colliderObject->Update();
	JsonLoader::Update();
	DebugText::GetInstance()->Print(50, 30 * 3, 2, "%d", fbxObject1->GetisPlay());
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
