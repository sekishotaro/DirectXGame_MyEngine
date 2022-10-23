#pragma once

#include <DirectXMath.h>

class Light
{
private: // エイリアス
// Microsoft::WRL::を省略
	//template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	Light(const int& window_width, const int& window_height);

	virtual void Update();

	/// <summary>
	/// ビュー行列を更新
	/// </summary>
	void UpdateViewMatrix();

	/// <summary>
	/// 射影行列を更新
	/// </summary>
	void UpdateProjectionMatrix();

	/// <summary>
	/// ビュー行列の取得
	/// </summary>
	/// <returns>ビュー行列</returns>
	inline const XMMATRIX& GetViewMatrix() {
		return matView;
	}

	/// <summary>
	/// 射影行列の取得
	/// </summary>
	/// <returns>射影行列</returns>
	inline const XMMATRIX& GetProjectionMatrix() {
		return matProjection;
	}

	/// <summary>
	/// ビュー射影行列の取得
	/// </summary>
	/// <returns>ビュー射影行列</returns>
	inline const XMMATRIX& GetViewProjectionMatrix() {
		return matViewProjection;
	}

	/// <summary>
	/// ビルボード行列の取得
	/// </summary>
	/// <returns>ビルボード行列</returns>
	inline const XMMATRIX& GetBillboardMatrix() {
		return matBillboard;
	}

	const XMFLOAT3& GetPosition() { return pos; viewDirty = true;}
	void SetPosition(const XMFLOAT3& pos) { this->pos = pos; }
	const XMFLOAT3& GetTarget() { return target; }
	void SetTarget(const XMFLOAT3& target) { this->target = target; viewDirty = true;}


protected:

	// ビュー行列
	XMMATRIX matView = DirectX::XMMatrixIdentity();
	// ビルボード行列
	XMMATRIX matBillboard = DirectX::XMMatrixIdentity();
	// Y軸回りビルボード行列
	XMMATRIX matBillboardY = DirectX::XMMatrixIdentity();
	// 射影行列
	XMMATRIX matProjection = DirectX::XMMatrixIdentity();
	// ビュー射影行列
	XMMATRIX matViewProjection = DirectX::XMMatrixIdentity();
	// ビュー行列ダーティフラグ
	bool viewDirty = false;
	// 射影行列ダーティフラグ
	bool projectionDirty = false;
	//ライトの位置
	XMFLOAT3 pos = { 0.0f, 10.0f, -10.0f };
	//ライトの向き  
	XMFLOAT3 target = { 0.0f, 0.0f, 0.0f };
	//上方向ベクトル
	const XMFLOAT3 up = { 0,1,0 };
	// アスペクト比
	float aspectRatio = 1.0f;

	static float window_height;
	static float window_width;
};

