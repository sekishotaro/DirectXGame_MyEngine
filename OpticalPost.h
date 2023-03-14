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

	static void Initialize();

	static void Update(const XMFLOAT3 &cameraPos);

	static void Draw();

	static void Finalize();

	static void Erase(int num);

	static void SetDrawFlag(bool flag) { drawFlag = flag; }

	static float& GetNum() { return num; }

	static float RotaUpdate(const XMFLOAT3& cameraPos, const XMFLOAT3 &crystalPos);

	static int NearCrystalNum();

	static void CrystalEfectSizeNumUpdate();

	static void SmallOpticalPostsMoveUpdate();
public:
	//オブジェクト
	static std::vector<std::unique_ptr<Object3d>> OpticalPosts;
	static std::vector<std::unique_ptr<Object3d>> smallOpticalPosts;
	//モデル
	static Model* modelOpticalPost;

	static bool drawFlag;

	//デバック用数
	static float num;
	//エフェクト用の数
	static int effectNum;

	static Model* modelCrystalEffect;
	static std::unique_ptr<Object3d> CrystalEffectObject;
	//移動量
	static std::vector<float> moveQuantitys;
	static float moveQuantityMax;
	//サイズ値
	static float sizeNum;
};

