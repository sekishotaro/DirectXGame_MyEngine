#pragma once
#include "Camera.h"
#include "DebugCamera.h"
#include <DirectXMath.h>

class InterpolationCamera
{
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	void StartInterpolationCamera(DebugCamera* camera);

	void EndInterpolationCamera(DebugCamera* camera);

	static float rotaX;
	static float rotaY;
	static float dis;

};