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
	std::unique_ptr<Object3d> object3;
	Model *model = nullptr;
	Model* model2 = nullptr;
	Camera *camera = nullptr;

	//数学授業用

	XMFLOAT3 pos1       = {  0.0f,  0.0f,  0.0f};
	XMFLOAT3 move1      = {  0.0f,  0.0f,  0.0f};
	XMFLOAT3 direction1 = {  0.0f,  0.0f,  0.0f};
	SphereF sphere1;
	float angle = 0.0f;
	float radius = 50.0f;
	float speed = 0.0f;
	float mass = 0.1346f;
	float length = 50.0f;
	float x = (length * 3.14f * 2.0f) / 8.0f;

	XMFLOAT3 pos2       = {  1.0f,   0.0f,  0.0f };
	XMFLOAT3 pos3 = { 1.0f,   2.0f,  0.0f };

	bool startFlag      = false;
	bool colFlag        = false;

	

};

