#include "TitleScene.h"
#include "SceneManager.h"
#include "Audio.h"
#include "Input.h"
#include "DebugText.h"
#include "DirectXCommon.h"
#include "SafeDelete.h"

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

	// �J��������
	camera = new DebugCamera(WinApp::window_width, WinApp::window_height);
	// �J�����Z�b�g
	Object3d::SetCamera(camera);
	camera->SetEye({ 0, 100, -30 });

	//�f�o�C�X�̃Z�b�g
	FbxObject3d::SetDevice(DirectXCommon::GetInstance()->GetDev());
	FbxObject3d::SetCamera(camera);
	//�O���t�B�b�N�X�p�C�v���C������
	FbxObject3d::CreateGraphicsPipeline();

	//���C�g
	lightGroup = LightGroup::Create();
	lightGroup->SetDirLightColor(0, { 1,1,1 });
	Object3d::SetLight(lightGroup);
	lightGroup->SetCircleShadowActive(0, true);
	lightGroup->SetCircleShadowActive(1, true);

	skydomeModel = Model::LoadFromOBJ("skydome");
	skydomeObject = Object3d::Create();
	skydomeObject->SetModel(skydomeModel);
	skydomeObject->SetScale({ 5.0f, 5.0f, 5.0f });

	terrainModel = Model::LoadFromOBJ("terrain");
	terrainObject = Object3d::Create();
	terrainObject->SetModel(terrainModel);
	terrainObject->SetScale({ 1.0f, 1.0f, 1.0f });


	fbxModel = FbxLoader::GetInstance()->LoadModelFromFile("model");
	fbxObject = new FbxObject3d();
	fbxObject->Initialize();
	fbxObject->SetModel(fbxModel);
	fbxObject->SetScale({ 0.01f ,0.01f ,0.01f });
}

void TitleScene::Finalize()
{
	safe_delete(camera);
	safe_delete(spriteBG);
	safe_delete(lightGroup);
}

void TitleScene::Update()
{
	// �Q�[���V�[���̖��t���[������

	Input *input = Input::GetInstance();
	lightGroup->SetCircleShadowDir(0, XMVECTOR({ circleShadowDir[0], circleShadowDir[1], circleShadowDir[2], 0 }));
	lightGroup->SetCircleShadowAtten(0, XMFLOAT3(circleShadowAtten));
	lightGroup->SetCircleShadowFactorAngle(0, XMFLOAT2(circleShadowFactorAngle));

	lightGroup->SetCircleShadowDir(1, XMVECTOR({ circleShadowDir[0], circleShadowDir[1], circleShadowDir[2], 0 }));
	lightGroup->SetCircleShadowAtten(1, XMFLOAT3(circleShadowAtten));
	lightGroup->SetCircleShadowFactorAngle(1, XMFLOAT2(circleShadowFactorAngle2));

	skydomeObject->Update();
	terrainObject->Update();

	camera->rotaX += 1.0f;
	camera->Update();
	fbxObject->Update();

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

	skydomeObject->Draw();
	terrainObject->Draw();

	fbxObject->Draw(cmdList);

	// 3D�I�u�W�F�N�g�`��㏈��
	Object3d::PostDraw();

	// �O�i�X�v���C�g�`��O����
	Sprite::PreDraw(cmdList);

	spriteBG->Draw();
	// �f�o�b�O�e�L�X�g�̕`��
	DebugText::GetInstance()->DrawAll(cmdList);

	// �X�v���C�g�`��㏈��
	Sprite::PostDraw();
}
