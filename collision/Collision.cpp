#include "Collision.h"

using namespace DirectX;

bool Collision::CheckSphere2Plane(const Sphere &sphere, const Plane &plane, DirectX::XMVECTOR *inter)
{
	//���W�n�̌��_���狅�̒��S���W�ւ̋���
	XMVECTOR distV = XMVector3Dot(sphere.center, plane.normal);
	//���ʂ̌��_���������Z���邱�ƂŁA���ʂƋ��̒��S�Ƃ̋������o��
	float dist = distV.m128_f32[0] - plane.distance;
	//�����̐�Βl�����a���傫����Γ������Ă��Ȃ�
	if (fabsf(dist) > sphere.radius) return false;

	//�^����_���v�Z
	if (inter)
	{
		//���ʏ�̍ŋߐړ_���A�^����_�Ƃ���
		*inter = -dist * plane.normal + sphere.center;
	}

	return true;
}

bool Collision::CheckSphereSphere(const SphereF& sphere1, const SphereF& sphere2)
{
	float X = (sphere1.center.x - sphere2.center.x) * (sphere1.center.x - sphere2.center.x);
	float Y = (sphere1.center.y - sphere2.center.y) * (sphere1.center.y - sphere2.center.y);
	float Z = (sphere1.center.z - sphere2.center.z) * (sphere1.center.z - sphere2.center.z);
	float R = (sphere1.radius + sphere2.radius) * (sphere1.radius + sphere2.radius);
	if (X + Y + Z <= R) {
		return true;
	}
	return false;
}

void Collision::ClosestPtPoint2Triangle(const DirectX::XMVECTOR &point, const Triangle &triangle, DirectX::XMVECTOR *closest)
{
	// point��p0�̊O���̒��_�̈�̒��ɂ��邩�ǂ����`�F�b�N
	XMVECTOR p0_p1 = triangle.p1 - triangle.p0;
	XMVECTOR p0_p2 = triangle.p2 - triangle.p0;
	XMVECTOR p0_pt = point - triangle.p0;

	XMVECTOR d1 = XMVector3Dot(p0_p1, p0_pt);
	XMVECTOR d2 = XMVector3Dot(p0_p2, p0_pt);

	if (d1.m128_f32[0] <= 0.0f && d2.m128_f32[0] <= 0.0f)
	{
		// p0���ŋߖT
		*closest = triangle.p0;
		return;
	}

	// point��p1�̊O���̒��_�̈�̒��ɂ��邩�ǂ����`�F�b�N
	XMVECTOR p1_pt = point - triangle.p1;

	XMVECTOR d3 = XMVector3Dot(p0_p1, p1_pt);
	XMVECTOR d4 = XMVector3Dot(p0_p2, p1_pt);

	if (d3.m128_f32[0] >= 0.0f && d4.m128_f32[0] <= d3.m128_f32[0])
	{
		// p1���ŋߖT
		*closest = triangle.p1;
		return;
	}

	// point��p0_p1�̕ӗ̈�̒��ɂ��邩�ǂ����`�F�b�N���A�����point��p0_p1��ɑ΂���ˉe��Ԃ�
	float vc = d1.m128_f32[0] * d4.m128_f32[0] - d3.m128_f32[0] * d2.m128_f32[0];
	if (vc <= 0.0f && d1.m128_f32[0] >= 0.0f && d3.m128_f32[0] <= 0.0f)
	{
		float v = d1.m128_f32[0] / (d1.m128_f32[0] - d3.m128_f32[0]);
		*closest = triangle.p0 + v * p0_p1;
		return;
	}

	// point��p2�̊O���̒��_�̈�̒��ɂ��邩�ǂ����`�F�b�N
	XMVECTOR p2_pt = point - triangle.p2;

	XMVECTOR d5 = XMVector3Dot(p0_p1, p2_pt);
	XMVECTOR d6 = XMVector3Dot(p0_p2, p2_pt);
	if (d6.m128_f32[0] >= 0.0f && d5.m128_f32[0] <= d6.m128_f32[0])
	{
		*closest = triangle.p2;
		return;
	}

	// point��p0_p2�̕ӗ̈�̒��ɂ��邩�ǂ����`�F�b�N���A�����point��p0_p2��ɑ΂���ˉe��Ԃ�
	float vb = d5.m128_f32[0] * d2.m128_f32[0] - d1.m128_f32[0] * d6.m128_f32[0];
	if (vb <= 0.0f && d2.m128_f32[0] >= 0.0f && d6.m128_f32[0] <= 0.0f)
	{
		float w = d2.m128_f32[0] / (d2.m128_f32[0] - d6.m128_f32[0]);
		*closest = triangle.p0 + w * p0_p2;
		return;
	}

	// point��p1_p2�̕ӗ̈�̒��ɂ��邩�ǂ����`�F�b�N���A�����point��p1_p2��ɑ΂���ˉe��Ԃ�
	float va = d3.m128_f32[0] * d6.m128_f32[0] - d5.m128_f32[0] * d4.m128_f32[0];
	if (va <= 0.0f && (d4.m128_f32[0] - d3.m128_f32[0]) >= 0.0f && (d5.m128_f32[0] - d6.m128_f32[0]) >= 0.0f)
	{
		float w = (d4.m128_f32[0] - d3.m128_f32[0]) / ((d4.m128_f32[0] - d3.m128_f32[0]) + (d5.m128_f32[0] - d6.m128_f32[0]));
		*closest = triangle.p1 + w * (triangle.p2 - triangle.p1);
		return;
	}

	float denom = 1.0f / (va + vb + vc);
	float v = vb * denom;
	float w = vc * denom;
	*closest = triangle.p0 + p0_p1 * v + p0_p2 * w;
}

