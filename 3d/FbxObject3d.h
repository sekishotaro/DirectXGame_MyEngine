#pragma once
#include "FbxModel.h"
#include "Camera.h"
#include "FbxLoader.h"

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include <string>

#include "CollisionInfo.h"

class BaseCollider;

class FbxObject3d
{
protected: // エイリアス
// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:	//定数
	//ボーンの最大数
	static const int MAX_BONES = 128;

public: // サブクラス
	// 定数バッファ用データ構造体(座標変換行列用)
	struct ConstBufferDataTransform
	{
		XMMATRIX viewproj;		//ビュープロジェクション行列
		XMMATRIX world;			//ワールド行列
		XMFLOAT3 cameraPos;		//カメラ座標 (ワールド座標)
	};

	//定数バッファ用データ構造体(スキニング)
	struct ConstBufferDataSkin
	{
		XMMATRIX bones[MAX_BONES];
	};

public: // 静的メンバ関数
	// setter
	static void SetDevice(ID3D12Device *device) { FbxObject3d::device = device; }
	static void SetCamera(Camera *camera) { FbxObject3d::camera = camera; }
	

private: // 静的メンバ変数
	//デバイス
	static ID3D12Device *device;
	// カメラ
	static Camera *camera;
	//ルートシグネチャ
	static ComPtr<ID3D12RootSignature> rootsignature;
	//パイプラインステートオブジェクト
	static ComPtr<ID3D12PipelineState> pipelinestate;

public: // メンバ関数

	FbxObject3d() = default;

	virtual ~FbxObject3d();

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize();
	/// <summary>
	/// グラフィックスパイプラインの生成
	/// </summary>
	static void CreateGraphicsPipeline();
	/// <summary>
	/// マイフレーム処理
	/// </summary>
	virtual void Update();
	/// <summary>
	/// モデルのセット
	/// </summary>
	/// <param name="model">モデル</param>
	virtual void SetModel(FbxModel *fbxModel) { this->fbxModel = fbxModel; };
	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="cmdList">コマンドリスト</param>
	virtual void Draw(ID3D12GraphicsCommandList *cmdList);

	/// <summary>
	/// アニメーション読み込み
	/// </summary>
	virtual void LoadAnimation();

	/// <summary>
	/// アニメーション開始
	/// </summary>
	virtual void PlayAnimation();

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

	void UpdateWorldMatrix();

	/// <summary>
	/// ワールド行列の取得
	/// </summary>
	/// <returns>ワールド行列</returns>
	const XMMATRIX& GetMatWorld() { return matWorld; }

protected:
	// 定数バッファ
	ComPtr<ID3D12Resource> constBuffTransform;
	//ローカルスケール
	XMFLOAT3 scale = { 1,1,1 };
	//X,Y,Z軸回りのローカル回転角
	XMFLOAT3 rotation = { 0,0,0 };
	//ローカル座標
	XMFLOAT3 position = { 0,0,0 };
	//ローカルワールド変換行列
	XMMATRIX matWorld = {};
	//モデル
	FbxModel *fbxModel = nullptr;
	//定数バッファ(スキン)
	ComPtr<ID3D12Resource> constBuffSkin;
	//1フレームの時間
	FbxTime frameTime;
	//アニメーション開始時間
	FbxTime startTime;
	//アニメーション終了時間
	FbxTime endTime;
	//現在時間(アニメーション)
	FbxTime currentTime;
	//アニメ―ション再生中
	bool isPlay = false;
	//アニメーションの数
	int fbxAnimationNum;
	//アニメーションをロードしたかの確認用フラグ
	bool LoadedFlag = false;
	//カメラいち
	XMFLOAT2 cameraPos2d = {};

	//クラス名 (デバック用)
	const char* name = nullptr;
	BaseCollider* collider = nullptr;

public:
	//アニメーションをするかのフラグ
	bool AnimationFlag = false;
	//アニメーションの数
	int AnimationNum = 0;
public:

	//getter
	bool GetisPlay() { return isPlay; }

	/// <summary>
	/// 座標の取得
	/// </summary>
	/// <returns>座標</returns>
	const XMFLOAT3& GetPosition() { return position; }

	/// <summary>
	/// 座標の設定
	/// </summary>
	/// <param name="position">座標</param>
	void SetPosition(const XMFLOAT3& position) { this->position = position; }

	/// <summary>
	/// 回転角の取得
	/// </summary>
	/// <returns>回転角</returns>
	const XMFLOAT3& GetRotation() { return rotation; }

	/// <summary>
	/// 回転角の設定
	/// </summary>
	/// <param name="rotation">回転角</param>
	void SetRotation(const XMFLOAT3& rotation) { this->rotation = rotation; }

	/// <summary>
	/// スケールの設定
	/// </summary>
	/// <param name="scale">スケール</param>
	void SetScale(const XMFLOAT3& scale) { this->scale = scale; }
};

