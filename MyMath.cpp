#include "MyMath.h"

const float MyMath::k = 0.01f;
const float MyMath::gravity = 9.8f;
const float MyMath::graAdjustConstant = 10.0f;
const float MyMath::friction = 0.8f;
const float MyMath::radian = 3.14f / 180.0f;
float MyMath::graValue = 0.0f;
float MyMath::airResistance = 0.0f;

MyMath::MyMath()
{
}

MyMath::~MyMath()
{
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
	graValue += 0.1;
}

void MyMath::GravityCheck(XMFLOAT3& pos, float& groundY, bool& graFlag)
{
	if (graFlag == true)
	{
		return;
	}

	if (pos.y <= groundY)
	{
		graFlag = true;
		pos.y = 0.0f;
	}
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

void MyMath::CircleMovement(XMFLOAT3 pos1, XMFLOAT3& pos2, XMFLOAT3& move, float& radiusC, float& angle)
{
	float radius = angle * radian;

	XMFLOAT3 oldPos2 = pos2;

	pos2.x = pos1.x + cos(radius) * radiusC;
	pos2.y = pos1.y + sin(radius) * radiusC;

	move.x = pos2.x - oldPos2.x;
	move.y = pos2.y - oldPos2.y;
	move.z = pos2.z - oldPos2.z;
}
