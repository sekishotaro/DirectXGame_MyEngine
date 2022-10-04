#pragma once
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>

class MyMath
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

	MyMath();

	~MyMath();

	/// <summary>
	/// 直線運動
	/// </summary>
	/// <param name="pos">対象の座標</param>
	/// <param name="move">移動量</param>
	static void Movement(XMFLOAT3& pos, XMFLOAT3& move);

	/// <summary>
	/// 直線運動
	/// </summary>
	/// <param name="pos">対象の座標</param>
	/// <param name="move">移動量</param>
	/// <param name="direction">方向</param>
	static void Movement(XMFLOAT3& pos, XMFLOAT3& move, XMFLOAT3 direction);

	/// <summary>
	/// 直線運動
	/// </summary>
	/// <param name="pos">対象の座標</param>
	/// <param name="move">移動量</param>
	/// <param name="direction">方向</param>
	/// <param name="mass">質量</param>
	static void Movement(XMFLOAT3& pos, XMFLOAT3& move, XMFLOAT3 direction, float& mass);

	/// <summary>
	/// 摩擦による移動値の減衰
	/// </summary>
	/// <param name="move">移動値</param>
	/// <param name="graFlag">重力の有無</param>
	static void Friction(XMFLOAT3& move, bool &graFlag);

	/// <summary>
	/// 空気抵抗
	/// </summary>
	/// <param name="move">移動値</param>
	static void AirResistance(XMFLOAT3& move);

	/// <summary>
	/// 重力追加
	/// </summary>
	/// <param name="pos">重力対象の座標</param>
	/// <param name="graflag">重力の有無</param>
	static void Gravity(XMFLOAT3& pos, bool &graFlag);

	/// <summary>
	/// 重力と移動量
	/// </summary>
	/// <param name="move">移動量</param>
	/// <param name="graFlag">重力の有無</param>
	static void GravityCheckMove(XMFLOAT3& move, bool& graFlag);

	/// <summary>
	/// 衝突反発
	/// </summary>
	/// <param name="move1">移動量1</param>
	/// <param name="direction1">方向1</param>
	/// <param name="move2">移動量2</param>
	/// <param name="direction2">方向2</param>
	static void CollisionRebound(XMFLOAT3& move1, XMFLOAT3& direction1, XMFLOAT3& move2, XMFLOAT3& direction2);

	/// <summary>
	/// 衝突反発
	/// </summary>
	/// <param name="move1">移動1</param>
	/// <param name="direction1">方向1</param>
	/// <param name="mass1">質量1</param>
	/// <param name="move2">移動量2</param>
	/// <param name="direction2">方向2</param>
	/// <param name="mass2">質量2</param>
	static void CollisionRebound(XMFLOAT3& move1, XMFLOAT3& direction1, float& mass1, XMFLOAT3& move2, XMFLOAT3& direction2, float& mass2);

	/// <summary>
	/// 衝突反発
	/// </summary>
	/// <param name="move1">移動1</param>
	/// <param name="direction1">方向1</param>
	/// <param name="reboundFactor1">反発係数1</param>
	/// <param name="move2">移動量2</param>
	/// <param name="direction2">方向2</param>
	/// <param name="reboundFactor22">反発係数2</param>
	static void CollisionReboundOn(XMFLOAT3& move1, XMFLOAT3& direction1, float& reboundFactor1, XMFLOAT3& move2, XMFLOAT3& direction2, float& reboundFactor2);

	
	/// <summary>
	/// 正負の判別
	/// </summary>
	/// <param name="num">数値</param>
	/// <returns>1 or -1</returns>
	static float Sign(const float& num);

	/// <summary>
	/// 最大値検索
	/// </summary>
	/// <param name="array">値</param>
	/// <param name="size">サイズ</param>
	/// <returns>最大値</returns>
	static float maxElement(const float* array, int size);

	/// <summary>
	/// 最小値検索
	/// </summary>
	/// <param name="array">値</param>
	/// <param name="size">サイズ</param>
	/// <returns>最小値</returns>
	static float minElement(const float* array, int size);
	

private:
	static const float k;					//抵抗力
	static const float gravity;				//重力定数
	static const float graAdjustConstant;	//重力調整定数
	static const float friction;			//摩擦定数
	static float graValue;					//重力増加値
	static float airResistance;				//空気抵抗値
};

