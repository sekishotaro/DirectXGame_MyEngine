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
#include "Effect2d.h"

#include "SphereCollider.h"
#include "CollisionManager.h"
#include "Player.h"
#include "UI.h"
#include "OpticalPost.h"
#include "Effect.h"

#include "SafeDelete.h"

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
	camera->SetEye({ 0, 0, -30 });
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
	Sprite::LoadTexture(99, L"Resources/smoke.png");
	Sprite::LoadTexture(98, L"Resources/smoke.png");
	// �w�i�X�v���C�g����
	spriteBG = Sprite::Create(1, { 0.0f,0.0f });
	smoke = Sprite::Create(99, { 0.0f,0.0f });
	smoke->SetColor({ 1.0f,1.0f,1.0f, 0.0f });
	smoke2 = Sprite::Create(98, { 0.0f,0.0f });
	smoke2->SetColor({ 1.0f,1.0f,1.0f, 0.0f });
	skydomeModel = Model::LoadFromOBJ("skydome");
	skydomeObject = Object3d::Create();
	skydomeObject->SetModel(skydomeModel);
	skydomeObject->SetScale({ 5.0f, 5.0f, 5.0f });
	//json
	JsonLoader::LoadFile("Scene4_7"); //�I�u�W�F�N�g�̓����蔻��
	JsonLoader::SetObject();

	////���f�������w�肵�ăt�@�C���ǂݍ���
	fbxModels[0] = FbxLoader::GetInstance()->LoadModelFromFile("model");
	fbxModels[1] = FbxLoader::GetInstance()->LoadModelFromFile("model2");
	fbxModels[2] = FbxLoader::GetInstance()->LoadModelFromFile("model3");
	fbxModels[3] = FbxLoader::GetInstance()->LoadModelFromFile("model4");
	fbxModels[4] = FbxLoader::GetInstance()->LoadModelFromFile("model5");
	fbxModels[5] = FbxLoader::GetInstance()->LoadModelFromFile("model6");
	fbxModels[6] = FbxLoader::GetInstance()->LoadModelFromFile("model7");
	fbxModels[7] = FbxLoader::GetInstance()->LoadModelFromFile("model8");
	fbxModels[8] = FbxLoader::GetInstance()->LoadModelFromFile("model9");
	fbxModels[9] = FbxLoader::GetInstance()->LoadModelFromFile("model10");
	fbxModels[10] = FbxLoader::GetInstance()->LoadModelFromFile("model11");
	fbxModels[11] = FbxLoader::GetInstance()->LoadModelFromFile("model12");
	fbxModels[12] = FbxLoader::GetInstance()->LoadModelFromFile("model13");
	fbxModels[13] = FbxLoader::GetInstance()->LoadModelFromFile("model14");
	fbxModels[14] = FbxLoader::GetInstance()->LoadModelFromFile("model15");

	collisionManager = CollisionManager::GetInstance();
	objFighter = Player::Create(fbxModels[0]);
	objFighter->SetModel1(fbxModels[0]);
	objFighter->SetModel2(fbxModels[1]);
	objFighter->SetModel3(fbxModels[2]);
	objFighter->SetModel4(fbxModels[3]);
	objFighter->SetModel5(fbxModels[4]);
	objFighter->SetModel6(fbxModels[5]);
	objFighter->SetModel7(fbxModels[6]);
	objFighter->SetModel8(fbxModels[7]);
	objFighter->SetModel9(fbxModels[8]);
	objFighter->SetModel10(fbxModels[9]);
	objFighter->SetModel11(fbxModels[10]);
	objFighter->SetModel12(fbxModels[11]);
	objFighter->SetModel13(fbxModels[12]);
	objFighter->SetModel14(fbxModels[13]);
	objFighter->SetModel15(fbxModels[14]);
	
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
	lightGroup->SetCircleShadowAtten(1, XMFLOAT3(circleShadowAtten));
	lightGroup->SetCircleShadowFactorAngle(1, XMFLOAT2(circleShadowFactorAngle2));

	SmokeUpdate();
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
	smoke->Draw();
	smoke2->Draw();
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
	ImGui::Text("rotX :%f", camera->GetRotaX());
	ImGui::Text("rotY :%f", objFighter->GetRotation().y);
	ImGui::Text("posZ :%f", objFighter->GetPosition().z);
	ImGui::Text("playerState :%d", static_cast<int>(objFighter->GetStatus()));
	ImGui::Checkbox("teleport", &objFighter->teleportFlag);
	ImGui::Checkbox("TimeLimitCancel", &objFighter->timeLimitcancel);
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
}

void GamePlayScene::GameStatus()
{
	//�A�b�v�f�[�g
	objFighter->Update();
	camera->Update();
	lightGroup->Update();
	skydomeObject->Update();
	JsonLoader::Update();
	
	OpticalPost::Update(camera->GetEye());
	OpticalPost::SetDrawFlag(true);
	
	UI::Update();
	
	Effect::Update(camera->GetEye());
}

void GamePlayScene::GameOverStatus()
{
	const float timeMax = 3.0f;
	static float time = timeMax;

	if (objFighter->GetTimeLimit() <= 0.0f)
	{
		objFighter->moveLimitFlag = true;
		if (time >= 0.0f)
		{
			float alpha = (timeMax - time) / timeMax;
			smoke2->SetColor({ 1.0f,1.0f,1.0f, alpha });
			time -= 1.0f / 60.0f;
			return;
		}

		state = reStart;
		//�X�e�[�W�̕���
		JsonLoader::ClystalSetObject();
		OpticalPost::Restart();
		
		//���@�̏�����
		objFighter->ReStart();
		camera->rotaX = 180.0f;
		time = timeMax;
	}
	else
	{
		smoke2->SetColor({ 1.0f,1.0f,1.0f,0.0f });
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

void GamePlayScene::SmokeUpdate()
{
	if (smokeFlag == false)
	{
		smoke->SetColor({ 1.0f,1.0f,1.0f,0.0f });
		return;
	}
	
	static float reStartNum = 1.0f;
	if (state == reStart)
	{
		if (reStartNum <= 0.0f)
		{
			state = play;
			reStartNum = 1.0f;
			objFighter->moveLimitFlag = false;
			return;
		}

		reStartNum -= 1.0f / 180.0f;

		smoke->SetColor({ 1.0f,1.0f,1.0f, reStartNum });
		return;
	}


	//�������Ԃ�������؂����牌���o�Ă���
	float halfTime = objFighter->GetTimeMax() / 2;
	if (objFighter->GetTimeLimit() > halfTime)
	{
		smoke->SetColor({ 1.0f,1.0f,1.0f,0.0f });
		return;
	}

	//���͐������Ԕ�������0�ɂȂ�܂�
	float a = halfTime - objFighter->GetTimeLimit();

	float Rate = a / halfTime;

	smoke->SetColor({ 1.0f,1.0f,1.0f, Rate });
}
