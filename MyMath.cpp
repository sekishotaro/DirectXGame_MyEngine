#include "MyMath.h"

const float MyMath::k = 0.01f;
const float MyMath::gravity = 9.8f;
const float MyMath::graAdjustConstant = 10.0f;
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
