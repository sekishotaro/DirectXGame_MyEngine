#pragma once
#include <DirectXMath.h>
#include "Camera.h"
#include "Object3d.h"
#include "BaseCollider.h"
#include "CollisionAttribute.h"
#include "Player.h"

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
	virtual void Update(Player* player);
	/// <summary>
	/// 球面座標系の位置決定
	/// </summary>
	/// <returns>カメラ位置</returns>
	XMFLOAT3 SphereCoordinateSystem(Player* player);
	/// <summary>
	/// カメラ移動処理
	/// </summary>
	/// <returns>カメラ位置</returns>
	XMFLOAT3 MoveUpdate(Player* player);
	/// <summary>
	/// カメラ操作
	/// </summary>
	void Operation(Player* player);
	/// <summary>
	/// カメラ地形当たり判定
	/// </summary>
	/// <param name="cameraPos">押し返し</param>
	void TerrainPushBackProcess( XMFLOAT3 &cameraPos, Player* player);
	/// <summary>
	/// 注目処理
	/// </summary>
	void NoticeProcess(Player* player);
	/// <summary>
	/// 注目角度取得
	/// </summary>
	/// <returns>プレイヤーの向いている角度</returns>
	float GetNoticeRot(Player* player);
	bool PlayerJumpUp(Player* player);
	/// <summary>
	/// カメラターゲット座標の
	/// </summary>
	XMFLOAT3 TargetProcess(Player* player);
	/// <summary>
	/// 崖つかみした瞬間から補間
	/// </summary>
	/// <returns></returns>
	float CliffMoveTargetState();
	/// <summary>
	/// 崖上がりタイミング取得
	/// </summary>
	void CliffFlagUpdate(Player* player);
	/// <summary>
	/// カメラ移動ディレイ
	/// </summary>
	void CorrectionProcess(Player* player);
	/// <summary>
	/// ディレイ確認
	/// </summary>
	/// <returns>ディレイをするか</returns>
	bool CorrectionCheck(Player* player);
	/// <summary>
	/// 地面によるカメラの補正
	/// </summary>
	void SlopeRotaYProcess(Player* player);
	/// <summary>
	/// 坂の上の処理
	/// </summary>
	void OnSlopeProcess(Player* player);
	/// <summary>
	/// 坂の上でない処理
	/// </summary>
	void UnSlopeProcess();

	float leap(float rotaA, float rotaB, float timeRatio);

	float AngleNormalize(const float rot);
public:

	XMFLOAT3 eye = {};
	XMFLOAT3 target = {};
	float dx = 0;
	float dy = 0;
	float dz = 0;
	XMFLOAT2 rota = { 270.0f, 70.0f };
	float dis = 20.0f;
	float correctionDis = 0.0f;
	XMFLOAT3 correctionVal = {};
	bool hitFlag = false;

	std::unique_ptr<Object3d> Object;
	float oldPosY = 0.0f;
	Model* Model = nullptr;
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

