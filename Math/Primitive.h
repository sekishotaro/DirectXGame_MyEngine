#pragma once
#include "Vector3.h"

//球
struct Sphere
{
	Vector3 position; //中心座標
	float radius;     //半径

	unsigned color;   //表示色

	//コンストラクタ
	Sphere(const Vector3 &position, float radius, unsigned color);

	void draw();
	void drawInfo(int x, int y, unsigned color);
};

struct Box
{
	//xyzのそれぞれの最小値と最大値をデータとして持たせる方法
	Vector3 minPosition; //最小値 xyz
	Vector3 maxPosition; //最大値 xyz

	unsigned color;

	//コンストラクタ
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

	//コンストラクタ
	Capsule(const Vector3 &startPosition, const Vector3 &endPosition, float radius, unsigned color);

	void draw();
	void drawInfo(int x, int y, unsigned color);
};