bool Collision::CheckSphere2Triangle(const Sphere &sphere, const Triangle &triangle, DirectX::XMVECTOR *inter)
{
	XMVECTOR p;
	//���̒��S�ɑ΂���ŋߐړ_�ł���O�p�`��ɂ���_p�����邯��
	ClosestPtPoint2Triangle(sphere.center, triangle, &p);
	//�_p�Ƌ��̒��S�̍����x�N�g��
	XMVECTOR v = p - sphere.center;
	//�����̓������߂�
	// (�����x�N�g�����m�̓��ς͎O�����̒藝�̃��[�g�����̎��ƈ�v����)
	v = XMVector3Dot(v, v);
	// ���ƎO�p�`�̋��������a�ȉ��Ȃ瓖�����Ă��Ȃ�
	if (v.m128_f32[0] > sphere.radius * sphere.radius) return false;
	//�^����_���v�Z
	if (inter)
	{
		//�O�p�`��̍ŋߐړ_p���^����_�Ƃ���
		*inter = p;
	}
	return true;
}

bool Collision::CheckRay2Plane(const Ray &ray, const Plane &plane, float *distance, DirectX::XMVECTOR *inter)
{
	const float epsilon = 1.0e-5f;  //�덷�z���p�̔����Ȓl
	//�ʖ@���ƃ��C�̕����x�N�g���̓���
	float d1 = XMVector3Dot(plane.normal, ray.dir).m128_f32[0];
	//���ʂɂ͓�����Ȃ�
	if (d1 > -epsilon) { return false; }
	//���_�ƌ��_�̋��� (���ʂ̖@������)
	//�ʖ@���ƃ��C�̎n�_���W (�ʒu�x�N�g��)�̓���
	float d2 = XMVector3Dot(plane.normal, ray.start).m128_f32[0];
	//�n�_�ƕ��ʂ̋��� (���ʂ̖@������)
	float dist = d2 - plane.distance;
	//�n�_�ƕ��ʂ̋��� (���C����)
	float t = dist / -d1;
	//��_���n�_�����ɂ���̂ŁA������Ȃ�
	if (t < 0) return false;
	//��������������
	if (distance) { *distance = t; }
	//��_���v�Z
	if (inter) { *inter = ray.start + t * ray.dir;}
	return true;
}

