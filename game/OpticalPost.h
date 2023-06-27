#pragma once
#include <DirectXMath.h>
#include "Object3d.h"

class OpticalPost
{
private:
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:

	void Initialize();

	void Update(const XMFLOAT3 &cameraPos);

	void Draw();

	void Finalize();

	void Erase(bool getFlag, int num);

	void SetDrawFlag(bool flag) { drawFlag = flag; }

	float& GetNum() { return num; }

	float RotaUpdate(const XMFLOAT3& cameraPos, const XMFLOAT3 &crystalPos);

	int NearCrystalNum();

	void CrystalEfectSizeNumUpdate();

	void SmallOpticalPostsMoveUpdate();

	void Restart();
public:
	//オブジェクト
	std::vector<std::unique_ptr<Object3d>> OpticalPosts;
	std::vector<std::unique_ptr<Object3d>> smallOpticalPosts;
	std::unique_ptr<Object3d> goalOpticalPostObject;
	//モデル
	Model* modelOpticalPost = nullptr;

	bool drawFlag = false;

	//デバック用数
	float num = 0.0f;
	//エフェクト用の数
	int effectNum = 7;

	Model* modelCrystalEffect = nullptr;
	std::unique_ptr<Object3d> CrystalEffectObject;
	//移動量
	std::vector<float> moveQuantitys;
	float moveQuantityMax = 3.0f;
	//サイズ値
	float sizeNum = 3.0f;
	//小さい光の柱の数
	const int smallOpticalPostNum = 20;
};

