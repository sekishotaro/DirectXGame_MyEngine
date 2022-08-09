#include "CollisionSet.h"
#include "JsonLoader.h"
#include "Player.h"
#include "MyMath.h"

bool CollisionSet::PlayerWallCollisionFlag = false;

void CollisionSet::CollisionCheck(const XMFLOAT3& pos, const XMFLOAT3& size, float& groundY)
{
	Box colPlayer;
	colPlayer.centerPos = pos;
	colPlayer.size = { size.x / 2, size.y, size.z / 2 };
	colPlayer.MaxPos = { colPlayer.centerPos.x + colPlayer.size.x,colPlayer.centerPos.y + colPlayer.size.y,colPlayer.centerPos.z + colPlayer.size.z };
	colPlayer.LeastPos = { colPlayer.centerPos.x - colPlayer.size.x,colPlayer.centerPos.y,colPlayer.centerPos.z - colPlayer.size.z };

	Box colBox[4];
	for (int i = 0; i < JsonLoader::colliderObjects.size(); i++)
	{
		colBox[i].centerPos = JsonLoader::colliderObjects[i].get()->GetPosition();
		colBox[i].size = {JsonLoader::colliderObjects[i].get()->GetScale().x / 2, JsonLoader::colliderObjects[i].get()->GetScale().y / 2, JsonLoader::colliderObjects[i].get()->GetScale().z / 2};
		colBox[i].MaxPos = {colBox[i].centerPos.x + colBox[i].size.x,colBox[i].centerPos.y + colBox[i].size.y,colBox[i].centerPos.z + colBox[i].size.z};
		colBox[i].LeastPos = { colBox[i].centerPos.x - colBox[i].size.x,colBox[i].centerPos.y - colBox[i].size.y,colBox[i].centerPos.z - colBox[i].size.z };
	}
	
	PlayerWallCollisionFlag = false;
	for (int i = 0; i < JsonLoader::colliderObjects.size(); i++)
	{
		if (Collision::CheckAABB(colPlayer, colBox[i]) == true)
		{
			PlayerWallCollisionFlag = true;
			JsonLoader::colliderObjects[i].get()->SetCollFlag(true);
		}
		else
		{
			JsonLoader::colliderObjects[i].get()->SetCollFlag(false);
		}
	}
	Player::SetWallColl(PlayerWallCollisionFlag);
}

void CollisionSet::CollisionPushBack( const XMFLOAT3& size, float& groundY)
{
	groundY = 0.0f;
	for (int i = 0; i < JsonLoader::colliderObjects.size(); i++)
	{
		if (JsonLoader::colliderObjects[i]->GetCollFlag() == false)
		{
			continue;
		}

		Box colPlayer;
		colPlayer.centerPos = Player::GetPos();
		colPlayer.size = { size.x / 2, size.y, size.z / 2 };
		colPlayer.MaxPos = { colPlayer.centerPos.x + colPlayer.size.x,colPlayer.centerPos.y + colPlayer.size.y,colPlayer.centerPos.z + colPlayer.size.z };
		colPlayer.LeastPos = { colPlayer.centerPos.x - colPlayer.size.x,colPlayer.centerPos.y,colPlayer.centerPos.z - colPlayer.size.z };

		Box colBox;
		colBox.centerPos = JsonLoader::colliderObjects[i].get()->GetPosition();
		colBox.size = { JsonLoader::colliderObjects[i].get()->GetScale().x / 2, JsonLoader::colliderObjects[i].get()->GetScale().y / 2, JsonLoader::colliderObjects[i].get()->GetScale().z / 2 };
		colBox.MaxPos = { colBox.centerPos.x + colBox.size.x,colBox.centerPos.y + colBox.size.y,colBox.centerPos.z + colBox.size.z };
		colBox.LeastPos = { colBox.centerPos.x - colBox.size.x,colBox.centerPos.y,colBox.centerPos.z - colBox.size.z };

		XMFLOAT3 m1 = {0,0,0};
		
		//X
		if (colPlayer.MaxPos.x >= colBox.MaxPos.x)
		{
			m1.x = colBox.MaxPos.x - colPlayer.LeastPos.x;
		}
		else
		{
			m1.x = colPlayer.MaxPos.x - colBox.LeastPos.x;
			m1.x *= -1;
		}

		//Y
		if (colPlayer.MaxPos.y >= colBox.MaxPos.y)
		{
			m1.y = colBox.MaxPos.y - colPlayer.LeastPos.y;
			groundY = colBox.MaxPos.y;
		}
		else
		{
			m1.y = colPlayer.MaxPos.y - colBox.LeastPos.y;
			m1.y *= -1;
		}

		//Z
		if (colPlayer.MaxPos.z >= colBox.MaxPos.z)
		{
			m1.z = colBox.MaxPos.z - colPlayer.LeastPos.z;
		}
		else
		{
			m1.z = colPlayer.MaxPos.z - colBox.LeastPos.z;
			m1.z *= -1;
		}


		XMFLOAT3 pPos = Player::GetPos();

		Player::MoveAdd(m1);
	}
}
