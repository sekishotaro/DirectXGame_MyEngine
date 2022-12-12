#pragma once
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include "Sprite.h"
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
	static void EnemyAccessEffectUpdate(XMFLOAT3 cameraPos);

private:
	static Sprite* EnemyAccessEffect;
	static float EnemyAccessEffectSize;
	static float EnemyAccessDisSpeed;
	static bool oldRaidFlag;
	static float EnemyPlayerDis;		//�P���N�����̋����̕ۑ�
};

