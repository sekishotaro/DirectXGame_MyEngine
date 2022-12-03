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

public:
	//オブジェクト
	static std::vector<std::unique_ptr<Object3d>> OpticalPosts;
	//モデル
	static Model* modelOpticalPost;

	static bool drawFlag;

	//デバック用数
	static float num;
};

