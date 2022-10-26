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

#include "SphereCollider.h"
#include "CollisionManager.h"
#include "Player.h"
#include "Light.h"


void GamePlayScene::Initialize()
{
	Audio::GetInstance()->LoadWave("futta-dream.wav");
	Audio::GetInstance()->LoadWave("zaza.wav");
	Audio::GetInstance()->LoadWave("What.wav");

	// �J��������
	camera = new DebugCamera(WinApp::window_width, WinApp::window_height);
	//camera = new Camera(WinApp::window_width, WinApp::window_height);
	//���C�g����
	light = new Light(WinApp::window_width, WinApp::window_height);

	//�f�o�C�X�̃Z�b�g
	FbxObject3d::SetDevice(DirectXCommon::GetInstance()->GetDev());

	// �J�����Z�b�g
	Object3d::SetCamera(camera);
	ColliderObject::SetCamera(camera);

	//���C�g�̃Z�b�g
	//light->SetPosition(camera->GetEye());
	//light->SetTarget(camera->GetTarget());
	Object3d::SetLight(light);

	MathObject::SetCamera(camera);
	//dCamera->SetEye({ 0, 0, 100 });			//prinding��
	camera->SetEye({ 0, 0, -30 });		//prin��
	FbxObject3d::SetCamera(camera);

	//�O���t�B�b�N�X�p�C�v���C������
	FbxObject3d::CreateGraphicsPipeline();

	// �e�N�X�`���ǂݍ���
	Sprite::LoadTexture(1, L"Resources/background.png");
	// �w�i�X�v���C�g����
	spriteBG = Sprite::Create(1, { 0.0f,0.0f });

	modelFighter = Model::LoadFromOBJ("modelOb1");

	//���f�������w�肵�ăt�@�C���ǂݍ���
	//fbxModel1 = FbxLoader::GetInstance()->LoadModelFromFile("model");
	//fbxModel2 = FbxLoader::GetInstance()->LoadModelFromFile("Enemy1");

	//3D�I�u�W�F�N�g�����ƃ��f���̃Z�b�g
	//fbxObject1 = new FbxObject3d;
	//fbxObject1->Initialize();
	//fbxObject1->SetModel(fbxModel1);
	//fbxObject1->SetScale({ 0.01f,0.01f,0.01f });

	//fbxObject2 = new FbxObject3d;
	//fbxObject2->Initialize();
	//fbxObject2->SetModel(fbxModel2);
	//fbxObject2->SetPosition(Enemy::GetPos());
	//fbxObject2->SetScale({ 0.01f,0.01f,0.01f });


	//colliderModel = ColliderModel::ColliderModelCreate("BOX");
	//colliderObject = ColliderObject::Create();
	//colliderObject->SetModel(colliderModel);

	//colliderObject->SetCenter({ 0, 2.5f, 0 });
	//colliderObject->SetScale(Player::GetSize());
	
	//json
	JsonLoader::LoadFile("Scene10_18"); //�I�u�W�F�N�g�̓����蔻��

	//JsonLoader::LoadFile("Scene9_27"); //���b�V���R���C�_�[
	
	JsonLoader::SetObject();

	//Enemy::Initialize();
	//mathModel = MathModel::LoadFromOBJ("sphere");
	//enemyCollider1Object = MathObject::Create();
	//enemyCollider2Object = MathObject::Create();

	////�I�u�W�F�N�g�Ƀ��f�����Ђ��t����
	//enemyCollider1Object->SetModel(mathModel);
	//enemyCollider1Object->SetPosition(XMFLOAT3(0, 5, 0));

	//enemyCollider2Object->SetModel(mathModel);
	//enemyCollider2Object->SetPosition(XMFLOAT3(0, 5, 0));
	//enemyCollider2Object->SetScale(XMFLOAT3(20, 20, 20));
	//enemyCollider2Object->SetColor(XMFLOAT4(0, 0, 1, 0.2));

	collisionManager = CollisionManager::GetInstance();
	objFighter = Player::Create(modelFighter);
}

void GamePlayScene::Finalize()
{
	//delete model;
	//delete fbxObject1;
	//delete fbxObject2;
}

