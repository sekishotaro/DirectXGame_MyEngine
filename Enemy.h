#pragma once
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include "CollisionPrimitive.h"

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

public:
	static void SetPos(XMFLOAT3& pos1) { pos = pos1; }

	static XMFLOAT3 GetPos() { return pos; }

	static void Tracking(const XMFLOAT3& playerPos);

	static void ShieldDodge(const XMFLOAT3& playerPos, const Box& wall);

	static void Initialize();

	static void Move(const XMFLOAT3& playerPos);

	static void PushBack(const Box& wall);
};

