#pragma once

#include "BaseScene.h"
#include "Sprite.h"
#include "Object3d.h"
#include "ColliderObject.h"
#include "FbxObject3d.h"
#include <DirectXMath.h>

#include <memory>
#include "DebugCamera.h"
#include "Camera.h"

#include "Player.h"

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
	Sprite *spriteBG = nullptr;
	std::unique_ptr<Object3d> objectX;
	Model *model = nullptr;
	DebugCamera* camera = nullptr;
	//Camera *camera = nullptr;
	FbxModel *fbxModel1 = nullptr;
	FbxObject3d *fbxObject1 = nullptr;

	Player *player = nullptr;
};

