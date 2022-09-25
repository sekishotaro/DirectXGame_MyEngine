#pragma once
#include "Collision.h"
#include "ColliderObject.h"
#include <DirectXMath.h>

class CollisionSet
{
private: // エイリアス
// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	static void CollisionCheck(const XMFLOAT3& pos, const XMFLOAT3& size, float& groundY);

	static void CollisionPushBack(const XMFLOAT3& size, float& groundY);

	static bool CollisionCheck1(const XMFLOAT3& pos1, const XMFLOAT3& size1, const XMFLOAT3& pos2, const XMFLOAT3& size2);

	static XMFLOAT3 GetNearPos2D(const XMFLOAT3& pos, const Box& box);


	static XMFLOAT3 GetNearVertex(const Box& box, const XMFLOAT3& pos);

	static XMFLOAT3 GetScecondNearVertex(const Box& box, const XMFLOAT3& pos);

	static XMFLOAT3 GetThirdNearVertex(const Box& box, const XMFLOAT3& pos);

	static XMFLOAT3 GetFourthNearVertex(const Box& box, const XMFLOAT3& pos);

	static XMFLOAT3 GetwhichNearPos(const XMFLOAT3& criteriaPos, const XMFLOAT3& posA, const XMFLOAT3& posB);

	static int SelectNearRoot(const XMFLOAT3& startPos, const XMFLOAT3& endPos, const XMFLOAT3& pos1, const XMFLOAT3& pos2);

private:
	static bool PlayerWallCollisionFlag;
};

