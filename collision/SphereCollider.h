#pragma once
#include "BaseCollider.h"
#include "CollisionPrimitive.h"

#include <DirectXMath.h>

/// <summary>
/// ���Փ˔���I�u�W�F�N�g
/// </summary>
class SphereCollider : public BaseCollider, public Sphere
{
private: //�G�C���A�X
	//DirectX::���ȗ�
	using XMVECTOR = DirectX::XMVECTOR;

public:
	SphereCollider(XMVECTOR offset = { 0,0,0,0 }, float radius = 1.0f) :
		offset(offset),
		radius(radius)
	{
		//���`����Z�b�g
		shapeType = COLLISIONSHAPE_SPHERE;
	}

	/// <summary>
	/// �X�V
	/// </summary>
	void Update() override;

	void UpdateF() override;

	inline void SetRadius(float radius) { this->radius = radius; }

	inline const XMVECTOR& GetOffset() { return offset; }

	inline void SetOffset(const XMVECTOR& offset) { this->offset = offset; }

	inline float GetRadius() { return radius; }

private:
	//�I�u�W�F�N�g���S����̃I�t�Z�b�g
	XMVECTOR offset;
	//���a
	float radius;
};

