#include "CollisionSet.h"
#include "JsonLoader.h"
#include "Player.h"
#include "MyMath.h"

bool CollisionSet::PlayerWallCollisionFlag = false;

//void CollisionSet::CollisionCheck(const XMFLOAT3& pos, const XMFLOAT3& size, float& groundY)
//{
//	Box colPlayer;
//	colPlayer.centerPos = pos;
//	colPlayer.size = { size.x / 2, size.y, size.z / 2 };
//	colPlayer.MaxPos = { colPlayer.centerPos.x + colPlayer.size.x,colPlayer.centerPos.y + colPlayer.size.y,colPlayer.centerPos.z + colPlayer.size.z };
//	colPlayer.LeastPos = { colPlayer.centerPos.x - colPlayer.size.x,colPlayer.centerPos.y,colPlayer.centerPos.z - colPlayer.size.z };
//
//	Box colBox[4];
//	for (int i = 0; i < JsonLoader::colliderObjects.size(); i++)
//	{
//		colBox[i].centerPos = JsonLoader::colliderObjects[i].get()->GetPosition();
//		colBox[i].size = {JsonLoader::colliderObjects[i].get()->GetScale().x / 2, JsonLoader::colliderObjects[i].get()->GetScale().y / 2, JsonLoader::colliderObjects[i].get()->GetScale().z / 2};
//		colBox[i].MaxPos = {colBox[i].centerPos.x + colBox[i].size.x,colBox[i].centerPos.y + colBox[i].size.y,colBox[i].centerPos.z + colBox[i].size.z};
//		colBox[i].LeastPos = { colBox[i].centerPos.x - colBox[i].size.x,colBox[i].centerPos.y - colBox[i].size.y,colBox[i].centerPos.z - colBox[i].size.z };
//	}
//	
//	PlayerWallCollisionFlag = false;
//	for (int i = 0; i < JsonLoader::colliderObjects.size(); i++)
//	{
//		if (Collision::CheckAABB(colPlayer, colBox[i]) == true)
//		{
//			PlayerWallCollisionFlag = true;
//			JsonLoader::colliderObjects[i].get()->SetCollFlag(true);
//		}
//		else
//		{
//			JsonLoader::colliderObjects[i].get()->SetCollFlag(false);
//		}
//	}
//	Player::SetWallColl(PlayerWallCollisionFlag);
//}
//
//void CollisionSet::CollisionPushBack( const XMFLOAT3& size, float& groundY)
//{
//	groundY = 0.0f;
//	for (int i = 0; i < JsonLoader::colliderObjects.size(); i++)
//	{
//		if (JsonLoader::colliderObjects[i]->GetCollFlag() == false)
//		{
//			continue;
//		}
//
//		Box colPlayer;
//		colPlayer.centerPos = { Player::GetPos().x, Player::GetPos().y + (size.y/2), Player::GetPos().z };
//		colPlayer.size = { size.x / 2, size.y / 2, size.z / 2 };
//		colPlayer.MaxPos   = { colPlayer.centerPos.x + colPlayer.size.x, colPlayer.centerPos.y + colPlayer.size.y,colPlayer.centerPos.z + colPlayer.size.z };
//		colPlayer.LeastPos = { colPlayer.centerPos.x - colPlayer.size.x, colPlayer.centerPos.y - colPlayer.size.y, colPlayer.centerPos.z - colPlayer.size.z };
//
//		Box colBox;
//		colBox.centerPos = JsonLoader::colliderObjects[i].get()->GetPosition();
//		colBox.size = { JsonLoader::colliderObjects[i].get()->GetScale().x / 2, JsonLoader::colliderObjects[i].get()->GetScale().y / 2, JsonLoader::colliderObjects[i].get()->GetScale().z / 2 };
//		colBox.MaxPos = { colBox.centerPos.x + colBox.size.x,colBox.centerPos.y + colBox.size.y,colBox.centerPos.z + colBox.size.z };
//		colBox.LeastPos = { colBox.centerPos.x - colBox.size.x,colBox.centerPos.y,colBox.centerPos.z - colBox.size.z };
//
//		XMFLOAT3 m1 = {0,0,0};
//		
//
//		//X
//		if (colPlayer.centerPos.z <= colBox.MaxPos.z && colBox.LeastPos.z <= colPlayer.centerPos.z)
//		{
//			if (colPlayer.centerPos.y <= colBox.MaxPos.y && colBox.LeastPos.y <= colPlayer.centerPos.y)
//			{
//				if (colPlayer.MaxPos.x >= colBox.MaxPos.x)
//				{
//					m1.x = colBox.MaxPos.x - colPlayer.LeastPos.x;
//				}
//				else
//				{
//					m1.x = colPlayer.MaxPos.x - colBox.LeastPos.x;
//					m1.x *= -1;
//				}
//			}
//		}
//		
//		//Z
//		if (colPlayer.centerPos.x <= colBox.MaxPos.x && colBox.LeastPos.x <= colPlayer.centerPos.x)
//		{
//			if (colPlayer.centerPos.y <= colBox.MaxPos.y && colBox.LeastPos.y <= colPlayer.centerPos.y)
//			{
//				if (colPlayer.MaxPos.z >= colBox.MaxPos.z)
//				{
//					m1.z = colBox.MaxPos.z - colPlayer.LeastPos.z;
//				}
//				else
//				{
//					m1.z = colPlayer.MaxPos.z - colBox.LeastPos.z;
//					m1.z *= -1;
//				}
//			}
//		}
//		
//		//Y
//		if (colPlayer.centerPos.z <= colBox.MaxPos.z && colBox.LeastPos.z <= colPlayer.centerPos.z)
//		{
//			if (colPlayer.centerPos.x <= colBox.MaxPos.x && colBox.LeastPos.x <= colPlayer.centerPos.x)
//			{
//				if (colPlayer.MaxPos.y >= colBox.MaxPos.y)
//				{
//					m1.y = colBox.MaxPos.y - colPlayer.LeastPos.y;
//					groundY = colBox.MaxPos.y;
//				}
//				else
//				{
//					m1.y = colPlayer.MaxPos.y - colBox.LeastPos.y;
//					m1.y *= -1;
//				}
//			}
//		}
//		
//
//		XMFLOAT3 pPos = Player::GetPos();
//
//		Player::MoveAdd(m1);
//	}
//}

