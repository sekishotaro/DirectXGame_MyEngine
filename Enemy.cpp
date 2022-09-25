#include "Enemy.h"
#include "math.h"
#include "JsonLoader.h"
#include "CollisionSet.h"
#include "MyMath.h"

Enemy::XMFLOAT3 Enemy::pos = { 0.0f, 0.0f, 3.0f };

int Enemy::nearNumber = 0;

void Enemy::Tracking(const XMFLOAT3& playerPos)
{
	//if (CollisionSet::CollisionCheck1(pos, XMFLOAT3(2,2,2), JsonLoader::enemyNaviareaObjects[0].get()->GetPosition(), JsonLoader::enemyNaviareaObjects[0].get()->GetScale()) == false )
	//{
	//	return;
	//}

	XMFLOAT3 dis = { playerPos.x - pos.x, playerPos.y - pos.y ,playerPos.z - pos.z };

	float sb = sqrt(dis.x * dis.x + dis.z * dis.z);

	pos.x += (dis.x / sb) / 10.0f;
	pos.z += (dis.z / sb) / 10.0f;
}

void Enemy::ShieldDodge(const XMFLOAT3& playerPos, const Box& wall)
{
	Box box;
	box.centerPos = wall.centerPos;
	box.LeastPos.x = wall.LeastPos.x - 2;
	box.LeastPos.y = wall.LeastPos.y - 2;
	box.LeastPos.z = wall.LeastPos.z - 2;
	box.MaxPos.x = wall.MaxPos.x + 2;
	box.MaxPos.y = wall.MaxPos.y + 2;
	box.MaxPos.z = wall.MaxPos.z + 2;

	//�G�@(���@)����123�Ԗڂɋ߂����_�����߂�
	XMFLOAT3 first, second, third, fourth;
	first = CollisionSet::GetNearVertex(box,pos);
	second = CollisionSet::GetScecondNearVertex(box, pos);
	third = CollisionSet::GetThirdNearVertex(box, pos);
	fourth = CollisionSet::GetFourthNearVertex(box, pos);
	//�i�s�����̔���
	LineSegment line;
	line.end = playerPos;


	//�G�@(���@)����1�ԋ߂����_���玩�@�ɍs���邩�H
	line.start = first;
	if (Collision::CheckLineSegmentBox(line, wall) == false) //�����Օ����������
	{
		//�ǔ�����
		XMFLOAT3 dis = { first.x - pos.x, first.y - pos.y ,first.z - pos.z };

		float sb = sqrt(dis.x * dis.x + dis.z * dis.z);

		pos.x += (dis.x / sb) / 10.0f;
		pos.z += (dis.z / sb) / 10.0f;

		nearNumber = 1;

		return;
	}

	//1�ԋ߂����_�ŁA�����Ȃ��ꍇ2��3�Ԗڂɋ߂����_�ōs����B
	//�G�@����2,3�Ԗڂɋ߂����_����s������܂��͗���������ꍇ�߂���
	line.start = second;

	//2�Ԗڂ����s����
	if (Collision::CheckLineSegmentBox(line, wall) == false)
	{
		//�ǔ�����
		XMFLOAT3 dis = { second.x - pos.x, second.y - pos.y ,second.z - pos.z };

		float sb = sqrt(dis.x * dis.x + dis.z * dis.z);

		pos.x += (dis.x / sb) / 10.0f;
		pos.z += (dis.z / sb) / 10.0f;

		nearNumber = 2;

		return;
	}

	//3�Ԗڂ����s����
	line.start = CollisionSet::GetwhichNearPos(playerPos, third, fourth);
	if (line.start.x == third.x && line.start.z == third.z)
	{

		//�ǔ�����
		XMFLOAT3 dis = { second.x - pos.x, second.y - pos.y , second.z - pos.z };

		float sb = sqrt(dis.x * dis.x + dis.z * dis.z);

		pos.x += (dis.x / sb) / 10.0f;
		pos.z += (dis.z / sb) / 10.0f;


		nearNumber = 3;

		return;
	}
	else
	{

		//�ǔ�����
		XMFLOAT3 dis = { first.x - pos.x, first.y - pos.y , first.z - pos.z };

		float sb = sqrt(dis.x * dis.x + dis.z * dis.z);

		pos.x += (dis.x / sb) / 10.0f;
		pos.z += (dis.z / sb) / 10.0f;

		nearNumber = 4;

		return;
	}


}

void Enemy::Initialize()
{
	pos = JsonLoader::enemyObjects[0].get()->GetPosition();
}

void Enemy::Move(const XMFLOAT3& playerPos)
{
	LineSegment line;
	line.start = Enemy::GetPos();
	line.end = playerPos;

	Box wall;
	wall.centerPos = JsonLoader::colliderObjects[0].get()->GetPosition();
	wall.size = JsonLoader::colliderObjects[0].get()->GetScale();
	wall.LeastPos = { wall.centerPos.x - wall.size.x / 2,wall.centerPos.y - wall.size.y / 2, wall.centerPos.z - wall.size.z / 2 };
	wall.MaxPos = { wall.centerPos.x + wall.size.x / 2,wall.centerPos.y + wall.size.y / 2, wall.centerPos.z + wall.size.z / 2 };

	//�Օ����ɂ��s���p�^�[���ω�
	if (Collision::CheckLineSegmentBox(line, wall) == true)
	{
		ShieldDodge(playerPos, wall);
	}
	else //�Ȃ��Ƃ�
	{
		Tracking(playerPos);
	}

}

void Enemy::PushBack(const Box& wall)
{
}
