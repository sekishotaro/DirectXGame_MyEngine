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
#include "Effect2d.h"

#include "SphereCollider.h"
#include "CollisionManager.h"
#include "Player.h"
#include "UI.h"
#include "OpticalPost.h"
#include "Effect.h"

#include "SafeDelete.h"

#include "ClockTime.h"

void GamePlayScene::Initialize()
{
	// �J��������
	camera = new DebugCamera(WinApp::window_width, WinApp::window_height);
	//camera = new Camera(WinApp::window_width, WinApp::window_height);
	//�f�o�C�X�̃Z�b�g
	FbxObject3d::SetDevice(DirectXCommon::GetInstance()->GetDev());
	// �J�����Z�b�g
	Object3d::SetCamera(camera);
	ColliderObject::SetCamera(camera);
	MathObject::SetCamera(camera);
	//dCamera->SetEye({ 0, 0, 100 });			//prinding��
	camera->SetEye({ 0, 0, -30 });		//prin��
	FbxObject3d::SetCamera(camera);
	//�O���t�B�b�N�X�p�C�v���C������
	FbxObject3d::CreateGraphicsPipeline();
	//���C�g
	lightGroup = LightGroup::Create();
	lightGroup->SetDirLightColor(0, { 1,1,1 });
	Object3d::SetLight(lightGroup);
	lightGroup->SetCircleShadowActive(0, true);
	lightGroup->SetCircleShadowActive(1, true);


	// �e�N�X�`���ǂݍ���
	Sprite::LoadTexture(1, L"Resources/background2.png");
	// �w�i�X�v���C�g����
	spriteBG = Sprite::Create(1, { 0.0f,0.0f });
	skydomeModel = Model::LoadFromOBJ("skydome");
	skydomeObject = Object3d::Create();
	skydomeObject->SetModel(skydomeModel);
	skydomeObject->SetScale({ 5.0f, 5.0f, 5.0f });
	//json
	JsonLoader::LoadFile("Scene4_7"); //�I�u�W�F�N�g�̓����蔻��
	JsonLoader::SetObject();

	////���f�������w�肵�ăt�@�C���ǂݍ���
	fbxModel = FbxLoader::GetInstance()->LoadModelFromFile("model");
	fbxModel2 = FbxLoader::GetInstance()->LoadModelFromFile("model2");
	fbxModel3 = FbxLoader::GetInstance()->LoadModelFromFile("model3");
	fbxModel4 = FbxLoader::GetInstance()->LoadModelFromFile("model4");
	fbxModel5 = FbxLoader::GetInstance()->LoadModelFromFile("model5");
	fbxModel6 = FbxLoader::GetInstance()->LoadModelFromFile("model6");
	fbxModel7 = FbxLoader::GetInstance()->LoadModelFromFile("model7");
	fbxModel8 = FbxLoader::GetInstance()->LoadModelFromFile("model8");
	fbxModel9 = FbxLoader::GetInstance()->LoadModelFromFile("model9");
	fbxModel10 = FbxLoader::GetInstance()->LoadModelFromFile("model10");
	fbxModel11 = FbxLoader::GetInstance()->LoadModelFromFile("model11");
	fbxModel12 = FbxLoader::GetInstance()->LoadModelFromFile("model12");
	fbxModel13 = FbxLoader::GetInstance()->LoadModelFromFile("model13");
	fbxModel14 = FbxLoader::GetInstance()->LoadModelFromFile("model14");
	fbxModel15 = FbxLoader::GetInstance()->LoadModelFromFile("model15");

	collisionManager = CollisionManager::GetInstance();
	objFighter = Player::Create(fbxModel);
	objFighter->SetModel1(fbxModel);
	objFighter->SetModel2(fbxModel2);
	objFighter->SetModel3(fbxModel3);
	objFighter->SetModel4(fbxModel4);
	objFighter->SetModel5(fbxModel5);
	objFighter->SetModel6(fbxModel6);
	objFighter->SetModel7(fbxModel7);
	objFighter->SetModel8(fbxModel8);
	objFighter->SetModel9(fbxModel9);
	objFighter->SetModel10(fbxModel10);
	objFighter->SetModel11(fbxModel11);
	objFighter->SetModel12(fbxModel12);
	objFighter->SetModel13(fbxModel13);
	objFighter->SetModel14(fbxModel14);
	objFighter->SetModel15(fbxModel15);
	////�G������
	//Enemy::Initialize();
	
	Effect::Initialize();
	UI::Initialize();

	OpticalPost::Initialize();
}

