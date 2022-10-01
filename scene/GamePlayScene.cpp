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

#include "SphereCollider.h"
#include "CollisionManager.h"
#include "Player.h"


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

	modelFighter = Model::LoadFromOBJ("chr_sword");

	//json
	JsonLoader::LoadFile("Scene9_27");
	JsonLoader::SetObject();

	collisionManager = CollisionManager::GetInstance();
	objFighter = Player::Create(modelFighter);
	
	//�R���C�_�[�̒ǉ�
}

void GamePlayScene::Finalize()
{

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

	DebugText::GetInstance()->Print(50, 30 * 1, 2, "Camera:X:%f", camera->GetEye().x);
	DebugText::GetInstance()->Print(50, 30 * 2, 2, "Camera:Y:%f", camera->GetEye().y);
	DebugText::GetInstance()->Print(50, 30 * 3, 2, "Camera:Z:%f", camera->GetEye().z);
	DebugText::GetInstance()->Print(50, 30 * 4, 2, "Player:X:%f", objFighter->GetPosition().x);
	DebugText::GetInstance()->Print(50, 30 * 5, 2, "Player:Y:%f", objFighter->GetPosition().y);
	DebugText::GetInstance()->Print(50, 30 * 6, 2, "Player:Z:%f", objFighter->GetPosition().z);
	
	//Ray ray;
	//ray.start = { 10.0f, 0.5f, 0.0f, 1 };
	//ray.dir = { 0,-1,0,0 };
	//RaycastHit raycastHit;

	//if (collisionManager->Raycast(ray, &raycastHit)) {
	//	DebugText::GetInstance()->Print(50, 30 * 4, 2, "Raycast Hit.");
	//}


	//�A�b�v�f�[�g
	camera->Update();
	objFighter->Update();
	JsonLoader::Update();

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

	// 3D�I�u�N�W�F�N�g�̕`��
	
	objFighter->Draw();
	//json
	JsonLoader::Draw();


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
