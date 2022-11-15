#pragma once
#include <DirectXMath.h>
#include "Camera.h"

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
};

