﻿#pragma once

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include "Model.h"
#include "Camera.h"
#include "CollisionInfo.h"
#include "LightGroup.h"

class BaseCollider;

/// <summary>
/// 3Dオブジェクト
/// </summary>
class Object3d
{
private: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public: // サブクラス

	// 定数バッファ用データ構造体B0
	struct ConstBufferDataB0
	{
		XMFLOAT4 color;	// 色 (RGBA)
		XMMATRIX viewproj;
		XMMATRIX world;
		XMFLOAT3 cameraPos;
		bool tilingFlag;
	};

private: // 定数
	static const int division = 50;					// 分割数
	static const float radius;				// 底面の半径
	static const float prizmHeight;			// 柱の高さ
	static const int planeCount = division * 2 + division * 2;		// 面の数
	static const int vertexCount = planeCount * 3;		// 頂点数

public: // 静的メンバ関数
	
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Object3d() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~Object3d();


	/// <summary>
	/// 静的初期化
	/// </summary>
	/// <param name="device">デバイス</param>
	/// <param name="window_width">画面幅</param>
	/// <param name="window_height">画面高さ</param>
	/// <returns>成否</returns>
	static bool StaticInitialize(ID3D12Device* device, Camera *camera = nullptr);

	/// <summary>
	/// 描画前処理
	/// </summary>
	/// <param name="cmdList">描画コマンドリスト</param>
	static void PreDraw(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// 描画後処理
	/// </summary>
	static void PostDraw();

	/// <summary>
	/// 3Dオブジェクト生成
	/// </summary>
	/// <returns></returns>
	static std::unique_ptr<Object3d> Create();

	/// <summary>
	/// カメラのセット
	/// </summary>
	/// <param name="camera">カメラ</param>
	static void SetCamera(Camera *camera)
	{
		Object3d::camera = camera;
	}

private: // 静的メンバ変数
	// デバイス
	static ID3D12Device* device;
	// デスクリプタサイズ
	//static UINT descriptorHandleIncrementSize;
	// コマンドリスト
	static ID3D12GraphicsCommandList* cmdList;
	// ルートシグネチャ
	static ComPtr<ID3D12RootSignature> rootsignature;
	// パイプラインステートオブジェクト
	static ComPtr<ID3D12PipelineState> pipelinestate;

	// カメラ
	static Camera *camera;
	
	//襲撃用フラグ
	static bool raidFlag;

private:// 静的メンバ関数

	/// <summary>
	/// カメラ初期化
	/// </summary>
	/// <param name="window_width">画面横幅</param>
	/// <param name="window_height">画面縦幅</param>
	static void InitializeCamera(int window_width, int window_height);

	/// <summary>
	/// グラフィックパイプライン生成
	/// </summary>
	/// <returns>成否</returns>
	static bool InitializeGraphicsPipeline();


	/// <summary>
	/// ビュー行列を更新
	/// </summary>
	static void UpdateViewMatrix();

public: // メンバ関数
	
	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns>成否</returns>
	virtual bool Initialize();
	
	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	virtual void Update();

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw();

	/// <summary>
	/// ワールド行列の取得
	/// </summary>
	/// <returns>ワールド行列</returns>
	const XMMATRIX& GetMatWorld() { return matWorld; }

	/// <summary>
	/// コライダーのセット
	/// </summary>
	/// <param name="collider">コライダー</param>
	void SetCollider(BaseCollider* collider);

	/// <summary>
	/// 衝突時コールバック関数
	/// </summary>
	/// <param name="info">衝突情報</param>
	virtual void OnCollision(const CollisionInfo& info) {}

	/// <summary>
	/// 座標の取得
	/// </summary>
	/// <returns>座標</returns>
	const XMFLOAT3& GetPosition() { return position; }

	/// <summary>
	/// 座標の設定
	/// </summary>
	/// <param name="position">座標</param>
	void SetPosition(const XMFLOAT3 &position) { this->position = position; }

	/// <summary>
	/// 回転角の取得
	/// </summary>
	/// <returns>回転角</returns>
	const XMFLOAT3 &GetRotation() { return rotation; }

	/// <summary>
	/// 回転角の設定
	/// </summary>
	/// <param name="rotation">回転角</param>
	void SetRotation(const XMFLOAT3 &rotation) { this->rotation = rotation; }

	/// <summary>
	/// スケールの設定
	/// </summary>
	/// <param name="scale">スケール</param>
	void SetScale(const XMFLOAT3& scale) { this->scale = scale; }


	const XMFLOAT3 &GetScale() { return scale; }

	//setter
	void SetModel(Model *model) { this->model = model; }

	void SetBillboard(bool isBillboard) { this->isBillboard = isBillboard; }

	inline Model* GetModel() { return model; }

	void UpdateWorldMatrix();

	void SetPSShaderName(std::wstring PSShaderName) { this->PSshaderName = PSshaderName; }

	void SetVSShaderName(std::wstring VSShaderName) { this->VSshaderName = VSshaderName; }

	static void SetLight(LightGroup* light) { Object3d::light = light; }

	static void SetRaidFlag(bool raidFlag) { Object3d::raidFlag = raidFlag; }


	void SetBaseCollider(BaseCollider* collider) { Object3d::collider = collider; }
	BaseCollider* GetBaseCollider() { return collider; }

	void OnTitleFlag() { tilingFlag = true; }

protected: // メンバ変数
	ComPtr<ID3D12Resource> constBuffB0; // 定数バッファ
	// 色
	XMFLOAT4 color = { 1,1,1,1 };
	// ローカルスケール
	XMFLOAT3 scale = { 1,1,1 };
	// X,Y,Z軸回りのローカル回転角
	XMFLOAT3 rotation = { 0,0,0 };
	// ローカル座標
	XMFLOAT3 position = { 0,0,0 };
	// ローカルワールド変換行列
	XMMATRIX matWorld = {};
	// 親オブジェクト
	Object3d* parent = nullptr;
	//モデル
	Model *model = nullptr;
	// ビルボード
	bool isBillboard = false;
	//クラス名 (デバック用)
	const char* name = nullptr;
	//コライダー
	BaseCollider* collider = nullptr;
	//ライト
	static LightGroup* light;
	//カメラいち
	XMFLOAT2 cameraPos2d = {};
	//タイリングフラグ
	bool tilingFlag = false;

	//シェーダー
	static std::wstring VSshaderName;
	static std::wstring PSshaderName;
};

