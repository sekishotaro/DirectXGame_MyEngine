#include "CollisionSet.h"
#include "JsonLoader.h"
#include "Player.h"

void CollisionSet::CollisionCheck(const XMFLOAT3& pos, const XMFLOAT3& size, float& groundY)
{
	Box colPlayer;
	colPlayer.centerPos = pos;
	colPlayer.size = { size.x / 2, size.y / 2, size.z / 2 };
	colPlayer.MaxPos = { colPlayer.centerPos.x + colPlayer.size.x,colPlayer.centerPos.y + colPlayer.size.y,colPlayer.centerPos.z + colPlayer.size.z };
	colPlayer.LeastPos = { colPlayer.centerPos.x - colPlayer.size.x,colPlayer.centerPos.y - colPlayer.size.y,colPlayer.centerPos.z - colPlayer.size.z };

	Box colBox[4];
	for (int i = 0; i < 4; i++)
	{
		colBox[i].centerPos = JsonLoader::colliderObjects[i].get()->GetPosition();
		colBox[i].size = {JsonLoader::colliderObjects[i].get()->GetScale().x / 2, JsonLoader::colliderObjects[i].get()->GetScale().y / 2, JsonLoader::colliderObjects[i].get()->GetScale().z / 2};
		colBox[i].MaxPos = {colBox[i].centerPos.x + colBox[i].size.x,colBox[i].centerPos.y + colBox[i].size.y,colBox[i].centerPos.z + colBox[i].size.z};
		colBox[i].LeastPos = { colBox[i].centerPos.x - colBox[i].size.x,colBox[i].centerPos.y - colBox[i].size.y,colBox[i].centerPos.z - colBox[i].size.z };
	}
	
	for (int i = 0; i < 4; i++)
	{
		if (Collision::CheckAABB(colPlayer, colBox[i]) == true)
		{
			JsonLoader::colliderObjects[i].get()->SetColor({ 1,1,0 });
			XMFLOAT3 move = Player::GetMove();
			if (move.y < 0.0f)
			{
				groundY = colBox[i].MaxPos.y;
				Player::SetPosY(colBox[i].MaxPos.y);
			}
			move.x *= -1;
			move.y *= -1;
			move.z *= -1;
			Player::AddPos(move);
		}
		else
		{
			JsonLoader::colliderObjects[i].get()->SetColor({ 1,0,0 });
			groundY = 0.0f;
		}
	}
}
