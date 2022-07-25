#pragma once
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>

class MyMath
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

	MyMath();

	~MyMath();

	/// <summary>
	/// ���������ړ�
	/// </summary>
	/// <param name="pos">�Ώۂ̍��W</param>
	/// <param name="move">�ړ��l</param>
	static void Movement(XMFLOAT3& pos, XMFLOAT3& move);

	/// <summary>
	/// ���C�ɂ��ړ��l�̌���
	/// </summary>
	/// <param name="move">�ړ��l</param>
	/// <param name="graFlag">�d�̗͂L��</param>
	static void Friction(XMFLOAT3& move, bool &graFlag);

	static void AirResistance(XMFLOAT3& move);


	/// <summary>
	/// �d�͒ǉ�
	/// </summary>
	/// <param name="pos">�d�͑Ώۂ̍��W</param>
	/// <param name="graflag">�d�̗͂L��</param>
	static void Gravity(XMFLOAT3& pos, bool &graFlag);

	/// <summary>
	/// �n�ʂɂ������̃`�F�b�N
	/// </summary>
	/// <param name="pos">�Ώۂ̍��W</param>
	/// <param name="groundY">�n�ʂ̍���</param>
	/// <param name="graFlag">�d�̗͂L��</param>
	static void GravityCheck(XMFLOAT3& pos, float& groundY, bool& graFlag);

	/// <summary>
	/// �d�͂ƈړ���
	/// </summary>
	/// <param name="move">�ړ���</param>
	/// <param name="graFlag">�d�̗͂L��</param>
	static void GravityCheckMove(XMFLOAT3& move, bool& graFlag);

private:
	static const float k;					//��R��
	static const float gravity;				//�d�͒萔
	static const float graAdjustConstant;	//�d�͒����萔
	static const float friction;			//���C�萔
	static float graValue;					//�d�͑����l
	static float airResistance;				//��C��R�l
};

