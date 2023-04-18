#include "TitleScene.h"
#include "SceneManager.h"
#include "Audio.h"
#include "Input.h"
#include "DebugText.h"
#include "DirectXCommon.h"

void TitleScene::Initialize()
{
	// �e�N�X�`���ǂݍ���
	Sprite::LoadTexture(1, L"Resources/TitleBackground.png");
	Sprite::LoadTexture(2, L"Resources/UI_StartKey.png");
	// �w�i�X�v���C�g����
	spriteBG = Sprite::Create(1, { 0.0f,0.0f });
	spriteUI = Sprite::Create(2, { (float)WinApp::window_width / 2.0f,(float)WinApp::window_height * 2.5f / 3.0f });
	spriteUI->SetAnchorPoint({ 0.5f, 0.5f });
	spriteUI->SetPosition({ (float)WinApp::window_width / 2.0f,(float)WinApp::window_height * 2.5f / 3.0f });
}

void TitleScene::Finalize()
{
}

void TitleScene::Update()
{
	// �Q�[���V�[���̖��t���[������

	Input *input = Input::GetInstance();

	if (input->TriggerKey(DIK_RETURN) || input->PushPadbutton(Button_A))
	{
		//�V�[���؂�ւ�
		SceneManager::GetInstance()->ChangeScene("GAMEPLAY");
	}
}

void TitleScene::Draw()
{
	// �Q�[���V�[���̕`��

// �R�}���h���X�g�̎擾
	ID3D12GraphicsCommandList *cmdList = DirectXCommon::GetInstance()->GetCmdList();

	// �w�i�X�v���C�g�`��O����
	Sprite::PreDraw(cmdList);
	// �w�i�X�v���C�g�`��
	spriteBG->Draw();
	spriteUI->Draw();
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
