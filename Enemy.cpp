#include "Enemy.h"
#include "math.h"

Enemy::XMFLOAT3 Enemy::pos = { 0.0f, 5.0f, 4.0f };

void Enemy::Tracking(XMFLOAT3 playerPos)
{
	XMFLOAT3 dis = { playerPos.x - pos.x, playerPos.y - pos.y ,playerPos.z - pos.z };

	float sb = sqrt(dis.x * dis.x + dis.z * dis.z);

	pos.x += (dis.x / sb) / 10.0f;
	pos.z += (dis.z / sb) / 10.0f;
}
