#pragma once
#include <DirectXMath.h>
#include "Camera.h"
#include "Object3d.h"
#include "BaseCollider.h"
#include "CollisionAttribute.h"

class GameCamera : public Camera
{
public: // メンバ関数
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="window_width">画面幅</param>
	/// <param name="window_height">画面高さ</param>
	GameCamera(int window_width, int window_height);

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~GameCamera() = default;

	/// <summary>
	/// 毎フレーム更新
	/// </summary>
	virtual void Update();
	/// <summary>
	/// 球面座標系の位置決定
	/// </summary>
	/// <returns>カメラ位置</returns>
	XMFLOAT3 SphereCoordinateSystem();
	/// <summary>
	/// カメラ移動処理
	/// </summary>
	/// <returns>カメラ位置</returns>
	XMFLOAT3 MoveUpdate();
	/// <summary>
	/// カメラ操作
	/// </summary>
	void Operation();
	/// <summary>
	/// カメラ地形当たり判定
	/// </summary>
	/// <param name="cameraPos">押し返し</param>
	void TerrainPushBackProcess( XMFLOAT3 &cameraPos);
	/// <summary>
	/// 
	/// </summary>
	void UpdateOnly();
	/// <summary>
	/// 注目処理
	/// </summary>
	void NoticeProcess();
	/// <summary>
	/// 注目角度取得
	/// </summary>
	/// <returns>プレイヤーの向いている角度</returns>
	float GetNoticeRot();
	bool PlayerJumpUp();
	/// <summary>
	/// カメラターゲット座標の
	/// </summary>
	XMFLOAT3 TargetProcess();
	/// <summary>
	/// 崖つかみした瞬間から補間
	/// </summary>
	/// <returns></returns>
	float CliffMoveTargetState();
	/// <summary>
	/// 崖上がりタイミング取得
	/// </summary>
	void CliffFlagUpdate();
	/// <summary>
	/// カメラ移動ディレイ
	/// </summary>
	void CorrectionProcess();
	/// <summary>
	/// ディレイ確認
	/// </summary>
	/// <returns>ディレイをするか</returns>
	bool CorrectionCheck();
	/// <summary>
	/// 地面によるカメラの補正
	/// </summary>
	void SlopeRotaYProcess();
	/// <summary>
	/// 坂の上の処理
	/// </summary>
	void OnSlopeProcess();
	/// <summary>
	/// 坂の上でない処理
	/// </summary>
	void UnSlopeProcess();

	float leap(float rotaA, float rotaB, float timeRatio);

	float AngleNormalize(const float rot);
public:
	static XMFLOAT3 eye;
	static XMFLOAT3 target;
	static float dx;
	static float dy;
	static float dz;
	XMFLOAT2 rota = { 270.0f, 70.0f };
	static float dis;
	float correctionDis = 0.0f;
	XMFLOAT3 correctionVal = {};
	static bool hitFlag;

	static std::unique_ptr<Object3d> Object;
	float oldPosY = 0.0f;
	static Model* Model;
	BaseCollider* collider = nullptr;

	bool viewpointSwitchFlag = false;
	float viewpointSwitchposParRotX = 0.0f;
	float viewpointSwitchposParRotY = 0.0f;
	float viewpointSwitchposParDis = 0.0f;

	XMFLOAT3 oldTargetPos = {};

	bool cliffTargetFlag = false;

	float cliffTargetCount = 1.0f;
	float movePreviousPosY = 0.0f;
	float moveAftaerPosY = 0.0f;

	bool slopeRotaFlag = true;
};

