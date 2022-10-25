#pragma once

#include <memory>
#include <DirectXMath.h>

#include "BaseScene.h"
#include "Sprite.h"
#include "PostEffect.h"
#include "Object3d.h"
#include "MathObject.h"
#include "ColliderObject.h"
#include "FbxObject3d.h"
#include "Collision.h"
#include "DebugCamera.h"
#include "Camera.h"
#include "Light.h"

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
	/// 影用描画
	/// </summary>
	void ShadowDraw() override;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
	
	DebugCamera* camera = nullptr;
	//Camera *camera = nullptr;
	Light* light = nullptr;
	Sprite *spriteBG = nullptr;
	
	//std::unique_ptr<Object3d> objectX;
	//Model *model = nullptr;
	//FbxModel *fbxModel1 = nullptr;
	//FbxObject3d *fbxObject1 = nullptr;
	//FbxModel* fbxModel2 = nullptr;
	//FbxObject3d* fbxObject2 = nullptr;
	//std::unique_ptr<ColliderObject> colliderObject;
	//ColliderModel *colliderModel;
	//bool graFlag = true;
	//float groundY = 0.0f;
	//std::unique_ptr<MathObject> enemyCollider1Object;
	//std::unique_ptr<MathObject> enemyCollider2Object;
	//MathModel* mathModel = nullptr;
	//Box Pbox;

	//モデルデータ
	Model* modelFighter = nullptr;
	//オブジェクトデータ
	Player* objFighter = nullptr;

	//衝突マネージャー
	CollisionManager* collisionManager = nullptr;
	TouchableObject* objGround = nullptr;
};

