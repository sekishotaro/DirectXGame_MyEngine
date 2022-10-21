#pragma once
/// <summary>
/// 当たり判定プリミティブ
/// </summary>
#include <DirectXMath.h>

/// <summary>
/// 球
/// </summary>
struct Sphere
{
	//中心座標
	DirectX::XMVECTOR center = { 0,0,0,1 };
	//半径
	float radius = 1.0f;
};

/// <summary>
/// 球
/// </summary>
struct SphereF
{
	//中心座標
	DirectX::XMFLOAT3 center = { 0,0,0 };
	//半径
	float radius = 1.0f;
};

/// <summary>
/// 円
/// </summary>
struct Circle
{
	//中心座標
	DirectX::XMFLOAT2 center = { 0,0 };
	//半径
	float radius = 1.0f;
};

/// <summary>
/// 円柱
/// </summary>
struct Cylinder
{
	//中心座標
	DirectX::XMFLOAT3 center = { 0,0,0 };
	//半径
	float radius = 1.0f;
	//高さ
	float top = 1.0f;
	float bottom = -1.0f;
};



/// <summary>
/// 平面
/// </summary>
struct Plane
{
	//法線ベクトル
	DirectX::XMVECTOR normal = { 0,1,0,0 };
	//原点(0,0,0)からの距離
	float distance = 0.0f;
};

/// <summary>
/// 板ポリ
/// </summary>
struct Plate
{
	//座標
	DirectX::XMVECTOR position = {};
	//サイズ
	DirectX::XMFLOAT3 size = {};
	//法線ベクトル
	DirectX::XMVECTOR normal = { 0,1,0,0 };

	//板ポリの四角形を構成する三角ポリゴン2つの頂点6つ(Todo:4つでもできるように)
	DirectX::XMVECTOR vert1 = { 0,0,0,0 };
	DirectX::XMVECTOR vert2 = { 0,0,0,0 };
	DirectX::XMVECTOR vert3 = { 0,0,0,0 };
	DirectX::XMVECTOR vert4 = { 0,0,0,0 };
	DirectX::XMVECTOR vert5 = { 0,0,0,0 };
	DirectX::XMVECTOR vert6 = { 0,0,0,0 };
};


/// <summary>
/// 法線付き三角形 (時計回りが表面)
/// </summary>
class Triangle
{
public:
	//頂点座標3つ
	DirectX::XMVECTOR p0;
	DirectX::XMVECTOR p1;
	DirectX::XMVECTOR p2;
	//法線ベクトル
	DirectX::XMVECTOR normal;

	/// <summary>
	/// 法線の計算
	/// </summary>
	void ComputeNormal();
};

/// <summary>
/// レイ (半直線)
/// </summary>
struct Ray
{
	//始点座標
	DirectX::XMVECTOR start = { 0,0,0,1 };
	//方向
	DirectX::XMVECTOR dir = { 1,0,0,0 };
};

/// <summary>
/// 線分
/// </summary>
struct LineSegment
{
	//始点
	DirectX::XMFLOAT3 start = { 0.0f, 0.0f, 0.0f };
	//終点
	DirectX::XMFLOAT3 end = { 0.0f, 0.0f, 0.0f };
};




/// <summary>
/// 長方形
/// </summary>
struct Box
{
	DirectX::XMFLOAT3 centerPos = { 0,0,0};
	DirectX::XMFLOAT3 size = { 1.0f, 1.0f, 1.0f };
	DirectX::XMFLOAT3 MaxPos = {centerPos.x + size.x, centerPos.y + size.y ,centerPos.z + size.z };
	DirectX::XMFLOAT3 LeastPos = { centerPos.x - size.x, centerPos.y - size.y ,centerPos.z - size.z };
};