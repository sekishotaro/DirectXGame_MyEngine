#pragma once

#include "BaseScene.h"
#include "Sprite.h"
#include "Object3d.h"
#include "GameCamera.h"
#include "Camera.h"
#include "LightGroup.h"
#include <DirectXMath.h>

class TitleScene : public BaseScene
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
	GameCamera* camera = nullptr;
	Sprite *spriteBG = nullptr;
	Sprite* spriteUI = nullptr;

	LightGroup* lightGroup = nullptr;
	float circleShadowDir[3] = { 0,-1,0 };
	float circleShadowPos[3] = { 1,5,0 };
	float circleShadowAtten[3] = { 0.5f,0.6f,0.0f };
	float circleShadowFactorAngle[2] = { 0.0f, 0.5f };
	float circleShadowFactorAngle2[2] = { 0.0f, 2.0f };


	std::unique_ptr<Object3d> skydomeObject;
	Model* skydomeModel = nullptr;

	std::unique_ptr<Object3d> terrainObjectA;
	Model* terrainModelA = nullptr;
	std::unique_ptr<Object3d> terrainObjectB;
	Model* terrainModelB = nullptr;
	std::unique_ptr<Object3d> terrainObjectC;
	Model* terrainModelC = nullptr;

	FbxModel* fbxModel = nullptr;
	FbxObject3d* fbxObject = nullptr;
};