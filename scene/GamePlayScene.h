#pragma once

#include "BaseScene.h"
#include "Sprite.h"
#include "Object3d.h"
#include <DirectXMath.h>

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
	float x1 = -40.0f;
	float move1X = 1.0f;
	float y1 = 0.0f;
	float z1 = 0.0f;
	float gravity = 9.8f;
	float dropValue =  -600.0f / 600.0f;
	float move1XValue = 1.0f;
	float d1a = 0.0f;
	float d1b = 0.0f;
	float k = 0.01; //空気抵抗比例定数


	float x2 = 40.0f;
	float y2 = 0.0f;
	float z2 = 0.0f;
	float move2XValue = 1.0f;
	float move2X = 1.0f;
	float k2 = 0.01f;

	float aro1 = 1.0;
	float aro2 = -1.0;

	bool flag = false;

	bool startFlag = false;

	float m1 = 1.001f;
	float m2 = 1.0f;
};

