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
		sutaminaMax,		// 0：満タン時
		sutaminaUse,		// 1：消費時
		sutaminaRecovery,	// 2：通常回復
		sutaminaBurst		// 3：スタミナバースト,回復
	};

	enum HitYStatus
	{
		onGrounds,		// 0：地形(平面)の上
		onSlope,		// 1：地形(坂)の上
		onObjects,		// 2：オブジェクトの上
		air				// 3：空中
	};

	enum HitXStatus
	{
		wall,			// 0：地形の壁
		object,			// 1：オブジェクト
		ivy,			// 2：上れる壁(蔦)※地形の壁にもあたっているが蔦を優先
		goal			// 3：ゴール
	};

public:
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
	bool MoveBoxConditionFlag();
	bool MoveStickCheck();
	bool FallStateCheck();
public:
	//ゲッター
	XMFLOAT3 GetMove() { return moveVal; };
	int GetCrystalGetNum() { return crystalGetNum; }
	bool GetGoalFlag() { return goalFlag; }
	int GetCrystalMaxNum() { return crystalMaxNum; }
	int GetCrystalNum() { return crystalNum; }
	bool GetStaminaCutFlag() { return staminaCut; }
	float GetStaminaQuantity() { return staminaQuantity; }
	float GetTimeLimit() { return timeLimit; }
	bool GetStaminaBoostFlag() { return staminaBoostFlag; }
	bool GetCrystalGetFlag() { return crystalGetFlag; }
	bool GetMoveBoxHitFlag() { return moveBoxHitFlag;}
	int GetAnimeNum() { return animeNum; }
	float GetTimeMax() { return timeLimitMax; }
	int GetInputX() { return inputX; };
	int GetInputY() { return inputY; };

	static bool GetSlopeFlag() { return slopeFlag; }
	PlayerStatus& GetStatus() { return playerStatus; }
	PlayerStatus& GetOldStatus() { return oldPlayerStatus; }

	void SetModel1(FbxModel* fbxModel) { this->fbxModels[0] = fbxModel; };
	void SetModel2(FbxModel* fbxModel) { this->fbxModels[1] = fbxModel; };
	void SetModel3(FbxModel* fbxModel) { this->fbxModels[2] = fbxModel; };
	void SetModel4(FbxModel* fbxModel) { this->fbxModels[3] = fbxModel; };
	void SetModel5(FbxModel* fbxModel) { this->fbxModels[4] = fbxModel; };
	void SetModel6(FbxModel* fbxModel) { this->fbxModels[5] = fbxModel; };
	void SetModel7(FbxModel* fbxModel) { this->fbxModels[6] = fbxModel; };
	void SetModel8(FbxModel* fbxModel) { this->fbxModels[7] = fbxModel; };
	void SetModel9(FbxModel* fbxModel) { this->fbxModels[8] = fbxModel; };
	void SetModel10(FbxModel* fbxModel) { this->fbxModels[9] = fbxModel; };
	void SetModel11(FbxModel* fbxModel) { this->fbxModels[10] = fbxModel; };
	void SetModel12(FbxModel* fbxModel) { this->fbxModels[11] = fbxModel; };
	void SetModel13(FbxModel* fbxModel) { this->fbxModels[12] = fbxModel; };
	void SetModel14(FbxModel* fbxModel) { this->fbxModels[13] = fbxModel; };
	void SetModel15(FbxModel* fbxModel) { this->fbxModels[14] = fbxModel; };
private:
	
	XMFLOAT3 parPos;					//前フレームの位置
	XMFLOAT3 moveV = {};
	XMFLOAT3 moveVal = {};				//プレイヤーの移動量
	DirectX::XMVECTOR fallV = {};		//落下ベクトル
	float moveAdjustmentNum = 1.0f;		//移動量調整数
	int inputX = 0;					//コントローラースティック入力X
	int inputY = 0;					//コントローラースティック入力Y
	int wallCount = 0;					//ツタに当たった時のめり込み時カウント
	DirectX::XMVECTOR climbNormal = {};	//壁のぼり用保存めり込み法線
	float timeLimit = 30.0f;				//現在の時間
	const float timeLimitMax = 30.0f;	//制限時間
	float forciblyRecoveryTime = 10.0f;	//スタミナがなくなった時の回復時間
	float staminaRecoveryTime = 5.0f;	//スタミナ回復待機時間
	float jumpHeightPosY = 0.0f;		//ジャンプした時に壁と接触しない高さ
	/// <summary>
	/// ステータス管理
	/// </summary>
	PlayerAnimeState PlayerState = idling;			//アニメーション
	PlayerAnimeState oldPlayerState = idling;		//前フレームのアニメーション
	PlayerStatus playerStatus = STATE_IDLING;		//プレイヤー
	PlayerStatus oldPlayerStatus = STATE_IDLING;	//前フレームのプレイヤー
	StaminaStatus staminaStatus = sutaminaMax;		//スタミナ

	//アニメーション
	int animeNum = 0;
	int oldAnimeNum = 0;
	bool animeFlag = false;
	//モデル
	static FbxModel* fbxModels[];

	//坂確認フラグ
	static bool slopeFlag;
	bool oldSlopeFlag = false;
	bool slopeRising = false;
	//のぼり用板ポリとの当たり判定
	static bool climbWallHit;
	//前フレームの当たり判定の結果
	bool climbWallHitPar = false;
	bool crystalGetFlag = false;
	int crystalGetNum = 0;
	//壁と接触確認フラグ
	bool wallHittingFlag = false;
	bool oldWallHittingFlag = false;
	bool jumpWallHittingFlag = false;
	//崖上がり
	int climbingCliffBoxNum = 0;
	//箱移動してた確認フラグ
	bool moveBoxFlag = false;
	bool moveBoxHitFlag = false;
	int moveBoxHitNum = 0;
	//接地フラグ
	bool onGround = false;
	//オブジェク接地フラグ
	bool onObject = false;
	//自機とゴールの当たり判定
	bool goalFlag = true;
	//クリスタルの数
	int crystalNum = 0;
	int crystalMaxNum = 0;
	bool staminaCut = false;
	float staminaQuantity = 100.0f;		// スタミナ残量
	// スタミナ消費確認フラグ
	bool staminaBoostFlag = false;
public:
	XMFLOAT3 moveBoxMax1 = { 0.0f, 0.0f, 0.0f };
	//debug
	bool teleportFlag = false;
	bool testFlag = false;
	bool timeLimitcancel = false;
	bool moveLimitFlag = false;
};

