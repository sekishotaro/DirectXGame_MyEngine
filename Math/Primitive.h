#pragma once
#include "Vector3.h"

//��
struct Sphere
{
	Vector3 position; //���S���W
	float radius;     //���a

	unsigned color;   //�\���F

	//�R���X�g���N�^
	Sphere(const Vector3 &position, float radius, unsigned color);

	void draw();
	void drawInfo(int x, int y, unsigned color);
};

struct Box
{
	//xyz�̂��ꂼ��̍ŏ��l�ƍő�l���f�[�^�Ƃ��Ď���������@
	Vector3 minPosition; //�ŏ��l xyz
	Vector3 maxPosition; //�ő�l xyz

	unsigned color;

	//�R���X�g���N�^
	Box(const Vector3 &minPosition, const Vector3 &maxPosition, unsigned color);

	void draw();
	void drawInfo(int x, int y, unsigned color);
};

struct Capsule
{
	Vector3 startPosition;
	Vector3 endPosition;
	float radius;

	unsigned color;

	//�R���X�g���N�^
	Capsule(const Vector3 &startPosition, const Vector3 &endPosition, float radius, unsigned color);

	void draw();
	void drawInfo(int x, int y, unsigned color);
};