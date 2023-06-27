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
	//�I�u�W�F�N�g
	std::vector<std::unique_ptr<Object3d>> OpticalPosts;
	std::vector<std::unique_ptr<Object3d>> smallOpticalPosts;
	std::unique_ptr<Object3d> goalOpticalPostObject;
	//���f��
	Model* modelOpticalPost = nullptr;

	bool drawFlag = false;

	//�f�o�b�N�p��
	float num = 0.0f;
	//�G�t�F�N�g�p�̐�
	int effectNum = 7;

	Model* modelCrystalEffect = nullptr;
	std::unique_ptr<Object3d> CrystalEffectObject;
	//�ړ���
	std::vector<float> moveQuantitys;
	float moveQuantityMax = 3.0f;
	//�T�C�Y�l
	float sizeNum = 3.0f;
	//���������̒��̐�
	const int smallOpticalPostNum = 20;
};

