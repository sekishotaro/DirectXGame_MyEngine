#pragma once

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>

#include "Input.h"
#include "FbxObject3d.h"
#include "Object3d.h"

class Player : public Object3d
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
	/// 3Dオブジェクト生成
	/// </summary>
	/// <returns>インスタンス生成</returns>
	static Player* Create(Model* model = nullptr);

public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns>成否</returns>
	bool Initialize() override;

	/// <summary>
	/// マイフレーム処理
	/// </summary>
	void Update() override;

	/// <summary>
	/// 衝突時コールバック関数
	/// </summary>
	/// <param name="info">衝突情報</param>
	void OnCollision(const CollisionInfo& info) override;


private:
	void PushBack(const DirectX::XMVECTOR& normal, const XMFLOAT3& distance);

	void ClimbWallJudge(const DirectX::XMVECTOR move);

	/// <summary>
	/// 移動処理
	/// </summary>
	void MoveOperation(DirectX::XMVECTOR& move);

	/// <summary>
	/// クリスタル処理
	/// </summary>
	void CrystalConfirmationProcess();

	/// <summary>
	/// ゴール接触確認処理
	/// </summary>
	void GoalConfirmationProcess();

	/// <summary>
	/// 障害物接触処理
	/// </summary>
	void ObstacleConfirmationProcess(const DirectX::XMVECTOR& move);

	/// <summary>
	/// 重力処理
	/// </summary>
	void GravityConfirmationProcess();

	/// <summary>
	/// 地形接触判定(メッシュコライダー)
	/// </summary>
	void TerrainConfirmationProcess();

	//スタミナ管理
	void StaminaManagement(const DirectX::XMVECTOR& move);

	//時間管理
	void TimeManagement();

public:
	//デバック用ゲッター
	static XMFLOAT3 GetPos() { return pos; }

	static XMFLOAT3 GetMove() { return moveV; };

	static bool &GetNowMove() { return nowMove;}
	
	static bool &GetadhesionMesh() { return adhesionMesh;}

	static int GetCrystal() { return crystalNum; }

	static bool &GetGoalFlag() { return goalFlag; }

	static bool &GetWallHitFlag() { return climbOperation; }

	static float GetTimeLimit() { return timeLimit; }

	static bool& GetStaminaFlag() { return staminaBoostFlag; }

	static float& GetStaminaQuantity() { return staminaQuantity; }

	static bool& GetStaminaCut() { return staminaCut; }

	static int& GetInputNumX() { return inputX; }
	static int& GetInputNumY() { return inputY; }

private:
	//接地フラグ
	static bool onGround;
	//接着フラグ
	static bool adhesionMesh;

	//障害物接着フラグ
	bool adhesion = false;
	//オブジェク接地フラグ
	bool onObject = false;
	//移動中確認フラグ
	static bool nowMove;

	//自機とゴールの当たり判定
	static bool goalFlag;

	//落下ベクトル
	DirectX::XMVECTOR fallV = {};
	//プレイヤーの位置

	static XMFLOAT3 pos;
	static XMFLOAT3 moveV;

	//移動量調整数
	static float moveAdjustmentNum;


	static int inputX;
	static int inputY;

	//クリスタルの数
	static int crystalNum;

	//ツタに当たった時のめり込み時カウント
	int wallCount = 0;

	static bool climbOperation;
	//壁のぼり用保存めり込み法線
	DirectX::XMVECTOR climbNormal = {};
	//のぼり用板ポリとの当たり判定
	static bool climbWallHit;
	//前フレームの当たり判定の結果
	bool climbWallHitPar = false;
	
	//制限時間
	static float timeLimit;
	float flame = 0.0f;


	//スタミナを使うか
	static bool staminaBoostFlag;
	//スタミナ残量
	static float staminaQuantity;
	//スタミナがなくなった時の回復時間
	float forciblyRecoveryTime = 10.0f;
	//スタミナが使えるかどうか(スタミナを使い切った場合になる状態)
	static bool staminaCut;
};

