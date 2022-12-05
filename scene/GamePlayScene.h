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
#include "DebugCamera.h"
#include "Camera.h"
#include "LightGroup.h"

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

	/// <summary>
	/// �Q�[���V�[���p
	/// </summary>
	
	DebugCamera* camera = nullptr;
	//Camera *camera = nullptr;
	Sprite *spriteBG = nullptr;
	
	//�I�u�W�F�N�g
	std::vector<std::unique_ptr<MathObject>> enemyColliderObjects;
	//���f��
	MathModel* modelEnemyCollider = nullptr;

	std::unique_ptr<Object3d> skydomeObject;
	Model* skydomeModel = nullptr;

	//���f���f�[�^
	Model* modelFighter = nullptr;
	//�I�u�W�F�N�g�f�[�^
	Player* objFighter = nullptr;

	//�Փ˃}�l�[�W���[
	CollisionManager* collisionManager = nullptr;
	TouchableObject* objGround = nullptr;

	//���C�g
	LightGroup* lightGroup = nullptr;
	float ambientColor0[3] = { 1,1,1 };

	float circleShadowDir[3] = { 0,-1,0 };
	float circleShadowPos[3] = { 1,5,0 };
	float circleShadowAtten[3] = { 0.5f,0.6f,0.0f };
	float circleShadowFactorAngle[2] = { 0.0f, 0.5f };
	float circleShadowFactorAngle2[2] = { 0.0f, 2.0f };
	FbxModel* fbxModel = nullptr;
};

