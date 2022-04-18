
#include "GamePlayScene.h"
#include "SceneManager.h"
#include "Audio.h"
#include "Input.h"
#include "DebugText.h"
#include "DirectXCommon.h"
#include "FbxLoader.h"
#include "FbxObject3d.h"

void GamePlayScene::Initialize()
{
	Audio::GetInstance()->LoadWave("futta-dream.wav");
	Audio::GetInstance()->LoadWave("zaza.wav");

	//Audio::GetInstance()->PlayWave("zaza.wav", true);

	// �J��������
	camera = new Camera(WinApp::window_width, WinApp::window_height);

	//�f�o�C�X�̃Z�b�g
	FbxObject3d::SetDevice(DirectXCommon::GetInstance()->GetDev());

	// �J�����Z�b�g
	Object3d::SetCamera(camera);
	//camera->SetEye({ 0, 0, 100 });			//prinding��
	camera->SetEye({ 300, 0, 300 });		//prin��
	FbxObject3d::SetCamera(camera);

	//�O���t�B�b�N�X�p�C�v���C������
	FbxObject3d::CreateGraphicsPipeline();

	// �e�N�X�`���ǂݍ���
	Sprite::LoadTexture(1, L"Resources/background.png");
	// �w�i�X�v���C�g����
	spriteBG = Sprite::Create(1, { 0.0f,0.0f });

	// �I�u�W�F�N�g����
	model = Model::LoadFromOBJ("sphere");
	objectX = Object3d::Create();
	//�I�u�W�F�N�g�Ƀ��f�����Ђ��t����
	objectX->SetModel(model);

	//���f�������w�肵�ăt�@�C���ǂݍ���
	fbxModel1 = FbxLoader::GetInstance()->LoadModelFromFile("prin");

	//3D�I�u�W�F�N�g�����ƃ��f���̃Z�b�g
	fbxObject1 = new FbxObject3d;
	fbxObject1->Initialize();
	fbxObject1->SetModel(fbxModel1);

}

void GamePlayScene::Finalize()
{
	delete model;
	delete fbxObject1;
	//delete fbxModel1;
}

void GamePlayScene::Update()
{
	// �Q�[���V�[���̖��t���[������
	Input *input = Input::GetInstance();

	//�I�u�W�F�N�g�ړ�
	if (input->PushKey(DIK_UP) || input->PushKey(DIK_DOWN) || input->PushKey(DIK_RIGHT) || input->PushKey(DIK_LEFT))
	{
		// ���݂̍��W���擾
		XMFLOAT3 position = objectX->GetPosition();

		// �ړ���̍��W���v�Z
		if (input->PushKey(DIK_UP)) { position.y += 1.0f; }
		else if (input->PushKey(DIK_DOWN)) { position.y -= 1.0f; }
		if (input->PushKey(DIK_RIGHT)) { position.x += 1.0f; }
		else if (input->PushKey(DIK_LEFT)) { position.x -= 1.0f; }

		// ���W�̕ύX�𔽉f
		objectX->SetPosition(position);
	}

	if (input->PushKey(DIK_W) || input->PushKey(DIK_S) || input->PushKey(DIK_D) || input->PushKey(DIK_A) || input->PushKey(DIK_E) || input->PushKey(DIK_Z))
	{
		// ���݂̍��W���擾
		XMFLOAT3 position = camera->GetEye();

		// �ړ���̍��W���v�Z
		if (input->PushKey(DIK_W)) { position.y += 1.0f; }
		else if (input->PushKey(DIK_S)) { position.y -= 1.0f; }
		if (input->PushKey(DIK_D)) { position.x += 1.0f; }
		else if (input->PushKey(DIK_A)) { position.x -= 1.0f; }
		if (input->PushKey(DIK_E)) { position.z += 1.0f; }
		else if (input->PushKey(DIK_Z)) { position.z -= 1.0f; }
		// ���W�̕ύX�𔽉f
		camera->SetEye(position);
	}

	DebugText::GetInstance()->Print(50, 30 * 1, 2, "X:%f", camera->GetEye().x);
	DebugText::GetInstance()->Print(50, 30 * 2, 2, "Y:%f", camera->GetEye().y);
	DebugText::GetInstance()->Print(50, 30 * 3, 2, "Z:%f", camera->GetEye().z);

	if (input->TriggerKey(DIK_SPACE))
	{
		//BGM�~�߂�
		//Audio::GetInstance()->SoundStop("zaza.wav");
		
		//�V�[���؂�ւ�
		SceneManager::GetInstance()->ChangeScene("TITLE");
	}

	camera->SetTarget({ 0, 0, 0 });
	

	fbxObject1->AnimationFlag = true;
	fbxObject1->AnimationNum = 1;
	//�A�b�v�f�[�g
	camera->Update();
	objectX->Update();
	fbxObject1->Update();
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

	// 3D�I�u�N�W�F�N�g�̕`��
	//objectX->Draw();

	//FBX3D�I�u�W�F�N�g�̕`��
	fbxObject1->Draw(cmdList);

	/// <summary>
	/// ������3D�I�u�W�F�N�g�̕`�揈����ǉ��ł���
	/// </summary>

	// 3D�I�u�W�F�N�g�`��㏈��
	Object3d::PostDraw();

	// �O�i�X�v���C�g�`��O����
	Sprite::PreDraw(cmdList);

	// �f�o�b�O�e�L�X�g�̕`��
	DebugText::GetInstance()->DrawAll(cmdList);

	// �X�v���C�g�`��㏈��
	Sprite::PostDraw();
}
