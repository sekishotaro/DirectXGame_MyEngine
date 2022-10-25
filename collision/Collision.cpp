#include "Collision.h"
#include "MyMath.h"

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

bool Collision::CheckSphere2Sphere(const Sphere& sphere1, const Sphere& sphere2, DirectX::XMVECTOR* inter, DirectX::XMVECTOR* reject)
{
	// ���S�_�̋����̂Q�� <= ���a�̘a�̂Q��@�Ȃ����
	float dist = XMVector3LengthSq(sphere1.center - sphere2.center).m128_f32[0];

	float radius2 = sphere1.radius + sphere2.radius;
	radius2 *= radius2;

	if (dist <= radius2) {
		if (inter) {
			// A�̔��a��0�̎����W��B�̒��S�@B�̔��a��0�̎����W��A�̒��S�@�ƂȂ�悤�⊮
			float t = sphere2.radius / (sphere1.radius + sphere2.radius);
			*inter = XMVectorLerp(sphere1.center, sphere2.center, t);
		}
		if (reject)
		{
			float rejectLen = sphere1.radius + sphere2.radius - sqrtf(dist);
			*reject = XMVector3Normalize(sphere1.center - sphere2.center);
			*reject *= rejectLen;
		}
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

bool Collision::CheckSphere2Triangle(const Sphere& sphere, const Triangle& triangle, DirectX::XMVECTOR* inter, DirectX::XMVECTOR* reject)
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
	//�����o���x�N�g�����v�Z
	if (reject)
	{
		float ds = XMVector3Dot(sphere.center, triangle.normal).m128_f32[0];
		float dt = XMVector3Dot(triangle.p0, triangle.normal).m128_f32[0];
		float rejectLen = dt - ds + sphere.radius;
		*reject = triangle.normal * rejectLen;
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
	XMVECTOR vec1 = XMVector3Dot(plane.normal, ray.start);
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
	LineSegment a_to_b, b_to_c, c_to_d, d_to_a;

	a_to_b.start = XMFLOAT3(box.LeastPos.x, 0, box.LeastPos.z);
	a_to_b.end   = XMFLOAT3(box.LeastPos.x, 0, box.MaxPos.z);

	b_to_c.start = XMFLOAT3(box.LeastPos.x, 0, box.MaxPos.z);
	b_to_c.end   = XMFLOAT3(box.MaxPos.x  , 0, box.MaxPos.z);

	c_to_d.start = XMFLOAT3(box.MaxPos.x  , 0, box.MaxPos.z);
	c_to_d.end   = XMFLOAT3(box.MaxPos.x  , 0, box.LeastPos.z);

	d_to_a.start = XMFLOAT3(box.MaxPos.x  , 0, box.LeastPos.z);
	d_to_a.end   = XMFLOAT3(box.LeastPos.x, 0, box.LeastPos.z);

	if (Check2LineSegment(line, a_to_b) == true)
	{
		return true;
	}
	if (Check2LineSegment(line, b_to_c) == true)
	{
		return true;
	}
	if (Check2LineSegment(line, c_to_d) == true)
	{
		return true;
	}
	if (Check2LineSegment(line, d_to_a) == true)
	{
		return true;
	}
	
	return false;
}

bool Collision::CheckRayBox(const Ray& ray, const Box& box)
{
	int sign = 0;
	XMFLOAT3 pos[4];
	pos[0] = { box.LeastPos.x, box.LeastPos.y, box.LeastPos.z };
	pos[1] = { box.LeastPos.x, box.LeastPos.y,   box.MaxPos.z };
	pos[2] = { box.MaxPos.x, box.LeastPos.y,   box.MaxPos.z };
	pos[3] = { box.MaxPos.x, box.LeastPos.y, box.LeastPos.z };

	for (int i = 0; i < 4; i++)
	{
		XMFLOAT3 v1 = { ray.start.m128_f32[0], ray.start.m128_f32[1], ray.start.m128_f32[2] };

		XMFLOAT3 v2 = { v1.x - pos[i].x, v1.y - pos[i].y, v1.z - pos[i].z };

		float dd = ray.dir.m128_f32[0] * v2.z - ray.dir.m128_f32[2] * v2.x;

		if (dd == 0)
		{
			return true;
		}

		if (i == 0)
		{
			sign = (int)MyMath::Sign(dd);
		}
		else
		{
			if (sign != (int)MyMath::Sign(dd))
			{
				return true;
			}
		}
	}
	return false;
}

Collision::XMVECTOR Collision::CheckRayBoxforPlane(const Ray& ray, const Box& box)
{
	Ray r1;
	r1.start = ray.start;	//�����ʒu
	r1.dir = ray.dir;		//����


	int count = 10;			//�J�E���g
	float distance = 0.0f;	//�����ʒu���烌�C�ƕ��ʂ����������Ƃ���̒���
	float dis = 0.0f;		//��r����Ƃ��̋L�^�p

	Plane plane[6];
	plane[0].normal = {  1,  0,  0,  0};	//�E��
	plane[1].normal = { -1,  0,  0,  0};	//����
	plane[2].normal = {  0,  1,  0,  0};	//���
	plane[3].normal = {  0, -1,  0,  0};	//����
	plane[4].normal = {  0,  0,  1,  0};	//����
	plane[5].normal = {  0,  0, -1,  0};	//�O��
	
	plane[0].distance = box.MaxPos.x;
	plane[1].distance = -box.LeastPos.x;
	plane[2].distance = box.MaxPos.y;
	plane[3].distance = -box.LeastPos.y;
	plane[4].distance = box.MaxPos.z;
	plane[5].distance = -box.LeastPos.z;


	for (int i = 0; i < 6; i++)
	{
		if (CheckRay2Plane(r1, plane[i], &distance))
		{
			if (count != 10 && dis <= distance)
			{
				distance = dis;
				return plane[count].normal;
			}
			dis = distance;
			count = i;
		}
	}

	//���C���{�b�N�X�̂ǂ̖ʂƂ��������Ă��Ȃ�
	if (count == 10)
	{
		return XMVECTOR{ 0, 0, 0, 0 };
	}
	
	return plane[count].normal;
}

bool Collision::Check2Box(const Box& box1, const Box& box2, XMFLOAT3& distance)
{
	if (box1.LeastPos.x > box2.MaxPos.x) return false;
	if (box1.MaxPos.x < box2.LeastPos.x) return false;
	if (box1.LeastPos.y > box2.MaxPos.y) return false;
	if (box1.MaxPos.y < box2.LeastPos.y) return false;
	if (box1.LeastPos.z > box2.MaxPos.z) return false;
	if (box1.MaxPos.z < box2.LeastPos.z) return false;

	float x1, x2, y1, y2, z1, z2;

	x1 = box2.LeastPos.x - box1.MaxPos.x;
	y1 = box2.LeastPos.y - box1.MaxPos.y;
	z1 = box2.LeastPos.z - box1.MaxPos.z;

	x2 = box2.MaxPos.x - box1.LeastPos.x;
	y2 = box2.MaxPos.y - box1.LeastPos.y;
	z2 = box2.MaxPos.z - box1.LeastPos.z;

	if ( fabsf(x1) < fabsf(x2))
	{
		distance.x = x1;
	}
	else
	{
		distance.x = x2;
	}
	
	if (fabsf(y1) < fabsf(y2))
	{
		distance.y = y1;
	}
	else
	{
		distance.y = y2;
	}
	
	if (fabsf(z1) < fabsf(z2))
	{
		distance.z = z1;
	}
	else
	{
		distance.z = z2;
	}

	return true;
}

bool Collision::Check2LineSegment(const LineSegment& line1, const LineSegment& line2)
{

	float baseX = line2.end.x   - line2.start.x;
	float baseZ = line2.end.z   - line2.start.z;
	float sub1X = line1.start.x - line2.start.x;
	float sub1Z = line1.start.z - line2.start.z;
	float sub2X = line1.end.x   - line2.start.x;
	float sub2Z = line1.end.z   - line2.start.z;

	float bs1 = baseX * sub1Z - baseZ * sub1X;
	float bs2 = baseX * sub2Z - baseZ * sub2X;
	float re = bs1 * bs2;
	if (re > 0)
	{
		return false;
	}

	baseX = line1.end.x - line1.start.x;
	baseZ = line1.end.z - line1.start.z;
	sub1X = line2.start.x - line1.start.x;
	sub1Z = line2.start.z - line1.start.z;
	sub2X = line2.end.x - line1.start.x;
	sub2Z = line2.end.z - line1.start.z;

	bs1 = baseX * sub1Z - baseZ * sub1X;
	bs2 = baseX * sub2Z - baseZ * sub2X;
	re = bs1 * bs2;
	if (re > 0)
	{
		return false;
	}

	return true;
}

bool Collision::CheckPlateRay(const Plate& plate, const Ray& ray)
{
	////���ʂƂ����Ƃ��̌��_����̋��������߂�
	//float dis = (plate.position.m128_f32[0] * plate.position.m128_f32[0]) + (plate.position.m128_f32[1] * plate.position.m128_f32[1]) + (plate.position.m128_f32[2] * plate.position.m128_f32[2]);
	//dis = sqrtf(dis);
	//
	//Plane p1;
	//p1.distance = dis;
	//p1.normal = plate.normal;

	////��_
	//float distance;
	//XMVECTOR inter;
	//if (CheckRay2Plane(ray, p1, &distance, &inter) == false) return false;

	//�O�p�|���S���Ƃ̓����蔻��𒲂ׂ�

	Triangle pori1, pori2;
	
	pori1.p0 = plate.vert1;
	pori1.p1 = plate.vert2;
	pori1.p2 = plate.vert3;
	
	pori2.p0 = plate.vert4;
	pori2.p1 = plate.vert5;
	pori2.p2 = plate.vert6;
	
	pori1.normal = pori2.normal = plate.normal;

	if (CheckRay2Triangle(ray, pori1) == true) return true;
	if (CheckRay2Triangle(ray, pori2) == true) return true;

	return false;
}

bool Collision::CheckSphereBox(const SphereF& sphere, const Box& box)
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
	float c = (float)sqrt(a * a + b * b);

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
	float d = (float)sqrt(a * a + b * b + c * c);

	if (d <= sphere.radius)
	{
		return true;
	}

	return false;
}