void GamePlayScene::Finalize()
{
	safe_delete(camera);
	safe_delete(spriteBG);
	safe_delete(lightGroup);
	JsonLoader::Finalize();
	OpticalPost::Finalize();
	Effect::Finalize();
}

void GamePlayScene::Update()
{
	//�L�[�{�[�h�A�}�E�X�̃C���v�b�g
	Input *input = Input::GetInstance();
	Input::MousePos mpos = input->MousePosLoad();

	lightGroup->SetCircleShadowDir(0, XMVECTOR({ circleShadowDir[0], circleShadowDir[1], circleShadowDir[2], 0 }));
	lightGroup->SetCircleShadowCasterPos(0, XMFLOAT3(objFighter->GetPos()));
	lightGroup->SetCircleShadowAtten(0, XMFLOAT3(circleShadowAtten));
	lightGroup->SetCircleShadowFactorAngle(0, XMFLOAT2(circleShadowFactorAngle));

	lightGroup->SetCircleShadowDir(1, XMVECTOR({ circleShadowDir[0], circleShadowDir[1], circleShadowDir[2], 0 }));
	//lightGroup->SetCircleShadowCasterPos(1, XMFLOAT3(JsonLoader::raidEnemyObjects[0].get()->GetPosition()));
	lightGroup->SetCircleShadowAtten(1, XMFLOAT3(circleShadowAtten));
	lightGroup->SetCircleShadowFactorAngle(1, XMFLOAT2(circleShadowFactorAngle2));

	//int state = 2;
	//switch(state)
	//{
	//	 case 1:
	//		 StartStatus();
	//		 return;
	//	 case 2:
	//		 GameStatus();
	//		 return;
	//	 case 3:
	//		 GameOverStatus();
	//		 return;
	//	 case 4:
	//		 ClearStatus();
	//		 return;
	//}

	if (input->TriggerKey(DIK_RETURN))
	{
		//�V�[���؂�ւ�
		SceneManager::GetInstance()->ChangeScene("GAMEOVER");
	}

	GameStatus();
	GameOverStatus();
	ClearStatus();
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

	//3D�I�u�W�F�N�g�`��O����
	Object3d::PreDraw(cmdList);
	ColliderObject::PreDraw(cmdList);
	MathObject::PreDraw(cmdList);

	
	//3D�I�u�N�W�F�N�g�̕`��
	skydomeObject->Draw();
	//json
	JsonLoader::Draw();
	objFighter->Draw(cmdList);
	//for (int i = 0; i < enemyColliderObjects.size(); i++)
	//{
	//	enemyColliderObjects[i].get()->Draw();
	//}
	
	OpticalPost::Draw();

	/// <summary>
	/// ������3D�I�u�W�F�N�g�̕`�揈����ǉ��ł���
	/// </summary>

	// 3D�I�u�W�F�N�g�`��㏈��
	Object3d::PostDraw();
	ColliderObject::PostDraw();
	MathObject::PostDraw();
	// �O�i�X�v���C�g�`��O����
	Sprite::PreDraw(cmdList);
	Effect2d::PreDraw(cmdList);

	if (moveFlag != true)
	{
		UI::Draw();
	}
	
	Effect::Draw();

	// �f�o�b�O�e�L�X�g�̕`��
	//DebugText::GetInstance()->DrawAll(cmdList);

	// �X�v���C�g�`��㏈��
	Sprite::PostDraw();
	Effect2d::PostDraw();
	//imgui�̕`��
	imguiManager::PraDraw();
	ImGui::NewFrame();
	ImGui::Begin("config1");//�E�B���h�E�̖��O
	ImGui::SetWindowSize(ImVec2(400, 500), ImGuiCond_::ImGuiCond_FirstUseEver);
	ImGui::Text("AnimeNum: %d", objFighter->GetAnimeNum());
	ImGui::Text("PlayerPos X: %f", objFighter->GetPos().x);
	ImGui::Text("PlayerPos Y: %f", objFighter->GetPos().y);
	ImGui::Text("PlayerPos Z: %f", objFighter->GetPos().z);
	ImGui::Text("PlayerRot Y: %f", objFighter->GetRotation().y);
	ImGui::Text("Camera    Y: %f",  camera->GetEye().y);
	ImGui::Text("CrystalNum: %d", JsonLoader::crystalObjects.size());
	ImGui::Text("crystal :%d", objFighter->GetCrystal());
	ImGui::Text("moveBoxMax_X :%f", objFighter->moveBoxMax1.x);
	ImGui::Text("moveBoxMax_Z :%f", objFighter->moveBoxMax1.z);
	ImGui::Checkbox("Terrain", &JsonLoader::hitTerrainDrawFlag);
	ImGui::Checkbox("teleport", &objFighter->teleportFlag);
	ImGui::Checkbox("ClimbingCliffFlag", &objFighter->GetClimbingCliffFlag());
	ImGui::Checkbox("Landing", &objFighter->GetLandingFlag());
	ImGui::Checkbox("slop", &objFighter->GetSlopeFlag());
	ImGui::Checkbox("jumpWallHittingFlag", &objFighter->GetJumpWallHitFlag());
	ImGui::Checkbox("wallHittingFlag", &objFighter->GetWallHitFlag());
	ImGui::Checkbox("wallKic", &objFighter->testFlag);
	imguiManager::PosDraw();
}

