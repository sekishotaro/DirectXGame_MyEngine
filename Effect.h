#pragma once
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include "Sprite.h"
#include "Camera.h"

class Effect
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
	/// 初期化
	/// </summary>
	static void Initialize();

	/// <summary>
	/// 解放
	/// </summary>
	static void Finalize();

	/// <summary>
	/// アップデート
	/// </summary>
	static void Update(XMFLOAT3 cameraPos);

	/// <summary>
	/// 描画
	/// </summary>
	static void Draw();

private:
	static void EnemyAccessEffectUpdate(XMFLOAT3 cameraPos);

private:
	static Sprite* EnemyAccessEffect;
	static float EnemyAccessEffectSize;
	static float EnemyAccessDisSpeed;
	static bool oldRaidFlag;
	static float EnemyPlayerDis;		//襲撃起動時の距離の保存
};

