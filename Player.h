#pragma once

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>

#include "Input.h"
#include "FbxObject3d.h"
#include "Object3d.h"

class Player : public FbxObject3d
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
	static Player* Create(FbxModel* model = nullptr);

public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns>成否</returns>
	void Initialize() override;

	/// <summary>
	/// マイフレーム処理
	/// </summary>
	void Update() override;

	/// <summary>
	/// 衝突時コールバック関数
	/// </summary>
	/// <param name="info">衝突情報</param>
	void OnCollision(const CollisionInfo& info) override;

	/// <summary>
	/// オブジェクトのアップデート
	/// </summary>
	void ObjectUpdate();

private:
	void PushBack(const DirectX::XMVECTOR& normal, const XMFLOAT3& distance);

	void ClimbWallJudge(const DirectX::XMVECTOR move);

	/// <summary>
	/// 移動処理
	/// </summary>
	/// <param name="move"></param>
	void MoveOperation(DirectX::XMVECTOR& move, float &power);

	/// <summary>
	/// 通常移動処理
	/// </summary>
	/// <param name="move"></param>
	void MoveNormal(DirectX::XMVECTOR& move, float &power);

	/// <summary>
	/// 壁のぼり移動処理
	/// </summary>
	/// <param name="move"></param>
	void MoveClimb(DirectX::XMVECTOR& move, float &power);

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
	void StaminaManagement();

	//箱移動処理
	void MoveBoxProcess(DirectX::XMVECTOR& move, float &power);

	//時間管理
	void TimeManagement();

	//アニメーション
	void AnimetionProcess();

	//坂下り当たり判定
	void SlopeDownhill(DirectX::XMVECTOR& move, float& power);

	bool StaminaUnusable();

public:
	//デバック用ゲッター
	static XMFLOAT3 GetPos() { return pos; }

	static XMFLOAT3 GetRot() { return rot; }

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

	static bool& GetAnimeFlag() { return animeFlag; }

	static int& GetInputNumX() { return inputX; }
	static int& GetInputNumY() { return inputY; }

	static float& GEtTestFloatNum() { return testRota; }

	static bool GetCrystalGetFlag() { return crystalGetFlag; }

	static bool GetJumpFlag() { return jumpFlag; }

	static int& GetAnimeNum() { return animeNum; }

	static bool &GetSlopeFlag() { return slopeFlag; }

	static float GetTimeMax() { return timeLimitMax; }

	void SetModel1(FbxModel* fbxModel) { this->fbxModel1 = fbxModel; };
	void SetModel2(FbxModel* fbxModel) { this->fbxModel2 = fbxModel; };
	void SetModel3(FbxModel* fbxModel) { this->fbxModel3 = fbxModel; };
	void SetModel4(FbxModel* fbxModel) { this->fbxModel4 = fbxModel; };
	void SetModel5(FbxModel* fbxModel) { this->fbxModel5 = fbxModel; };
	void SetModel6(FbxModel* fbxModel) { this->fbxModel6 = fbxModel; };
	void SetModel7(FbxModel* fbxModel) { this->fbxModel7 = fbxModel; };
	void SetModel8(FbxModel* fbxModel) { this->fbxModel8 = fbxModel; };
	void SetModel9(FbxModel* fbxModel) { this->fbxModel9 = fbxModel; };
	void SetModel10(FbxModel* fbxModel) { this->fbxModel10 = fbxModel; };
	void SetModel11(FbxModel* fbxModel) { this->fbxModel11 = fbxModel; };
	void SetModel12(FbxModel* fbxModel) { this->fbxModel12 = fbxModel; };
	void SetModel13(FbxModel* fbxModel) { this->fbxModel13 = fbxModel; };
private:
	//全フレームの位置
	XMFLOAT3 parPos;
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
	//坂確認フラグ
	static bool slopeFlag;

	//ジャンプ確認フラグ
	static bool jumpFlag;
	static bool fallFlag;
	static bool landingFlag;

	//自機とゴールの当たり判定
	static bool goalFlag;

	//落下ベクトル
	DirectX::XMVECTOR fallV = {};
	//プレイヤーの位置

	static XMFLOAT3 pos;
	static XMFLOAT3 rot;
	static XMFLOAT3 moveV;

	//移動量調整数
	static float moveAdjustmentNum;


	static int inputX;
	static int inputY;

	//確認用
	static float testRota;

	//クリスタルの数
	static int crystalNum;

	//ツタに当たった時のめり込み時カウント
	int wallCount = 0;

	//壁のぼり状態
	static bool climbOperation;
	static bool oldClimbOperation;
	//壁のぼり用保存めり込み法線
	DirectX::XMVECTOR climbNormal = {};
	//のぼり用板ポリとの当たり判定
	static bool climbWallHit;
	//前フレームの当たり判定の結果
	bool climbWallHitPar = false;
	
	//制限時間
	static float timeLimit;
	static const float timeLimitMax;
	float flame = 0.0f;


	/// <summary>
	/// スタミナ消費確認フラグ
	/// </summary>
	static bool staminaBoostFlag;
	/// <summary>
	/// スタミナ残量
	/// </summary>
	static float staminaQuantity;
	//スタミナがなくなった時の回復時間
	float forciblyRecoveryTime = 10.0f;
	//スタミナ回復待機時間
	float staminaRecoveryTime = 5.0f;
	//スタミナが使えるかどうか(スタミナを使い切った場合になる状態)
	static bool staminaCut;
	static bool crystalGetFlag;
	//箱移動してた確認フラグ
	static bool moveBoxFlag;

	//アニメーション
	static int animeNum;
	static int oldAnimeNum;
	static bool animeFlag;

	//モデル
	static FbxModel* fbxModel1;	//基本
	static FbxModel* fbxModel2;	//ウォーキング
	static FbxModel* fbxModel3;	//ランニング
	static FbxModel* fbxModel4; //ジャンプ
	static FbxModel* fbxModel5;	//走りジャンプ
	static FbxModel* fbxModel6;	//クライミング
	static FbxModel* fbxModel7; //崖上がり
	static FbxModel* fbxModel8; //ダンス
	static FbxModel* fbxModel9; //キック
	static FbxModel* fbxModel10; //スライディング
	static FbxModel* fbxModel11; //押し歩き
	static FbxModel* fbxModel12; //疲れ歩き
	static FbxModel* fbxModel13; //疲れ歩き
};

