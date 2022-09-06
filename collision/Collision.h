#pragma once

#include "CollisionPrimitive.h"
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>

/// <summary>
/// �����蔻��w���p�[�N���X
/// </summary>
class Collision
{
private: // �G�C���A�X
// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;


public:
	/// <summary>
	/// ���ƕ��ʂ̓����蔻��
	/// </summary>
	/// <param name="sphere">��</param>
	/// <param name="plane">����</param>
	/// <param name="inter">��_</param>
	/// <returns>�������Ă��邩�ۂ�</returns>
	static bool CheckSphere2Plane(const Sphere &sphere, const Plane &plane, DirectX::XMVECTOR *inter = nullptr);

	/// <summary>
	/// ���Ƌ��̓����蔻��
	/// </summary>
	/// <param name="sphere1">��</param>
	/// <param name="sphere2">��</param>
	/// <returns></returns>
	static bool CheckSphereSphere(const SphereF& sphere1, const SphereF& sphere2);

	/// <summary>
	/// �_�ƎO�p�`�̍ŋߐړ_�����߂�
	/// </summary>
	/// <param name="point">�_</param>
	/// <param name="triangle">�O�p�`</param>
	/// <param name="closest">�ŋߐړ_(�o�͗p)</param>
	static void ClosestPtPoint2Triangle(const DirectX::XMVECTOR &point, const Triangle &triangle, DirectX::XMVECTOR *closest);

	/// <summary>
	/// ���Ɩ@���t���O�p�`�̓�����`�F�b�N
	/// </summary>
	/// <param name="sphere">��</param>
	/// <param name="triangle">�O�p�`</param>
	/// <param name="inter">��_ (�O�p�`��̍ŋߐړ_)</param>
	/// <returns>�������Ă��邩�ۂ�</returns>
	static bool CheckSphere2Triangle(const Sphere &sphere, const Triangle &triangle, DirectX::XMVECTOR *inter = nullptr);

	/// <summary>
	/// ���C�ƕ��ʂ̓����蔻��
	/// </summary>
	/// <param name="ray">���C</param>
	/// <param name="plane">����</param>
	/// <param name="distance">���� (�o�͗p)</param>
	/// <param name="inter">��_ (�o�͗p)</param>
	/// <returns>�������Ă��邩�ۂ�</returns>
	static bool CheckRay2Plane(const Ray &ray, const Plane &plane, float *distance = nullptr, DirectX::XMVECTOR *inter = nullptr);

	/// <summary>
	/// ���C�Ɩ@���t���O�p�`�̓����蔻��
	/// </summary>
	/// <param name="ray">���C</param>
	/// <param name="triangle">�O�p�`</param>
	/// <param name="distance">���� (�o�͗p)</param>
	/// <param name="inter">���� (�o�͗p)</param>
	/// <returns>�������Ă��邩�ۂ�</returns>
	static bool CheckRay2Triangle(const Ray &ray, const Triangle &triangle, float *distance = nullptr, DirectX::XMVECTOR *inter = nullptr);

	/// <summary>
	/// ���C�Ƌ��̓����蔻��
	/// </summary>
	/// <param name="ray">���C</param>
	/// <param name="sphere">��</param>
	/// <param name="distance">���� (�o�͗p)</param>
	/// <param name="inter">��_ (�o�͗p)</param>
	/// <returns>�������Ă��邩�ۂ�</returns>
	static bool CheckRay2Sphere(const Ray &ray, const Sphere &sphere, float *distance = nullptr, DirectX::XMVECTOR *inter = nullptr);

	/// <summary>
	/// �����ƒ����`�̓����蔻��
	/// </summary>
	/// <param name="line">����</param>
	/// <param name="box"></param>
	/// <returns></returns>
	static bool CheckLineSegmentBox(const LineSegment& line, const Box& box);

	/// <summary>
	/// 
	/// </summary>
	/// <param name="ray"></param>
	/// <param name="box"></param>
	/// <returns></returns>
	static bool CheckRayBox(const Ray& ray, const Box& box);

	static bool Check2LineSegment(const LineSegment& line1, const LineSegment& line2);


	static bool CheckSphereBox(const SphereF& sphere, const Box& box);
	static bool CheckAABB(const Box& box1, const Box& box2);
	static bool CheckCircleDot(const Circle& circle, const XMFLOAT2& dot);
	static bool CheckCylinderDotXY(const Cylinder& cylinder, const XMFLOAT3& dot);
	static bool CheckCylinderDotXZ(const Cylinder& cylinder, const XMFLOAT3& dot);
	static bool CheckCylinderDotZY(const Cylinder& cylinder, const XMFLOAT3& dot);
	static bool CheckSphereDot(const SphereF& sphere, XMFLOAT3& dot);
};