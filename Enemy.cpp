#include "Enemy.h"
#include "math.h"
#include "JsonLoader.h"
#include "CollisionSet.h"
#include "MyMath.h"

Enemy::XMFLOAT3 Enemy::pos = { 0.0f, 0.0f, 3.0f };
Enemy::XMFLOAT3 Enemy::initialPos = { 0.0f, 0.0f, 0.0f };
float Enemy::monitoringEnemyColliderScale = 2.0f;
bool Enemy::chengeScaleFlag = false;
int Enemy::nearNumber = 0;

float Enemy::speedConstant = 10.0f;

bool Enemy::raidMode = false;
bool Enemy::gameOverFlag = false;

void Enemy::Tracking(const XMFLOAT3& playerPos)
{

	XMFLOAT3 dis = { playerPos.x - pos.x, playerPos.y - pos.y ,playerPos.z - pos.z };

	float sb = sqrt(dis.x * dis.x + dis.z * dis.z);

	pos.x += (dis.x / sb) / speedConstant;
	pos.z += (dis.z / sb) / speedConstant;
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

	//敵機(自機)から123番目に近い頂点を求める
	XMFLOAT3 first, second, third, fourth;
	first = CollisionSet::GetNearVertex(box,pos);
	second = CollisionSet::GetScecondNearVertex(box, pos);
	third = CollisionSet::GetThirdNearVertex(box, pos);
	fourth = CollisionSet::GetFourthNearVertex(box, pos);
	//進行方向の判別
	LineSegment line;
	line.end = playerPos;


	//敵機(自機)から1番近い頂点から自機に行けるか？
	line.start = first;
	if (Collision::CheckLineSegmentBox(line, wall) == false) //もし遮蔽が無ければ
	{
		//追尾処理
		XMFLOAT3 dis = { first.x - pos.x, first.y - pos.y ,first.z - pos.z };

		float sb = sqrt(dis.x * dis.x + dis.z * dis.z);

		pos.x += (dis.x / sb) / speedConstant;
		pos.z += (dis.z / sb) / speedConstant;

		nearNumber = 1;

		return;
	}

	//1番近い頂点で、いけない場合2か3番目に近い頂点で行ける。
	//敵機から2,3番目に近い頂点から行ける方または両方いける場合近い方
	line.start = second;

	//2番目だけ行ける
	if (Collision::CheckLineSegmentBox(line, wall) == false)
	{
		//追尾処理
		XMFLOAT3 dis = { second.x - pos.x, second.y - pos.y ,second.z - pos.z };

		float sb = sqrt(dis.x * dis.x + dis.z * dis.z);

		pos.x += (dis.x / sb) / speedConstant;
		pos.z += (dis.z / sb) / speedConstant;

		nearNumber = 2;

		return;
	}

	//3番目だけ行ける
	line.start = CollisionSet::GetwhichNearPos(playerPos, third, fourth);
	if (line.start.x == third.x && line.start.z == third.z)
	{

		//追尾処理
		XMFLOAT3 dis = { second.x - pos.x, second.y - pos.y , second.z - pos.z };

		float sb = sqrt(dis.x * dis.x + dis.z * dis.z);

		pos.x += (dis.x / sb) / speedConstant;
		pos.z += (dis.z / sb) / speedConstant;


		nearNumber = 3;

		return;
	}
	else
	{

		//追尾処理
		XMFLOAT3 dis = { first.x - pos.x, first.y - pos.y , first.z - pos.z };

		float sb = sqrt(dis.x * dis.x + dis.z * dis.z);

		pos.x += (dis.x / sb) / speedConstant;
		pos.z += (dis.z / sb) / speedConstant;

		nearNumber = 4;

		return;
	}


}

void Enemy::Initialize()
{
	gameOverFlag = false;

	pos = JsonLoader::raidEnemyObjects[0].get()->GetPosition();
	initialPos = JsonLoader::raidEnemyObjects[0].get()->GetPosition();
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

	//遮蔽物による行動パターン変化
	if (Collision::CheckLineSegmentBox(line, wall) == true)
	{
		ShieldDodge(playerPos, wall);
	}
	else //ないとき
	{
		Tracking(playerPos);
	}

}

void Enemy::PushBack(const Box& wall)
{
}

Enemy::XMFLOAT3 Enemy::MonitoringCollisionScale()
{
	if (monitoringEnemyColliderScale <= 2.0f)
	{
		chengeScaleFlag = true;
	}
	else if (monitoringEnemyColliderScale >= 15.0f)
	{
		chengeScaleFlag = false;
	}

	if (chengeScaleFlag == true)
	{
		monitoringEnemyColliderScale += 0.01f;
	}
	else
	{
		monitoringEnemyColliderScale -= 0.01f;
	}

	return { monitoringEnemyColliderScale , monitoringEnemyColliderScale ,monitoringEnemyColliderScale };
}

void Enemy::Update(int time, const XMFLOAT3& playerPos)
{
	if (time <= 0 && raidMode == false)
	{
		raidMode = true;
	}
	else if (raidMode == true && time != 60)
	{
		raidMode = true;
	}
	else if (time == 60.0f)
	{
		raidMode = false;
	}

	if (raidMode == true)		//襲撃モード時
	{
		Tracking(playerPos);

		SphereF player, Enemy;

		Enemy.center = pos;
		Enemy.radius = 5.0f;

		player.center = playerPos;
		player.radius = 5.0f;

		if (Collision::CheckSphereSphere(player, Enemy) == true)
		{
			gameOverFlag = true;
		}
		
	}
	else						//非襲撃モード時
	{
		for (int i = 0; i < JsonLoader::enemyObjects.size(); i++)
		{
			SphereF sphere;
			sphere.center = JsonLoader::enemyObjects[i].get()->GetPosition();
			sphere.radius = monitoringEnemyColliderScale;

			XMFLOAT3 Ppos = playerPos;
			if (Collision::CheckSphereDot(sphere, Ppos))
			{
				raidMode = true;
			}
		}
		

		pos = initialPos;
	}

	JsonLoader::raidEnemyObjects[0].get()->SetPosition(pos);
}