bool Collision::CheckRay2Triangle(const Ray &ray, const Triangle &triangle, float *distance, DirectX::XMVECTOR *inter)
{
	//�O�p�`������Ă��镽�ʂ��Z�o
	Plane plane;
	XMVECTOR interPlane;
	plane.normal = triangle.normal;
	plane.distance = XMVector3Dot(triangle.normal, triangle.p0).m128_f32[0];
	//���C�ƕ��ʂ��������Ă��Ȃ���΁A�������Ă��Ȃ�
	if (!CheckRay2Plane(ray, plane, distance, &interPlane)) { return false; }
	//���C�ƕ��ʂ��������Ă����̂ŁB�����ƌ�_���������܂ꂽ
	//���C�ƕ��ʂ̌�_���O�p�`�̓����ɂ��邩����
	const float epsilon = 1.0e-5f;  //�덷�z���p�̔����Ȓl
	XMVECTOR m;
	//��p0_p1�ɂ���
	XMVECTOR pt_p0 = triangle.p0 - interPlane;
	XMVECTOR p0_p1 = triangle.p1 - triangle.p0;
	m = XMVector3Cross(pt_p0, p0_p1);
	//�ӂ̊O���ł���Γ������Ă��Ȃ��̂Ŕ����ł��؂�
	if (XMVector3Dot(m, triangle.normal).m128_f32[0] < -epsilon) { return false; }

	//��p1_p2�ɂ���
	XMVECTOR pt_p1 = triangle.p1 - interPlane;
	XMVECTOR p0_p2 = triangle.p2 - triangle.p1;
	m = XMVector3Cross(pt_p1, p0_p2);
	//�ӂ̊O���ł���Γ������Ă��Ȃ��̂Ŕ����ł��؂�
	if (XMVector3Dot(m, triangle.normal).m128_f32[1] < -epsilon) { return false; }

	//��p2_p0�ɂ���
	XMVECTOR pt_p2 = triangle.p2 - interPlane;
	XMVECTOR p0_p0 = triangle.p0 - triangle.p2;
	m = XMVector3Cross(pt_p2, p0_p0);
	//�ӂ̊O���ł���Γ������Ă��Ȃ��̂Ŕ����ł��؂�
	if (XMVector3Dot(m, triangle.normal).m128_f32[2] < -epsilon) { return false; }

	//�����Ȃ̂ŁA�������Ă���
	if(inter) 
	{
		*inter = interPlane;
	}

	return true;
}

bool Collision::CheckRay2Sphere(const Ray &ray, const Sphere &sphere, float *distance, DirectX::XMVECTOR *inter)
{
	XMVECTOR m = ray.start - sphere.center;
	float b = XMVector3Dot(m, ray.dir).m128_f32[0];
	float c = XMVector3Dot(m, m).m128_f32[0] - sphere.radius * sphere.radius;
	//lay�̎n�_��sphere�̊O���ɂ���(c > 0)�Alay��sphere���痣��Ă������������������Ă���ꍇ(b > 0)�A������Ȃ�
	if (c > 0.0f && b > 0.0f) return false;

	float discr = b * b - c;
	//���̔��ʎ��̓��C�������O��Ă��邱�ƂɈ�v
	if (discr < 0.0f) { return false; }

	//���C�͋��ƌ������Ă���
	//��������ŏ��̒lt���v�Z
	float t = -b - sqrtf(discr);
	//t�����ł���ꍇ�A���C�͋��̓�������J�n���Ă���̂�t���[���ɃN�����v
	if (t < 0) t = 0.0f;
	if (distance) { *distance = 1; }

	if (inter) { *inter = ray.start + t * ray.dir; }

	return true;
}

