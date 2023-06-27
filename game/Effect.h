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
private: // �G�C���A�X
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;

public:
	/// <summary>
	/// ������
	/// </summary>
	void Initialize();

	/// <summary>
	/// ���
	/// </summary>
	void Finalize();

	/// <summary>
	/// �A�b�v�f�[�g
	/// </summary>
	void Update(XMFLOAT3 cameraPos, int crystalNum);

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

private:
	void ClystalEffectUpdate(int crystalNum);

	XMFLOAT2 leap(XMFLOAT2 start, XMFLOAT2 end, float time);

private:
	Effect2d* crystalEffect = nullptr;
	std::vector<Effect2d*> crystalEffects;
	float crystalMoveTime = 0.0f;

	//�e�N���X�^����UI�ʒu
	const XMFLOAT2 crystalUIPos[8] = { { 1190.0f, 165.0f },{ 1250.0f, 180.0f },{ 1230.0f, 290.0f },{ 1200.0f, 300.0f },
		{ 1130.0f, 300.0f },{ 1140.0f, 390.0f },{ 1145.0f, 440.0f },{ 1210.0f, 460.0f } };
};