void GamePlayScene::ObjectsUpdate()
{
	//�v���[���[�I�u�W�F�N�g�̃A�b�v�f�[�g
	objFighter->ObjectUpdate();
	//Json�ǂݍ��݂̃I�u�W�F�N�g�̃A�b�v�f�[�g
	JsonLoader::Update();
	//���̒��I�u�W�F�N�g�̃A�b�v�f�[�g
	OpticalPost::Update(camera->GetEye());
	//�X�J�C�h�[��
	skydomeObject->Update();
	//�J����
	camera->Update();
}

void GamePlayScene::StartStatus()
{
	//�J�n����
	if(count <= 10)
	{
		moveFlag = true;
		if (ClockTime::GetAddSecFlag() == true)
		{
			count++;
		}
		interpolationCamera.StartInterpolationCamera(camera);
		ObjectsUpdate();
		return;
	}
	else if(count >= 11 && count <= 109)
	{
		count = 110;
		moveFlag = false;
	}
}

void GamePlayScene::GameStatus()
{
	//�A�b�v�f�[�g
	camera->Update();
	lightGroup->Update();
	skydomeObject->Update();
	JsonLoader::Update();
	objFighter->Update();
	OpticalPost::Update(camera->GetEye());
	OpticalPost::SetDrawFlag(true);
	
	UI::Update();
	
	Effect::Update(camera->GetEye());
}

void GamePlayScene::GameOverStatus()
{
	if (objFighter->GetTimeLimit() <= 0.0f)
	{
		SceneManager::GetInstance()->ChangeScene("GAMEOVER");
	}
}

void GamePlayScene::ClearStatus()
{
	//�Q�[���I������
	if (objFighter->GetCrystal() == 0 && objFighter->GetGoalFlag() == true)
	{
		SceneManager::GetInstance()->ChangeScene("TITLE");
		return;
	}
}
