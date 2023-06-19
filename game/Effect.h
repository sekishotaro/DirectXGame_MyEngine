#pragma once
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include "Sprite.h"
#include "Effect2d.h"
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
	static void ClystalEffectUpdate();

	static XMFLOAT2 leap(XMFLOAT2 start, XMFLOAT2 end, float time);

private:
	static Effect2d* crystalEffect;
	static std::vector<Effect2d*> crystalEffects;
	static float crystalMoveTime;

	//各クリスタルのUI位置
	static const XMFLOAT2 crystalUIPos[8];
};

