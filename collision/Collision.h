#pragma once

#include "CollisionPrimitive.h"
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>

/// <summary>
/// 当たり判定ヘルパークラス
/// </summary>
class Collision
{
private: // エイリアス
// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;

public:
	/// <summary>
	/// 球と平面の当たり判定
	/// </summary>
	/// <param name="sphere">球</param>
	/// <param name="plane">平面</param>
	/// <param name="inter">交点</param>
	/// <returns>交差しているか否か</returns>
	static bool CheckSphere2Plane(const Sphere &sphere, const Plane &plane, DirectX::XMVECTOR *inter = nullptr);

	/// <summary>
	/// 球と球の当たり判定
	/// </summary>
	/// <param name="sphere1">球</param>
	/// <param name="sphere2">球</param>
	/// <returns></returns>
	static bool CheckSphereSphere(const SphereF& sphere1, const SphereF& sphere2);

	static bool CheckSphere2Sphere(const Sphere& sphere1, const Sphere& sphere2, DirectX::XMVECTOR* inter = nullptr, DirectX::XMVECTOR* reject = nullptr);

	/// <summary>
	/// 点と三角形の最近接点を求める
	/// </summary>
	/// <param name="point">点</param>
	/// <param name="triangle">三角形</param>
	/// <param name="closest">最近接点(出力用)</param>
	static void ClosestPtPoint2Triangle(const DirectX::XMVECTOR &point, const Triangle &triangle, DirectX::XMVECTOR *closest);

	/// <summary>
	/// 球と法線付き三角形の当たりチェック
	/// </summary>
	/// <param name="sphere">球</param>
	/// <param name="triangle">三角形</param>
	/// <param name="inter">交点 (三角形上の最近接点)</param>
	/// <param name="reject"></param>
	/// <returns>交差しているか否か</returns>
	static bool CheckSphere2Triangle(const Sphere &sphere, const Triangle &triangle, DirectX::XMVECTOR* inter = nullptr, DirectX::XMVECTOR* reject = nullptr);

	/// <summary>
	/// レイと平面の当たり判定
	/// </summary>
	/// <param name="ray">レイ</param>
	/// <param name="plane">平面</param>
	/// <param name="distance">距離 (出力用)</param>
	/// <param name="inter">交点 (出力用)</param>
	/// <returns>交差しているか否か</returns>
	static bool CheckRay2Plane(const Ray &ray, const Plane &plane, float *distance = nullptr, DirectX::XMVECTOR *inter = nullptr);

	/// <summary>
	/// レイと法線付き三角形の当たり判定
	/// </summary>
	/// <param name="ray">レイ</param>
	/// <param name="triangle">三角形</param>
	/// <param name="distance">距離 (出力用)</param>
	/// <param name="inter">交差 (出力用)</param>
	/// <returns>交差しているか否か</returns>
	static bool CheckRay2Triangle(const Ray &ray, const Triangle &triangle, float *distance = nullptr, DirectX::XMVECTOR *inter = nullptr);

	/// <summary>
	/// レイと球の当たり判定
	/// </summary>
	/// <param name="ray">レイ</param>
	/// <param name="sphere">球</param>
	/// <param name="distance">距離 (出力用)</param>
	/// <param name="inter">交点 (出力用)</param>
	/// <returns>交差しているか否か</returns>
	static bool CheckRay2Sphere(const Ray &ray, const Sphere &sphere, float *distance = nullptr, DirectX::XMVECTOR *inter = nullptr);

	/// <summary>
	/// 線分と長方形の当たり判定
	/// </summary>
	/// <param name="line">線分</param>
	/// <param name="box"></param>
	/// <returns></returns>
	static bool CheckLineSegmentBox(const LineSegment& line, const Box& box);

	/// <summary>
	/// レイと立方体の当たり判定
	/// </summary>
	/// <param name="ray"></param>
	/// <param name="box"></param>
	/// <returns></returns>
	static bool CheckRayBox(const Ray& ray, const Box& box);


	/// <summary>
	/// レイと立方体の当たり判定
	/// </summary>
	/// <param name="ray">レイ</param>
	/// <param name="box">立方体</param>
	/// <param name="distance">めり込むまでの距離</param>
	/// <param name="inter">一番最初に接触した面との接触箇所</param>
	/// <returns>最初に侵入した面の法線</returns>
	static XMVECTOR CheckRayBoxforPlane(const Ray& ray, const Box& box);

	/// <summary>
	/// ボックス同士の当たり判定AABB
	/// </summary>
	/// <param name="box1">立方体</param>
	/// <param name="box2">立方体</param>
	/// <param name="distance">距離(出力用)</param>
	/// <returns>交差しているか否か</returns>
	static bool Check2Box(const Box& box1, const Box& box2, XMFLOAT3& distance);

	static bool CheckBoxDot(const Box& box, const XMFLOAT3& dot);
	static bool CheckQuadrangleDot(const Quadrangle& quadrangle, const XMFLOAT2& dot);

	static bool Check2LineSegment(const LineSegment& line1, const LineSegment& line2);

	static bool CheckPlateRay(const Plate& plate, const Ray& ray);

	static bool CheckSphereBox(const SphereF& sphere, const Box& box);
	static bool CheckAABB(const Box& box1, const Box& box2);

	static bool CheckCircleDot(const Circle& circle, const XMFLOAT2& dot);
	
	static bool CheckCylinderDotXY(const Cylinder& cylinder, const XMFLOAT3& dot);
	static bool CheckCylinderDotXZ(const Cylinder& cylinder, const XMFLOAT3& dot);
	static bool CheckCylinderDotZY(const Cylinder& cylinder, const XMFLOAT3& dot);
	static bool CheckSphereDot(const SphereF& sphere, XMFLOAT3& dot);
};