#pragma once

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>

#include "Input.h"
#include "FbxObject3d.h"
#include "Object3d.h"

class Player : public Object3d
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

	/// <summary>
	/// 3Dオブジェクト生成
	/// </summary>
	/// <returns>インスタンス生成</returns>
	static Player* Create(Model* model = nullptr);

public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns>成否</returns>
	bool Initialize() override;

	/// <summary>
	/// マイフレーム処理
	/// </summary>
	void Update() override;

	/// <summary>
	/// 衝突時コールバック関数
	/// </summary>
	/// <param name="info">衝突情報</param>
	void OnCollision(const CollisionInfo& info) override;

	void PushBack(const DirectX::XMVECTOR& normal, const XMFLOAT3& distance);

	//デバック用ゲッター
	static XMFLOAT3 GetPos() { return pos; }

	static XMFLOAT3 GetMove() { return moveV; };

	static int GetNowMove() {
		if (nowMove == true) return 1;
		if (nowMove == false) return 0;
	}
	
	static int GetadhesionMesh() {
		if (adhesionMesh == true) return 1;
		if (adhesionMesh == false) return 0;
	}

	static int GetCrystal() { return crystalNum; }

private:
	//接地フラグ
	static bool onGround;
	//接着フラグ
	static bool adhesionMesh;

	//障害物接着フラグ
	bool adhesion = false;
	//オブジェク接地フラグ
	bool onObject = false;
	//移動中確認フラグ
	static bool nowMove;
	//落下ベクトル
	DirectX::XMVECTOR fallV;
	//プレイヤーの位置

	static XMFLOAT3 pos;
	static XMFLOAT3 moveV;

	//クリスタルの数
	static int crystalNum;
};

