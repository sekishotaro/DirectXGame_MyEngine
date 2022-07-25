#pragma once

#include "BaseScene.h"
#include "Sprite.h"
#include "Object3d.h"
#include <DirectXMath.h>
#include "Collision.h"

#include <memory>

#include "Camera.h"

class GamePlayScene : public BaseScene
{
private: // エイリアス
// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;

public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 解放
	/// </summary>
	void Finalize() override;

	/// <summary>
	/// アップデート
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;



	/// <summary>
	/// ゲームシーン用
	/// </summary>
	Sprite *spriteBG = nullptr;
	std::unique_ptr<Object3d> object1;
	std::unique_ptr<Object3d> object2;
	Model *model = nullptr;
	Model* model2 = nullptr;
	Camera *camera = nullptr;

	//数学授業用

	XMFLOAT3 pos1       = {-40.0f,  0.0f,  0.0f};
	XMFLOAT3 move1      = {  2.0f,  0.0f,  0.0f};
	XMFLOAT3 direction1 = {  1.0f,  0.0f,  0.0f};
	float resistance1   = 0.0f;
	bool flaggra1       = false;
	SphereF sphere1;

	XMFLOAT3 pos2       = {  0.0f,  0.0f,  0.0f };
	XMFLOAT3 move2      = {  0.0f,  0.0f,  0.0f };
	XMFLOAT3 direction2 = { -0.0f,  0.0f,  0.0f };
	float resistance2   = 0.0f;
	bool flaggra2       = false;
	SphereF sphere2;


	bool startFlag      = false;
	float groundY       = 0.0f;
	bool colFlag        = false;
};

