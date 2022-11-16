#pragma once
#include <DirectXMath.h>
#include "Object3d.h"

class OpticalPost
{
private:
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:

	static void Initialize();

	static void Update();

	static void Draw();

	static void Finalize();

	static void Erase(int num);

	static void SetDrawFlag(bool flag) { drawFlag = flag; }

public:
	//�I�u�W�F�N�g
	static std::vector<std::unique_ptr<Object3d>> OpticalPosts;
	//���f��
	static Model* modelOpticalPost;

	static bool drawFlag;
};

