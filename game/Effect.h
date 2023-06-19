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
	static void Initialize();

	/// <summary>
	/// ���
	/// </summary>
	static void Finalize();

	/// <summary>
	/// �A�b�v�f�[�g
	/// </summary>
	static void Update(XMFLOAT3 cameraPos);

	/// <summary>
	/// �`��
	/// </summary>
	static void Draw();

private:
	static void ClystalEffectUpdate();

	static XMFLOAT2 leap(XMFLOAT2 start, XMFLOAT2 end, float time);

private:
	static Effect2d* crystalEffect;
	static std::vector<Effect2d*> crystalEffects;
	static float crystalMoveTime;

	//�e�N���X�^����UI�ʒu
	static const XMFLOAT2 crystalUIPos[8];
};

