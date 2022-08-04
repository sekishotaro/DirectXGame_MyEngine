#pragma once
#include "Collision.h"
#include "ColliderObject.h"
#include <DirectXMath.h>

class CollisionSet
{
private: // �G�C���A�X
// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	static void CollisionCheck(const XMFLOAT3& pos, const XMFLOAT3& size, float& groundY);

	static void CollisionPushBack(const XMFLOAT3& size);

private:
	static bool PlayerWallCollisionFlag;
};

