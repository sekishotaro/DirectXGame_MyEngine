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

	enum StaminaStatus
	{
		sutaminaMax,		//満タン時
		sutaminaUse,		//消費時
		sutaminaRecovery,	//通常回復
		sutaminaBurst		//スタミナバースト,回復
	};

	enum PlayerAnimeState
	{
		idling,				// 0：アイドリング
		walking,			// 1：ウォーキング
		running,			// 2：ランニング
		jump,				// 3：ジャンプ
		fall,				// 4：落下
		climbing,			// 5：壁のぼり
		landing,			// 6：着地
		hangingCliff,		// 7：崖ぶら下がりアイドリング
		kick,				// 8：キック
		sliding,			// 9：スライディング
		pushingWalking,		// 10：押し歩き
		tiredWalking,		// 11：スタミナ切れ歩き
		tiredIdling,		// 12：スタミナ切れアイドリング
		wallKickUp,			// 13：壁蹴りジャンプ
		hangingCliffUp		// 14：崖上がり
	};

	/// <summary>
	/// 5/17作成改訂版
	/// </summary>
	enum PlayerStatus
	{
		STATE_IDLING,				//00:アイドル状態
		STATE_WALKING,				//01:歩き
		STATE_RUNNING,				//02:走り
		STATE_JUMP_UP,				//03:ジャンプ上昇
		STATE_JUMP_DOWN,			//04:ジャンプ下降
		STATE_FALL,					//05:落下
		STATE_CLIMBING,				//06:壁のぼり
		STATE_LANDING,				//07:着地
		STATE_CLIFF_IDLING,			//08:崖つかみアイドル状態
		STATE_SLOPE_SLIDING,		//09:坂滑り落ち
		STATE_PUSHBOX,				//10:箱押し状態
		STATE_TIRED_IDLING,			//11:スタミナ切れ
		STATE_TIRED_WALKING,		//12:スタミナ切れ歩き
		STATE_WALLKICK_UP,			//13:壁蹴りジャンプ上昇
		STATE_WALLKICK_DOWN,		//14:壁蹴りジャンプ下降
		STATE_CLIFFUP,				//15:崖上がり
	};



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

	void Finalize();

	void ReStart();

private:
	void PushBack(const DirectX::XMVECTOR& normal, const XMFLOAT3& distance);

	void ClimbWallJudge(const DirectX::XMVECTOR move);

	/// <summary>
	/// 移動処理
	/// </summary>
	/// <param name="move"></param>
	void MoveOperation(DirectX::XMVECTOR& move, float &power);
	
	/// <summary>
	/// 崖上がり
	/// </summary>
	void MoveClimbingCliff(DirectX::XMVECTOR& move, float& power);

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

	//壁キックジャンプ
	bool climbingKickJump();

	//崖上がり
	void climbingCliff();

	//スタミナ使用不可確認
	bool StaminaUnusable();

	//移動値加算決定
	void MoveAddDetermination(DirectX::XMVECTOR& move, float& power);

	//崖上がり時の内側に移動処理
	void BoxInMove();
	//坂から立ち上がり確認
	bool SlopeRisingFlag();

	bool TimeCheck(float& time);

	void StatusProsecc();

private:
	bool StaminaConsumptionFlag();
	bool moveBoxConditionFlag();
public:
	//デバック用ゲッター
	static XMFLOAT3 GetPos() { return pos; }

	static XMFLOAT3 GetRot() { return rot; }

	static XMFLOAT3 GetMove() { return moveV; };

	static bool &GetNowMove() { return nowMove;}
	
	static bool &GetadhesionMesh() { return adhesionMesh;}

	static int GetCrystal() { return crystalNum; }

	static bool &GetGoalFlag() { return goalFlag; }

	static bool &GetClimbWallHitFlag() { return climbOperation; }

	static float GetTimeLimit() { return timeLimit; }

	static bool& GetStaminaFlag() { return staminaBoostFlag; }

	static float& GetStaminaQuantity() { return staminaQuantity; }

	static bool& GetStaminaCut() { return staminaCut; }

	static bool& GetAnimeFlag() { return animeFlag; }

	static int& GetInputNumX() { return inputX; }
	static int& GetInputNumY() { return inputY; }

	static float& GEtTestFloatNum() { return testRota; }

	static bool GetCrystalGetFlag() { return crystalGetFlag; }
	static bool GetMoveBoxHitFlag() { return moveBoxHitFlag;}
	static bool GetJumpFlag() { return jumpFlag; }
	static bool GetFallFlag() { return fallFlag; }
	static int& GetAnimeNum() { return animeNum; }

	static bool &GetSlopeFlag() { return slopeFlag; }
	bool& GetOldSlopeFlag() { return oldSlopeFlag; }
	static bool& GetWallHitFlag() { return wallHittingFlag; }
	static bool& GetJumpWallHitFlag() { return jumpWallHittingFlag; }
	static bool& GetClimbingCliffFlag() { return climbingCliffFlag; }
	static bool& GetLandingFlag() { return landingFlag; }
	static float GetTimeMax() { return timeLimitMax; }

	static PlayerStatus& GetStatus() { return playerStatus; }
	static PlayerStatus& GetOldStatus() { return oldPlayerStatus; }

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
	void SetModel14(FbxModel* fbxModel) { this->fbxModel14 = fbxModel; };
	void SetModel15(FbxModel* fbxModel) { this->fbxModel15 = fbxModel; };

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
	bool oldSlopeFlag = false;
	bool slopeRising = false;
	//ジャンプ確認フラグ
	static bool jumpFlag;
	static bool wallKickUpFlag;
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
	static bool moveBoxHitFlag;
	int moveBoxHitNum = 0;
	//壁と接触確認フラグ
	static bool wallHittingFlag;
	static bool oldWallHittingFlag;
	static bool jumpWallHittingFlag;
	//ジャンプした時に壁と接触しない高さ
	float jumpHeightPosY = 0.0f;
	//崖上がり
	static bool climbingCliffFlag;
	static bool climbingCliffUpFlag;
	int climbingCliffBoxNum = 0;
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
	static FbxModel* fbxModel7; //着地
	static FbxModel* fbxModel8; //崖ぶら下がりアイドリング
	static FbxModel* fbxModel9; //キック
	static FbxModel* fbxModel10; //スライディング
	static FbxModel* fbxModel11; //押し歩き
	static FbxModel* fbxModel12; //スタミナ切れ歩き
	static FbxModel* fbxModel13; //スタミナ切れアイドリング
	static FbxModel* fbxModel14; //壁蹴りジャンプ
	static FbxModel* fbxModel15; //崖上がり

	PlayerAnimeState PlayerState = idling;

	static PlayerStatus playerStatus;
	static PlayerStatus oldPlayerStatus;

public:
	XMFLOAT3 moveBoxMax1 = { 0.0f, 0.0f, 0.0f };
	bool movingFlag = false;
	//debug
	bool teleportFlag = false;
	bool testFlag = false;
	bool timeLimitcancel = false;
};