bool CollisionSet::CollisionCheck1(const XMFLOAT3& pos1, const XMFLOAT3& size1, const XMFLOAT3& pos2, const XMFLOAT3& size2)
{
	Box col1;
	col1.centerPos = pos1;
	col1.size = { size1.x / 2, size1.y / 2, size1.z / 2 };
	col1.MaxPos = { col1.centerPos.x + col1.size.x,col1.centerPos.y + col1.size.y,col1.centerPos.z + col1.size.z };
	col1.LeastPos = { col1.centerPos.x - col1.size.x,col1.centerPos.y,col1.centerPos.z - col1.size.z };

	Box col2;
	col2.centerPos = pos2;
	col2.size = { size2.x / 2, size2.y / 2, size2.z / 2 };
	col2.MaxPos   = { col2.centerPos.x + col2.size.x, col2.centerPos.y + col2.size.y, col2.centerPos.z + col2.size.z };
	col2.LeastPos = { col2.centerPos.x - col2.size.x, col2.centerPos.y - col2.size.x, col2.centerPos.z - col2.size.z };

	if (Collision::CheckAABB(col1, col2) == true)
	{
		return true;
	}
	else
	{
		return false;
	}
}

CollisionSet::XMFLOAT3 CollisionSet::GetNearPos2D(const XMFLOAT3& pos, const Box& box)
{
	//返す場所の値
	XMFLOAT3 Pos = { 0,0,0 };

	float dis1x;
	float dis2x;
	float disX;
	dis1x  = pos.x - box.LeastPos.x;
	dis2x = pos.x - box.MaxPos.x;

	if (dis1x < dis2x)
	{
		disX = dis1x;
	}
	else
	{
		disX = dis2x;
	}

	float dis1z;
	float dis2z;
	float disZ;
	dis1z = pos.z - box.LeastPos.z;
	dis2z = pos.z - box.MaxPos.z;

	if (dis1z < dis2z)
	{
		disZ = dis1z;
	}
	else
	{
		disZ = dis2z;
	}

	if (disX < disZ)
	{
		Pos.x = disX;
		Pos.z = pos.z;
	}
	else
	{
		Pos.x = pos.x;
		Pos.z = disZ;
	}


	return Pos;
}

CollisionSet::XMFLOAT3 CollisionSet::GetNearVertex(const Box& box, const XMFLOAT3& pos)
{
	XMFLOAT3 Pos[4];
	Pos[0] = { box.LeastPos.x, 0, box.LeastPos.z };		//左前
	Pos[1] = { box.LeastPos.x, 0, box.MaxPos.z	};		//左奥
	Pos[2] = {	 box.MaxPos.x, 0, box.MaxPos.z	};		//右奥
	Pos[3] = {	 box.MaxPos.x, 0, box.LeastPos.z };		//右前

	float dis[4];
	for (int i = 0; i < 4; i++)
	{
		float x = pos.x - Pos[i].x;
		float z = pos.z - Pos[i].z;

		dis[i] = sqrt(x * x + z * z);
	}

	float min = MyMath::minElement(dis, 4);

	for (int i = 0; i < 4; i++)
	{
		if (dis[i] == min)
		{
			return Pos[i];
		}
	}
}

