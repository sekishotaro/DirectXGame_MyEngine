#include "GameOver.h"
#include "SceneManager.h"
#include "Audio.h"
#include "Input.h"
#include "DebugText.h"
#include "DirectXCommon.h"

void GameOver::Initialize()
{
	// �e�N�X�`���ǂݍ���
	Sprite::LoadTexture(1, L"Resources/background2.png");
	// �w�i�X�v���C�g����
	spriteBG = Sprite::Create(1, { 0.0f,0.0f });
}

void GameOver::Finalize()
{
}

void GameOver::Update()
{
	// �Q�[���V�[���̖��t���[������

	Input* input = Input::GetInstance();

	if (input->TriggerKey(DIK_RETURN) || input->PushPadbutton(Button_A))
	{
		//�V�[���؂�ւ�
		SceneManager::GetInstance()->ChangeScene("GAMEPLAY");
	}

	if (input->TriggerKey(DIK_SPACE) || input->PushPadbutton(Button_Y))
	{
		//�V�[���؂�ւ�
		SceneManager::GetInstance()->ChangeScene("TITLE");
	}

	DebugText::GetInstance()->Print(50, 20, 2, "Push_Key [ENTER] or Push_Pad [Y] for TITLE_SCENE");
	DebugText::GetInstance()->Print(50, 60, 2, "Push_Key [SPACE] or Push_Pad [A] for GAMEPLAY_SCENE");
}

void GameOver::Draw()
{
	// �Q�[���V�[���̕`��

	// �R�}���h���X�g�̎擾
	ID3D12GraphicsCommandList* cmdList = DirectXCommon::GetInstance()->GetCmdList();

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