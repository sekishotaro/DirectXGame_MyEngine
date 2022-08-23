#include "Enemy.h"
#include "math.h"
#include "JsonLoader.h"
#include "CollisionSet.h"

Enemy::XMFLOAT3 Enemy::pos = { 0.0f, 0.0f, 3.0f };

void Enemy::Tracking(XMFLOAT3 playerPos)
{
	if (CollisionSet::CollisionCheck1(pos, XMFLOAT3(2,2,2), JsonLoader::enemyNaviareaObjects[0].get()->GetPosition(), JsonLoader::enemyNaviareaObjects[0].get()->GetScale()) == false )
	{
		return;
	}

	XMFLOAT3 dis = { playerPos.x - pos.x, playerPos.y - pos.y ,playerPos.z - pos.z };

	float sb = sqrt(dis.x * dis.x + dis.z * dis.z);

	pos.x += (dis.x / sb) / 10.0f;
	pos.z += (dis.z / sb) / 10.0f;
}

void Enemy::Initialize()
{
	pos = JsonLoader::enemyObjects[0].get()->GetPosition();
}