bool Collision::CheckLineSegmentBox(const LineSegment& line, const Box& box)
{
	if (line.start.x <= line.end.x)
	{
		if (line.start.x > box.MaxPos.x)
		{
			return false;
		}

		if (line.end.x < box.LeastPos.x)
		{
			return false;
		}
	}
	else
	{
		if (line.end.x > box.MaxPos.x)
		{
			return false;
		}

		if (line.start.x < box.LeastPos.x)
		{
			return false;
		}
	}

	if (line.start.y <= line.end.y)
	{
		if (line.start.y > box.MaxPos.y)
		{
			return false;
		}

		if (line.end.y < box.LeastPos.y)
		{
			return false;
		}
	}
	else
	{
		if (line.end.y > box.MaxPos.y)
		{
			return false;
		}

		if (line.start.y < box.LeastPos.y)
		{
			return false;
		}
	}

	if (line.start.z <= line.end.z)
	{
		if (line.start.z > box.MaxPos.z)
		{
			return false;
		}

		if (line.end.z < box.LeastPos.z)
		{
			return false;
		}
	}
	else
	{
		if (line.end.z > box.MaxPos.z)
		{
			return false;
		}

		if (line.start.z < box.LeastPos.z)
		{
			return false;
		}
	}



	return true;
}

bool Collision::CheckBoxSphere(const SphereF& sphere, const Box& box)
{
	int Check1 = 0;
	int Check2 = 0;
	int Check3 = 0;
	
	Cylinder LXLY, LXMY, MXLY, MXMY;

	//
	if (box.LeastPos.x <= sphere.center.x && sphere.center.x <= box.MaxPos.x)
	{
		Check1 += 1;
	}
	if (box.LeastPos.y - sphere.radius <= sphere.center.y && sphere.center.y <= box.MaxPos.y + sphere.radius)
	{
		Check1 += 1;
	}
	if (box.LeastPos.z <= sphere.center.z && sphere.center.z <= box.MaxPos.z)
	{
		Check1 += 1;
	}


	//
	if (box.LeastPos.x - sphere.radius <= sphere.center.x && sphere.center.x <= box.MaxPos.x + sphere.radius)
	{
		Check2 += 1;
	}
	if (box.LeastPos.y <= sphere.center.y && sphere.center.y <= box.MaxPos.y)
	{
		Check2 += 1;
	}
	if (box.LeastPos.z <= sphere.center.z && sphere.center.z <= box.MaxPos.z)
	{
		Check2 += 1;
	}

	//
	if (box.LeastPos.x <= sphere.center.x && sphere.center.x <= box.MaxPos.x)
	{
		Check3 += 1;
	}
	if (box.LeastPos.y <= sphere.center.y && sphere.center.y <= box.MaxPos.y)
	{
		Check3 += 1;
	}
	if (box.LeastPos.z - sphere.radius <= sphere.center.z && sphere.center.z <= box.MaxPos.z + sphere.radius)
	{
		Check3 += 1;
	}


	//XY��
	LXLY.radius = sphere.radius;
	LXMY.radius = sphere.radius;
	MXLY.radius = sphere.radius;
	MXMY.radius = sphere.radius;
	
	LXLY.center = XMFLOAT3(box.LeastPos.x, box.LeastPos.y, box.centerPos.z);
	LXMY.center = XMFLOAT3(box.LeastPos.x, box.MaxPos.y, box.centerPos.z);
	MXLY.center = XMFLOAT3(box.MaxPos.x, box.LeastPos.y, box.centerPos.z);
	MXMY.center = XMFLOAT3(box.MaxPos.x, box.MaxPos.y, box.centerPos.z);

	LXLY.top = box.MaxPos.z;
	LXMY.top = box.MaxPos.z;
	MXLY.top = box.MaxPos.z;
	MXMY.top = box.MaxPos.z;

	LXLY.bottom = box.LeastPos.z;
	LXMY.bottom = box.LeastPos.z;
	MXLY.bottom = box.LeastPos.z;
	MXMY.bottom = box.LeastPos.z;

	if (CheckCylinderDotXY(LXLY, sphere.center) == true) return true;
	if (CheckCylinderDotXY(LXMY, sphere.center) == true) return true;
	if (CheckCylinderDotXY(MXLY, sphere.center) == true) return true;
	if (CheckCylinderDotXY(MXMY, sphere.center) == true) return true;

	//XZ��
	
	LXLY.radius = sphere.radius;
	LXMY.radius = sphere.radius;
	MXLY.radius = sphere.radius;
	MXMY.radius = sphere.radius;

	LXLY.center = XMFLOAT3(box.LeastPos.x, box.LeastPos.y, box.centerPos.z);
	LXMY.center = XMFLOAT3(box.LeastPos.x, box.MaxPos.y, box.centerPos.z);
	MXLY.center = XMFLOAT3(box.MaxPos.x, box.LeastPos.y, box.centerPos.z);
	MXMY.center = XMFLOAT3(box.MaxPos.x, box.MaxPos.y, box.centerPos.z);

	LXLY.top = box.MaxPos.y;
	LXMY.top = box.MaxPos.y;
	MXLY.top = box.MaxPos.y;
	MXMY.top = box.MaxPos.y;

	LXLY.bottom = box.LeastPos.y;
	LXMY.bottom = box.LeastPos.y;
	MXLY.bottom = box.LeastPos.y;
	MXMY.bottom = box.LeastPos.y;

	if (CheckCylinderDotXZ(LXLY, sphere.center) == true) return true;
	if (CheckCylinderDotXZ(LXMY, sphere.center) == true) return true;
	if (CheckCylinderDotXZ(MXLY, sphere.center) == true) return true;
	if (CheckCylinderDotXZ(MXMY, sphere.center) == true) return true;
	
	//YZ��
	LXLY.radius = sphere.radius;
	LXMY.radius = sphere.radius;
	MXLY.radius = sphere.radius;
	MXMY.radius = sphere.radius;

	LXLY.center = XMFLOAT3(box.LeastPos.x, box.LeastPos.y, box.centerPos.z);
	LXMY.center = XMFLOAT3(box.LeastPos.x, box.MaxPos.y, box.centerPos.z);
	MXLY.center = XMFLOAT3(box.MaxPos.x, box.LeastPos.y, box.centerPos.z);
	MXMY.center = XMFLOAT3(box.MaxPos.x, box.MaxPos.y, box.centerPos.z);

	LXLY.top = box.MaxPos.x;
	LXMY.top = box.MaxPos.x;
	MXLY.top = box.MaxPos.x;
	MXMY.top = box.MaxPos.x;

	LXLY.bottom = box.LeastPos.x;
	LXMY.bottom = box.LeastPos.x;
	MXLY.bottom = box.LeastPos.x;
	MXMY.bottom = box.LeastPos.x;

	if (CheckCylinderDotZY(LXLY, sphere.center) == true) return true;
	if (CheckCylinderDotZY(LXMY, sphere.center) == true) return true;
	if (CheckCylinderDotZY(MXLY, sphere.center) == true) return true;
	if (CheckCylinderDotZY(MXMY, sphere.center) == true) return true;

	if (Check1 >= 3 || Check2 >= 3 || Check3 >= 3)
	{
		return true;
	}

	SphereF LXLYLZ; //�����O
	SphereF LXLYMZ; //������
	SphereF MXLYLZ; //�E���O
	SphereF MXLYMZ; //�E����
	SphereF LXMYLZ; //����O
	SphereF LXMYMZ; //���㉜
	SphereF MXMYLZ; //�E��O
	SphereF MXMYMZ; //�E�㉜

	LXLYLZ.center = XMFLOAT3(box.LeastPos.x, box.LeastPos.y, box.LeastPos.z);
	LXLYMZ.center = XMFLOAT3(box.LeastPos.x, box.LeastPos.y, box.MaxPos.z);
	MXLYLZ.center = XMFLOAT3(box.MaxPos.x  , box.LeastPos.y, box.LeastPos.z);
	MXLYMZ.center = XMFLOAT3(box.MaxPos.x  , box.LeastPos.y, box.MaxPos.z);
	LXMYLZ.center = XMFLOAT3(box.LeastPos.x, box.MaxPos.y  , box.LeastPos.z);
	LXMYMZ.center = XMFLOAT3(box.LeastPos.x, box.MaxPos.y  , box.MaxPos.z);
	MXMYLZ.center = XMFLOAT3(box.MaxPos.x  , box.MaxPos.y  , box.LeastPos.z);
	MXMYMZ.center = XMFLOAT3(box.MaxPos.x  , box.MaxPos.y  , box.MaxPos.z);

	LXLYLZ.radius = sphere.radius;
	LXLYMZ.radius = sphere.radius;
	MXLYLZ.radius = sphere.radius;
	MXLYMZ.radius = sphere.radius;
	LXMYLZ.radius = sphere.radius;
	LXMYMZ.radius = sphere.radius;
	MXMYLZ.radius = sphere.radius;
	MXMYMZ.radius = sphere.radius;

	XMFLOAT3 dot = sphere.center;

	if(CheckSphereDot(LXLYLZ ,dot) == true) return true;
	if(CheckSphereDot(LXLYMZ ,dot) == true) return true;
	if(CheckSphereDot(MXLYLZ ,dot) == true) return true;
	if(CheckSphereDot(MXLYMZ ,dot) == true) return true;
	if(CheckSphereDot(LXMYLZ ,dot) == true) return true;
	if(CheckSphereDot(LXMYMZ ,dot) == true) return true;
	if(CheckSphereDot(MXMYLZ ,dot) == true) return true;
	if(CheckSphereDot(MXMYMZ ,dot) == true) return true;

	return false;
}