void GamePlayScene::Update()
{
	//�L�[�{�[�h�A�}�E�X�̃C���v�b�g
	Input *input = Input::GetInstance();
	Input::MousePos mpos = input->MousePosLoad();


	XMFLOAT3 cameraPos = objFighter->GetPosition();
	static XMFLOAT3 pos = {0, 5.0f, -10.0f};

	cameraPos.x += pos.x;
	cameraPos.y += pos.y;
	cameraPos.z += pos.z;

	camera->SetEye(cameraPos);
	//�J�����̈ړ�
	if (input->PushKey(DIK_UP) || input->PushKey(DIK_DOWN) || input->PushKey(DIK_RIGHT) || input->PushKey(DIK_LEFT) || input->PushKey(DIK_E) || input->PushKey(DIK_Z))
	{
		// ���݂̍��W���擾
		//XMFLOAT3 position = camera->GetEye();

		// �ړ���̍��W���v�Z
		if (input->PushKey(DIK_UP)) { pos.y += 1.0f; }
		else if (input->PushKey(DIK_DOWN)) { pos.y -= 1.0f; }
		if (input->PushKey(DIK_RIGHT)) { pos.x += 1.0f; }
		else if (input->PushKey(DIK_LEFT)) { pos.x -= 1.0f; }
		if (input->PushKey(DIK_E)) { pos.z += 1.0f; }
		else if (input->PushKey(DIK_Z)) { pos.z -= 1.0f; }
	}

	//�v���C���[�̈ړ�

	//�v���C���[�ƒn�ʂ̓����蔻�菈��
	//if (Player::GetWallColl() == true)
	//{
	//	colliderObject->SetColor({ 1, 1, 0});
	//}
	//else if (Player::GetWallColl() == false)
	//{
	//	colliderObject->SetColor({ 1, 0, 0});
	//}

	//if (input->PushKey(DIK_G))
	//{
	//	Enemy::Move(Player::GetPos());
	//}

	//if (input->PushKey(DIK_I))
	//{
	//	XMFLOAT3 pos = Enemy::GetPos();
	//	pos.y += 1.0f;
	//	Enemy::SetPos(pos);
	//}

	////���@�̓����蔻��pBOX�̐ݒ�
	//Pbox.centerPos = Player::GetPos();
	//Pbox.LeastPos = XMFLOAT3(Player::GetPos().x - Player::GetSize().x /2, Player::GetPos().y, Player::GetPos().z - Player::GetSize().z / 2);
	//Pbox.MaxPos = XMFLOAT3(Player::GetPos().x + Player::GetSize().x / 2, Player::GetPos().y + Player::GetSize().y, Player::GetPos().z + Player::GetSize().z / 2);

	////�G�̓����蔻��v�Z�p�̋��̏�����
	//SphereF Esphere;
	//Esphere.center = Enemy::GetPos();
	//Esphere.radius = 2;

	//SphereF E2sphere;
	//E2sphere.center = Enemy::GetPos();
	//E2sphere.radius = 20;

	////���@�ƓG�̐���
	//LineSegment line;
	//line.start = Enemy::GetPos();
	//line.end = Player::GetPos();

	////�ǂƂ̓����蔻��v�Z�p��Box�̏�����
	//Box wall;
	//wall.centerPos = JsonLoader::colliderObjects[0].get()->GetPosition();
	//wall.size = JsonLoader::colliderObjects[0].get()->GetScale();
	//wall.LeastPos = { wall.centerPos.x - wall.size.x / 2,wall.centerPos.y - wall.size.y / 2, wall.centerPos.z - wall.size.z / 2};
	//wall.MaxPos = { wall.centerPos.x + wall.size.x / 2,wall.centerPos.y + wall.size.y / 2, wall.centerPos.z + wall.size.z /2 };

	//if (Collision::CheckLineSegmentBox(line, wall) == true)
	//{
	//	enemyCollider1Object->SetColor({ 1, 1, 0, 0.5f });
	//}
	//else
	//{
	//	enemyCollider1Object->SetColor({ 1, 1, 1, 0.5f });
	//}

	//if (Collision::CheckSphereBox(E2sphere, Pbox) == true)
	//{
	//	Enemy::Move(Player::GetPos());
	//}


	//if (Collision::CheckSphereBox(Esphere, Pbox) == true)
	//{
	//	colliderObject->SetColor({ 1, 0, 0 });
	//}
	//else
	//{
	//	colliderObject->SetColor({ 1, 1, 1 });
	//}
	//DebugText::GetInstance()->Print(50, 30 * 8, 2, "M:Y:%f", Player::GetMove().y);
	//DebugText::GetInstance()->Print(50, 30 * 9, 2, "M:Z:%f", Player::GetMove().z);
	
	//if (input->TriggerKey(DIK_SPACE))
	//{
	//	//BGM�~�߂�
	//	Audio::GetInstance()->SoundStop("zaza.wav");
	//	Audio::GetInstance()->PlayWave("zaza.wav", false);
	//	
	//	//�V�[���؂�ւ�
	//	//SceneManager::GetInstance()->ChangeScene("TITLE");
	//}	
	//DebugText::GetInstance()->Print(50, 30 * 1, 2, "Camera:X:%f", camera->GetEye().x);
	//DebugText::GetInstance()->Print(50, 30 * 2, 2, "Camera:Y:%f", camera->GetEye().y);
	//DebugText::GetInstance()->Print(50, 30 * 3, 2, "Camera:Z:%f", camera->GetEye().z);
	//DebugText::GetInstance()->Print(50, 30 * 4, 2, "Player:X:%f", objFighter->GetPosition().x);
	//DebugText::GetInstance()->Print(50, 30 * 5, 2, "Player:Y:%f", objFighter->GetPosition().y);
	//DebugText::GetInstance()->Print(50, 30 * 6, 2, "Player:Z:%f", objFighter->GetPosition().z);
	
	//Ray ray;
	//ray.start = { 10.0f, 0.5f, 0.0f, 1 };
	//ray.dir = { 0,-1,0,0 };
	//RaycastHit raycastHit;

	//if (collisionManager->Raycast(ray, &raycastHit)) {
	//	DebugText::GetInstance()->Print(50, 30 * 4, 2, "Raycast Hit.");
	//}


	//�`��I�u�W�F�N�g�֘A�̍X�V
	//enemyCollider1Object->SetPosition(Enemy::GetPos());
	//enemyCollider2Object->SetPosition(Enemy::GetPos());
	//fbxObject1->SetPosition(Player::GetPos());
	//colliderObject->SetPosition(Player::GetPos());
	//fbxObject2->SetPosition(Enemy::GetPos());
	//fbxObject1->AnimationFlag = false;
	//fbxObject2->AnimationFlag = false;
	//fbxObject1->AnimationNum = 1;

	//�Q�[���I������
	if (objFighter->GetCrystal() == 0 && objFighter->GetGoalFlag() == true)
	{
		SceneManager::GetInstance()->ChangeScene("TITLE");
	}
	
	//�A�b�v�f�[�g
	JsonLoader::Update();

	camera->Update();
	light->Update();
	objFighter->Update();

	//�S�Ă̏Փ˂��`�F�b�N
	collisionManager->CheckAllCollisions();

	//enemyCollider1Object->Update();
	//enemyCollider2Object->Update();
	//fbxObject1->Update();
	//fbxObject2->Update();
	//colliderObject->Update();
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
	
	objFighter->Draw();
	//json
	JsonLoader::Draw();

	//FBX3D�I�u�W�F�N�g�̕`��
	//fbxObject1->Draw(cmdList);
	//fbxObject2->Draw(cmdList);
	//colliderObject->Draw();
	//enemyCollider1Object->Draw();
	//enemyCollider2Object->Draw();
	
	/// <summary>
	/// ������3D�I�u�W�F�N�g�̕`�揈����ǉ��ł���
	/// </summary>

	// 3D�I�u�W�F�N�g�`��㏈��
	Object3d::PostDraw();
	ColliderObject::PostDraw();
	MathObject::PostDraw();
	// �O�i�X�v���C�g�`��O����
	Sprite::PreDraw(cmdList);

	// �f�o�b�O�e�L�X�g�̕`��
	DebugText::GetInstance()->DrawAll(cmdList);

	//�|�X�g�G�t�F�N�g
	// �X�v���C�g�`��㏈��
	Sprite::PostDraw();

	//imgui�̕`��
	imguiManager::PraDraw();
	ImGui::NewFrame();
	ImGui::Begin("config1");//�E�B���h�E�̖��O
	ImGui::SetWindowSize(ImVec2(400, 500), ImGuiCond_::ImGuiCond_FirstUseEver);
	
	ImGui::Text("PosX    :%.4f", objFighter->GetPosition().x);
	ImGui::Text("PosY    :%.4f", objFighter->GetPosition().y);
	ImGui::Text("PosZ    :%.4f", objFighter->GetPosition().z);
	ImGui::Text("MoveX   :%.4f", objFighter->GetMove().x);
	ImGui::Text("MoveY   :%.4f", objFighter->GetMove().y);
	ImGui::Text("MoveZ   :%.4f", objFighter->GetMove().z);
	ImGui::Text("cameraX :%.4f", camera->GetEye().x);
	ImGui::Text("cameraY :%.4f", camera->GetEye().y);
	ImGui::Text("cameraZ :%.4f", camera->GetEye().z);
	ImGui::Text("crystal :%d", objFighter->GetCrystal());
	ImGui::Checkbox("GoalFlag", &objFighter->GetGoalFlag());
	imguiManager::PosDraw();
}

void GamePlayScene::ShadowDraw()
{
	// �R�}���h���X�g�̎擾
	ID3D12GraphicsCommandList* cmdList = DirectXCommon::GetInstance()->GetCmdList();
	
	// �[�x�o�b�t�@�N���A
	DirectXCommon::GetInstance()->ClearDepthBuffer();

	// 3D�I�u�W�F�N�g�`��O����
	Object3d::PreDraw(cmdList);
	ColliderObject::PreDraw(cmdList);
	MathObject::PreDraw(cmdList);

	// 3D�I�u�N�W�F�N�g�̕`��
	objFighter->ShadowDraw();
	//json
	JsonLoader::ShadowDraw();
	//FBX3D�I�u�W�F�N�g�̕`��

	// 3D�I�u�W�F�N�g�`��㏈��
	Object3d::PostDraw();
	ColliderObject::PostDraw();
	MathObject::PostDraw();
}
