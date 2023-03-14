#pragma once
#include <DirectXMath.h>
#include "Camera.h"
#include "Object3d.h"
#include "BaseCollider.h"
#include "CollisionAttribute.h"

class DebugCamera : public Camera
{
public: // メンバ関数
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="window_width">画面幅</param>
	/// <param name="window_height">画面高さ</param>
	DebugCamera(int window_width, int window_height);

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~DebugCamera() = default;

	/// <summary>
	/// 毎フレーム更新
	/// </summary>
	virtual void Update();

	XMFLOAT3 SphereCoordinateSystem();

	XMFLOAT3 MoveUpdate();

	void UpdateProcess( XMFLOAT3 &cameraPos);

	void UpdateOnly();

	void RaidCameraCount();

	static float GetRotaY() { return rotaY; }

public:
	static XMFLOAT3 eye;
	static XMFLOAT3 target;
	static float dx;
	static float dy;
	static float dz;
	static XMFLOAT3 distance;
	static float rotaX;
	static float rotaY;
	static float dis;
	static bool hitFlag;

	static std::unique_ptr<Object3d> Object;
	static Model* Model;
	BaseCollider* collider = nullptr;

	bool RaidTargetCameraFlag = false;
	bool oldRaidFlag = false;
	int count = 0;
};