bool Collision::CheckAABB(const Box& box1, const Box& box2)
{
	if (box1.LeastPos.x > box2.MaxPos.x) return false;
	if (box1.MaxPos.x < box2.LeastPos.x) return false;
	if (box1.LeastPos.y > box2.MaxPos.y) return false;
	if (box1.MaxPos.y < box2.LeastPos.y) return false;
	if (box1.LeastPos.z > box2.MaxPos.z) return false;
	if (box1.MaxPos.z < box2.LeastPos.z) return false;

	return true;
}

bool Collision::CheckCircleDot(const Circle& circle, const XMFLOAT2& dot)
{
	float a = dot.x - circle.center.x;
	float b = dot.y - circle.center.y;
	float c = sqrt(a * a + b * b);

	if (c <= circle.radius)
	{
		return true;
	}

	return false;
}

bool Collision::CheckCylinderDotXY(const Cylinder& cylinder, const XMFLOAT3& dot)
{
	if (cylinder.top < dot.z || cylinder.bottom > dot.z)
	{
		return false;
	}
	

	Circle circle;
	circle.center = XMFLOAT2(cylinder.center.x, cylinder.center.y);
	circle.radius = cylinder.radius;

	XMFLOAT2 Dot = XMFLOAT2(dot.x, dot.y);
	if(CheckCircleDot(circle, Dot) == true ) return true;
}

