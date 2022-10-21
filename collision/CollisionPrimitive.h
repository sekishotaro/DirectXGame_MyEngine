#pragma once
/// <summary>
/// �����蔻��v���~�e�B�u
/// </summary>
#include <DirectXMath.h>

/// <summary>
/// ��
/// </summary>
struct Sphere
{
	//���S���W
	DirectX::XMVECTOR center = { 0,0,0,1 };
	//���a
	float radius = 1.0f;
};

/// <summary>
/// ��
/// </summary>
struct SphereF
{
	//���S���W
	DirectX::XMFLOAT3 center = { 0,0,0 };
	//���a
	float radius = 1.0f;
};

/// <summary>
/// �~
/// </summary>
struct Circle
{
	//���S���W
	DirectX::XMFLOAT2 center = { 0,0 };
	//���a
	float radius = 1.0f;
};

/// <summary>
/// �~��
/// </summary>
struct Cylinder
{
	//���S���W
	DirectX::XMFLOAT3 center = { 0,0,0 };
	//���a
	float radius = 1.0f;
	//����
	float top = 1.0f;
	float bottom = -1.0f;
};



/// <summary>
/// ����
/// </summary>
struct Plane
{
	//�@���x�N�g��
	DirectX::XMVECTOR normal = { 0,1,0,0 };
	//���_(0,0,0)����̋���
	float distance = 0.0f;
};

/// <summary>
/// �|��
/// </summary>
struct Plate
{
	//���W
	DirectX::XMVECTOR position = {};
	//�T�C�Y
	DirectX::XMFLOAT3 size = {};
	//�@���x�N�g��
	DirectX::XMVECTOR normal = { 0,1,0,0 };

	//�|���̎l�p�`���\������O�p�|���S��2�̒��_6��(Todo:4�ł��ł���悤��)
	DirectX::XMVECTOR vert1 = { 0,0,0,0 };
	DirectX::XMVECTOR vert2 = { 0,0,0,0 };
	DirectX::XMVECTOR vert3 = { 0,0,0,0 };
	DirectX::XMVECTOR vert4 = { 0,0,0,0 };
	DirectX::XMVECTOR vert5 = { 0,0,0,0 };
	DirectX::XMVECTOR vert6 = { 0,0,0,0 };
};


/// <summary>
/// �@���t���O�p�` (���v��肪�\��)
/// </summary>
class Triangle
{
public:
	//���_���W3��
	DirectX::XMVECTOR p0;
	DirectX::XMVECTOR p1;
	DirectX::XMVECTOR p2;
	//�@���x�N�g��
	DirectX::XMVECTOR normal;

	/// <summary>
	/// �@���̌v�Z
	/// </summary>
	void ComputeNormal();
};

/// <summary>
/// ���C (������)
/// </summary>
struct Ray
{
	//�n�_���W
	DirectX::XMVECTOR start = { 0,0,0,1 };
	//����
	DirectX::XMVECTOR dir = { 1,0,0,0 };
};

/// <summary>
/// ����
/// </summary>
struct LineSegment
{
	//�n�_
	DirectX::XMFLOAT3 start = { 0.0f, 0.0f, 0.0f };
	//�I�_
	DirectX::XMFLOAT3 end = { 0.0f, 0.0f, 0.0f };
};




/// <summary>
/// �����`
/// </summary>
struct Box
{
	DirectX::XMFLOAT3 centerPos = { 0,0,0};
	DirectX::XMFLOAT3 size = { 1.0f, 1.0f, 1.0f };
	DirectX::XMFLOAT3 MaxPos = {centerPos.x + size.x, centerPos.y + size.y ,centerPos.z + size.z };
	DirectX::XMFLOAT3 LeastPos = { centerPos.x - size.x, centerPos.y - size.y ,centerPos.z - size.z };
};