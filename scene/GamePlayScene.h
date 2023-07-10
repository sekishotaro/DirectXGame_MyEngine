#pragma once

#include <memory>
#include <DirectXMath.h>

#include "BaseScene.h"
#include "Sprite.h"
#include "Object3d.h"
#include "MathObject.h"
#include "ColliderObject.h"
#include "FbxObject3d.h"
#include "Collision.h"
#include "GameCamera.h"
#include "Camera.h"
#include "LightGroup.h"
#include "Effect.h"
#include "UI.h"
#include "OpticalPost.h"

class CollisionManager;
class Player;
class TouchableObject;

class GamePlayScene : public BaseScene
{
private: // �G�C���A�X
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;

	enum State
	{
		play,
		timeOver,
		clear,
		reStart,
	};

public:

	/// <summary>
	/// ������
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// ���
	/// </summary>
	void Finalize() override;

	/// <summary>
	/// �A�b�v�f�[�g
	/// </summary>
	void Update() override;

	/// <summary>
	/// �`��
	/// </summary>
	void Draw() override;

private:

	/// <summary>
	/// �I�u�W�F�N�g�̃A�b�v�f�[�g�����p
	/// </summary>
	void ObjectsUpdate();

	/// <summary>
	/// �X�^�[�g���
	/// </summary>
	void StartStatus();

	/// <summary>
	/// �Q�[�������
	/// </summary>
	void GameStatus();

	/// <summary>
	/// �Q�[�����
	/// </summary>
	void GameOverStatus();

	/// <summary>
	/// �N���A���
	/// </summary>
	void ClearStatus();

	void SmokeUpdate();
public:

	/// <summary>
	/// �Q�[���V�[���p
	/// </summary>
	
	GameCamera* camera = nullptr;
	//Camera *camera = nullptr;
	Sprite *spriteBG = nullptr;
	Sprite* smoke = nullptr;
	Sprite* smoke2 = nullptr;
	Sprite* smokes[2] = { nullptr, nullptr};

	std::unique_ptr<Object3d> skydomeObject;
	Model* skydomeModel = nullptr;

	//�I�u�W�F�N�g�f�[�^
	Player* objFighter = nullptr;

	//�Փ˃}�l�[�W���[
	CollisionManager* collisionManager = nullptr;
	TouchableObject* objGround = nullptr;
	Effect *effect = new Effect;
	OpticalPost *opticalPost = new OpticalPost;
	StaminaUI* staminaUI = new StaminaUI();
	TimeUI* timeUI = new TimeUI();
	ControllerUI* controllerUI = new ControllerUI();
	CrystalUI* crystalUI = new CrystalUI();
	//���C�g
	LightGroup* lightGroup = nullptr;
	float ambientColor0[3] = { 1,1,1 };

	float circleShadowDir[3] = { 0,-1,0 };
	float circleShadowPos[3] = { 1,5,0 };
	float circleShadowAtten[3] = { 0.5f,0.6f,0.0f };
	float circleShadowFactorAngle[2] = { 0.0f, 0.5f };
	float circleShadowFactorAngle2[2] = { 0.0f, 2.0f };
	FbxModel* fbxModels[15] = { nullptr, nullptr, nullptr, nullptr, nullptr,
	nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, };

	bool moveFlag = false;
	bool smokeFlag = true;
	int count = 0;

	State state = play;
};