bool Collision::CheckCylinderDotXZ(const Cylinder& cylinder, const XMFLOAT3& dot)
{
	if (cylinder.top < dot.y || cylinder.bottom > dot.y)
	{
		return false;
	}

	Circle circle;
	circle.center = XMFLOAT2(cylinder.center.x, cylinder.center.z);
	circle.radius = cylinder.radius;

	XMFLOAT2 Dot = XMFLOAT2(dot.x, dot.z);
	CheckCircleDot(circle, Dot);
}

bool Collision::CheckCylinderDotZY(const Cylinder& cylinder, const XMFLOAT3& dot)
{
	if (cylinder.top < dot.x || cylinder.bottom > dot.x)
	{
		return false;
	}
	

	Circle circle;
	circle.center = XMFLOAT2(cylinder.center.z, cylinder.center.y);
	circle.radius = cylinder.radius;

	XMFLOAT2 Dot = XMFLOAT2(dot.z, dot.y);
	CheckCircleDot(circle, Dot);
}

bool Collision::CheckSphereDot(const SphereF& sphere, XMFLOAT3& dot)
{
	float a = dot.x - sphere.center.x;
	float b = dot.y - sphere.center.y;
	float c = dot.z - sphere.center.z;
	float d = sqrt(a * a + b * b + c * c);

	if (d <= sphere.radius)
	{
		return true;
	}

	return false;
}
