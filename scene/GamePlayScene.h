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
private: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
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

private:

	/// <summary>
	/// オブジェクトのアップデートだけ用
	/// </summary>
	void ObjectsUpdate();

	/// <summary>
	/// スタート状態
	/// </summary>
	void StartStatus();

	/// <summary>
	/// ゲーム中状態
	/// </summary>
	void GameStatus();

	/// <summary>
	/// ゲーム状態
	/// </summary>
	void GameOverStatus();

	/// <summary>
	/// クリア状態
	/// </summary>
	void ClearStatus();

	void SmokeUpdate();
public:

	/// <summary>
	/// ゲームシーン用
	/// </summary>
	
	GameCamera* camera = nullptr;
	//Camera *camera = nullptr;
	Sprite *spriteBG = nullptr;
	Sprite* smoke = nullptr;
	Sprite* smoke2 = nullptr;
	Sprite* smokes[2] = { nullptr, nullptr};

	std::unique_ptr<Object3d> skydomeObject;
	Model* skydomeModel = nullptr;

	//オブジェクトデータ
	Player* objFighter = nullptr;

	//衝突マネージャー
	CollisionManager* collisionManager = nullptr;
	TouchableObject* objGround = nullptr;
	Effect *effect = new Effect;
	OpticalPost *opticalPost = new OpticalPost;
	StaminaUI* staminaUI = new StaminaUI();
	TimeUI* timeUI = new TimeUI();
	ControllerUI* controllerUI = new ControllerUI();
	CrystalUI* crystalUI = new CrystalUI();
	//ライト
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

