#include "GamePlayScene.h"
#include "SceneManager.h"
#include "Audio.h"
#include "Input.h"
#include "DebugText.h"
#include "DirectXCommon.h"
#include "MyMath.h"
#include "Easing.h"

void GamePlayScene::Initialize()
{
	Audio::GetInstance()->LoadWave("futta-dream.wav");
	Audio::GetInstance()->LoadWave("zaza.wav");

	//Audio::GetInstance()->PlayWave("zaza.wav", true);

	// �J��������
	camera = new Camera(WinApp::window_width, WinApp::window_height);

	// �J�����Z�b�g
	Object3d::SetCamera(camera);
	XMFLOAT3 position = { 0.0f, 0.0f, -100.0f };
	// ���W�̕ύX�𔽉f
	camera->SetEye(position);
	// �e�N�X�`���ǂݍ���

	Sprite::LoadTexture(1, L"Resources/background.png");
	// �w�i�X�v���C�g����
	spriteBG = Sprite::Create(1, { 0.0f,0.0f });

	// �I�u�W�F�N�g����
	model = Model::LoadFromOBJ("sphere");
	model2 = Model::LoadFromOBJ("sphere2");

	object1 = Object3d::Create();
	object2 = Object3d::Create();

	//�I�u�W�F�N�g�Ƀ��f�����Ђ��t����
	object1->SetModel(model);
	object2->SetModel(model);
}

void GamePlayScene::Finalize()
{
	delete model;
}

void GamePlayScene::Update()
{
	// �Q�[���V�[���̖��t���[������

	Input* input = Input::GetInstance();

	sphere1.center = pos1;
	sphere1.radius = 5.0f;

	if (input->PushKey(DIK_RETURN))
	{
		startFlag = true;
	}

	if (input->PushKey(DIK_R))
	{
		startFlag = false;
	}

	if (startFlag == true)
	{
		pos1 =	MyMath::Pendulum(angle, speed, mass, length, x);
	}



	//�J�����̈ړ�
	if (input->PushKey(DIK_W) || input->PushKey(DIK_S) || input->PushKey(DIK_D) || input->PushKey(DIK_A))
	{
		// ���݂̍��W���擾
		XMFLOAT3 position = camera->GetEye();

		// �ړ���̍��W���v�Z
		if (input->PushKey(DIK_W)) { position.y += 1.0f; }
		else if (input->PushKey(DIK_S)) { position.y -= 1.0f; }
		if (input->PushKey(DIK_D)) { position.x += 1.0f; }
		else if (input->PushKey(DIK_A)) { position.x -= 1.0f; }

		// ���W�̕ύX�𔽉f
		camera->SetEye(position);
	}
	object1->SetPosition(pos1);
	object2->SetPosition(pos2);

	DebugText::GetInstance()->Print(0, 30 * 1, 2, "        Camera:%f", camera->GetEye().x);
	DebugText::GetInstance()->Print(0, 30 * 2, 2, "        Camera:%f", camera->GetEye().y);
	DebugText::GetInstance()->Print(0, 30 * 3, 2, "          pos1X:%f", object1->GetPosition().x);
	DebugText::GetInstance()->Print(0, 30 * 4, 2, "          pos21:%f", object1->GetPosition().y);
	DebugText::GetInstance()->Print(0, 30 * 5, 2, "          pos2Z:%f", object1->GetPosition().z);
	DebugText::GetInstance()->Print(0, 30 * 6, 2, "          angle:%f", angle);
	DebugText::GetInstance()->Print(0, 30 * 7, 2, "          speed:%f", speed);
	DebugText::GetInstance()->Print(0, 30 * 8, 2, "              x:%f", x);
	if (input->TriggerKey(DIK_SPACE))
	{
		//BGM�~�߂�
		//Audio::GetInstance()->SoundStop("zaza.wav");

		//�V�[���؂�ւ�
		SceneManager::GetInstance()->ChangeScene("TITLE");
	}

	//�A�b�v�f�[�g
	camera->Update();
	object1->Update();
	object2->Update();
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
	object1->Draw();
	object2->Draw();
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