CollisionSet::XMFLOAT3 CollisionSet::GetScecondNearVertex(const Box& box, const XMFLOAT3& pos)
{
	XMFLOAT3 Pos[4];
	Pos[0] = { box.LeastPos.x, 0, box.LeastPos.z };		//左前
	Pos[1] = { box.LeastPos.x, 0, box.MaxPos.z };		//左奥
	Pos[2] = { box.MaxPos.x, 0, box.MaxPos.z };		//右奥
	Pos[3] = { box.MaxPos.x, 0, box.LeastPos.z };		//右前

	float dis[4];
	for (int i = 0; i < 4; i++)
	{
		float x = pos.x - Pos[i].x;
		float z = pos.z - Pos[i].z;

		dis[i] = sqrt(x * x + z * z);
	}
	float tmp;


	float disA[4];

	for (int i = 0; i < 4; i++)
	{
		disA[i] = dis[i];
	}

	for (int i = 0; i < 4; ++i) {
		for (int j = i + 1; j < 4; ++j) {
			if (disA[i] > disA[j]) {
				tmp = disA[i];
				disA[i] = disA[j];
				disA[j] = tmp;
			}
		}
	}

	for (int i = 0; i < 4; i++)
	{
		if (disA[1] == dis[i])
		{
			return Pos[i];
		}
	}

}

CollisionSet::XMFLOAT3 CollisionSet::GetThirdNearVertex(const Box& box, const XMFLOAT3& pos)
{
	XMFLOAT3 Pos[4];
	Pos[0] = { box.LeastPos.x, 0, box.LeastPos.z };		//左前
	Pos[1] = { box.LeastPos.x, 0, box.MaxPos.z };		//左奥
	Pos[2] = { box.MaxPos.x, 0, box.MaxPos.z };		//右奥
	Pos[3] = { box.MaxPos.x, 0, box.LeastPos.z };		//右前

	float dis[4];
	for (int i = 0; i < 4; i++)
	{
		float x = pos.x - Pos[i].x;
		float z = pos.z - Pos[i].z;

		dis[i] = sqrt(x * x + z * z);
	}
	float tmp;


	float disA[4];

	for (int i = 0; i < 4; i++)
	{
		disA[i] = dis[i];
	}

	for (int i = 0; i < 4; ++i) {
		for (int j = i + 1; j < 4; ++j) {
			if (disA[i] > disA[j]) {
				tmp = disA[i];
				disA[i] = disA[j];
				disA[j] = tmp;
			}
		}
	}

	for (int i = 0; i < 4; i++)
	{
		if (disA[2] == dis[i])
		{
			return Pos[i];
		}
	}
}

CollisionSet::XMFLOAT3 CollisionSet::GetFourthNearVertex(const Box& box, const XMFLOAT3& pos)
{
	XMFLOAT3 Pos[4];
	Pos[0] = { box.LeastPos.x, 0, box.LeastPos.z };		//左前
	Pos[1] = { box.LeastPos.x, 0, box.MaxPos.z };		//左奥
	Pos[2] = { box.MaxPos.x, 0, box.MaxPos.z };		//右奥
	Pos[3] = { box.MaxPos.x, 0, box.LeastPos.z };		//右前

	float dis[4];
	for (int i = 0; i < 4; i++)
	{
		float x = pos.x - Pos[i].x;
		float z = pos.z - Pos[i].z;

		dis[i] = sqrt(x * x + z * z);
	}
	float tmp;


	float disA[4];

	for (int i = 0; i < 4; i++)
	{
		disA[i] = dis[i];
	}

	for (int i = 0; i < 4; ++i) {
		for (int j = i + 1; j < 4; ++j) {
			if (disA[i] > disA[j]) {
				tmp = disA[i];
				disA[i] = disA[j];
				disA[j] = tmp;
			}
		}
	}

	for (int i = 0; i < 4; i++)
	{
		if (disA[3] == dis[i])
		{
			return Pos[i];
		}
	}
}

CollisionSet::XMFLOAT3 CollisionSet::GetwhichNearPos(const XMFLOAT3& criteriaPos, const XMFLOAT3& posA, const XMFLOAT3& posB)
{
	float disA, disB;
	disA = sqrt((criteriaPos.x * posA.x) + (criteriaPos.z * posA.z));
	disB = sqrt((criteriaPos.x * posB.x) + (criteriaPos.z * posB.z));

	if (disA <= disB)
	{
		return posA;
	}
	else
	{
		return posB;
	}
}

int CollisionSet::SelectNearRoot(const XMFLOAT3& startPos, const XMFLOAT3& endPos, const XMFLOAT3& pos1, const XMFLOAT3& pos2)
{
	float dis1, dis2, dis1_1, dis2_1;
	float x, z;

	x = (pos1.x - startPos.x);
	z = (pos1.z - startPos.z);
	dis1 = sqrt((x * x) + (z * z));

	x = (pos2.x - startPos.x);
	z = (pos2.z - startPos.z);
	dis2 = sqrt((x * x) + (z * z));

	x = (endPos.x - pos1.x);
	z = (endPos.z - pos1.z);
	dis1_1 = sqrt((x * x) + (z * z));

	x = (endPos.x - pos2.x);
	z = (endPos.z - pos2.z);
	dis2_1 = sqrt((x * x) + (z * z));

	dis1 += dis1_1;
	dis2 += dis2_1;

	if (dis1 < dis2)
	{
		return 1;
	}
	else if (dis1 == dis2)
	{
		return 3;
	}
	else
	{
		return 2;
	}

}

