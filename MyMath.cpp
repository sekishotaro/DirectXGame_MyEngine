#include "MyMath.h"
#include "Player.h"

const float MyMath::k = 0.01f;
const float MyMath::gravity = 9.8f;
const float MyMath::graAdjustConstant = 20.0f;
const float MyMath::friction = 0.8f;
float MyMath::graValue = 0.0f;
float MyMath::airResistance = 0.0f;

MyMath::MyMath()
{
}

MyMath::~MyMath()
{
}

void MyMath::Movement(XMFLOAT3& pos, XMFLOAT3& move)
{
	pos.x += move.x;
	pos.y += move.y;
	pos.z += move.z;
}

void MyMath::Movement(XMFLOAT3& pos, XMFLOAT3& move, XMFLOAT3 direction)
{
	pos.x += move.x * direction.x;
	pos.y += move.y * direction.y;
	pos.z += move.z * direction.z;
}

void MyMath::Movement(XMFLOAT3& pos, XMFLOAT3& move, XMFLOAT3 direction, float& mass)
{
	float DXmass = mass / 1.5f;
	pos.x += move.x * direction.x * DXmass;
	pos.y += move.y * direction.y * DXmass;
	pos.z += move.z * direction.z * DXmass;
}

void MyMath::Friction(XMFLOAT3& move, bool& graFlag)
{
	if (graFlag == true)
	{
		move.x -= friction / 50.0f;
		if (move.x <= 0.0f)
		{
			move.x = 0.0f;
		}
	}
}

void MyMath::AirResistance(XMFLOAT3& move)
{
	airResistance = k * move.x;
	move.x -= airResistance;

	airResistance = k * move.y;
	move.y -= airResistance;

	airResistance = k * move.z;
	move.z -= airResistance;
}

void MyMath::Gravity(XMFLOAT3& pos, bool& graflag)
{
	if (graflag == true)
	{
		graValue = 0.0f;
		return;
	}

	pos.y -= gravity * graValue / graAdjustConstant;
	graValue += 0.1f;
}

void MyMath::GravityCheckMove(XMFLOAT3& move, bool& graFlag)
{
	if (graFlag == true)
	{
		move.y = 0.0f;
	}
}

void MyMath::CollisionRebound(XMFLOAT3& move1, XMFLOAT3& direction1, XMFLOAT3& move2, XMFLOAT3& direction2)
{
	XMFLOAT3 move	   = move1;
	XMFLOAT3 direction = direction1;

	move1	   = move2;
	direction1 = direction2;

	move2      = move;
	direction2 = direction;
}

void MyMath::CollisionRebound(XMFLOAT3& move1, XMFLOAT3& direction1, float& mass1, XMFLOAT3& move2, XMFLOAT3& direction2, float& mass2)
{
	XMFLOAT3 move = move1;
	XMFLOAT3 direction = direction1;
	float mass = mass1;

	move1 = move2;
	direction1 = direction2;
	mass1 = mass2;

	move2 = move;
	direction2 = direction;
	mass2 = mass;
}

void MyMath::CollisionReboundOn(XMFLOAT3& move1, XMFLOAT3& direction1, float& reboundFactor1, XMFLOAT3& move2, XMFLOAT3& direction2, float& reboundFactor2)
{
	XMFLOAT3 move = move1;
	XMFLOAT3 direction = direction1;

	move1 = { move2.x * reboundFactor1, move2.y * reboundFactor1 , move2.z * reboundFactor1 };
	direction1 = direction2;

	move2 = { move.x * reboundFactor1, move.y * reboundFactor1 , move.z * reboundFactor1 };
	direction2 = direction;
}

float MyMath::Sign(const float& num)
{
	if (num >= 0)
	{
		return 1;
	}
	else if (0 > num)
	{
		return -1;
	}

	return 0;
}

float MyMath::maxElement(const float* array, int size)
{
	assert(array != NULL);
	assert(size >= 1);

	float max = array[0];
	for (int i = 1; i < size; ++i) {
		if (max < array[i]) {
			max = array[i];
		}
	}

	return max;
}

float MyMath::minElement(const float* array, int size)
{
	assert(array != NULL);
	assert(size >= 1);

	float min = array[0];
	for (int i = 1; i < size; ++i) {
		if (min > array[i]) {
			min = array[i];
		}
	}

	return min;
}
