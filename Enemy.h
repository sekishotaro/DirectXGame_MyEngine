#pragma once
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include "CollisionPrimitive.h"

#include "MathObject.h"

class Enemy
{
private:
	// Microsoft::WRL::Çè»ó™
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::Çè»ó™
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

private:
	static XMFLOAT3 pos;
	static XMFLOAT3 initialPos;

	static int nearNumber;
	static float speedConstant;

	static bool raidMode;

	static float monitoringEnemyColliderScale;
	static bool chengeScaleFlag;

	static bool gameOverFlag;

public:

	static void SetPos(XMFLOAT3& pos1) { pos = pos1; }

	static XMFLOAT3 GetPos() { return pos; }

	static int GetNearNum() { return nearNumber; }

	static bool GetGameOver() { return gameOverFlag; }

	static void Tracking(const XMFLOAT3& playerPos);

	static void ShieldDodge(const XMFLOAT3& playerPos, const Box& wall);

	static void Initialize();

	static void Move(const XMFLOAT3& playerPos);

	static void PushBack(const Box& wall);

	static void Update(int time, const XMFLOAT3& playerPos);

	static XMFLOAT3 MonitoringCollisionScale();
};

