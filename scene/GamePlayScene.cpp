#include "GamePlayScene.h"
#include "SceneManager.h"
#include "Audio.h"
#include "Input.h"
#include "DebugText.h"
#include "DirectXCommon.h"
#include "Collision.h"

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
	object2->SetModel(model2);
}

void GamePlayScene::Finalize()
{
	delete model;
}

void GamePlayScene::Update()
{
	// �Q�[���V�[���̖��t���[������
	
	Input *input = Input::GetInstance();

	
	////�����鏈��
	//d1b = k * move1X;
	//move1XValue = move1X - d1b;
	//x1 += move1XValue;

	//d1a = k * dropValue;
	//dropValue += (gravity / 600.0f) - d1a;
	//y1 -= dropValue;


	//���C
	//�n�ʂɂ��Ă���Ɖ���
	SphereF sphere1;
	sphere1.center = { x1, y1, z1 };
	sphere1.radius = 2.0f;
	SphereF sphere2;
	sphere2.center = { x2, y2, z2 };
	sphere2.radius = 2.0f;

	if (Collision::CheckSphereSphere(sphere1, sphere2))
	{
		if(flag == false)
		{
			//x1 -= 5;
			aro1 *= -1.0;
			aro2 *= -1.0;
			flag = true;
		}
	}
	
	move1XValue -= k2;
	if (move1XValue > 0 )
	{
		x1 += move1XValue * aro1;
	}
	
	
	move2XValue -= k2;
	if (move2XValue > 0)
	{
		x2 += move2XValue * aro2;
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
	object1->SetPosition(XMFLOAT3(x1, y1, z1));
	object2->SetPosition(XMFLOAT3(x2, y2, z2));

	DebugText::GetInstance()->Print(50, 30 * 1, 2, "      Camera:%f", camera->GetEye().x);
	DebugText::GetInstance()->Print(50, 30 * 2, 2, "      Camera:%f", camera->GetEye().y);
	DebugText::GetInstance()->Print(50, 30 * 3, 2, "        posY:%f", object1->GetPosition().y);
	DebugText::GetInstance()->Print(50, 30 * 4, 2, "          k2:%f", k2);
	DebugText::GetInstance()->Print(50, 30 * 5, 2, " moveX1Value:%f", move1XValue);
	DebugText::GetInstance()->Print(50, 30 * 6, 2, " moveX2Value:%f", move2XValue);
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
