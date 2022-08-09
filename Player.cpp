#include "Player.h"
#include "MyMath.h"

Player::XMFLOAT3 Player::pos = { 0.0f, 10.0f,-20.0f };
Player::XMFLOAT3 Player::move = { 0.0f, 0.0f, 0.0f };
Player::XMFLOAT3 Player::rot = { 0.0f, 0.0f, 0.0f };
Player::XMFLOAT3 Player::size = { 1.0f, 5.0f, 1.0f };
bool Player::groundFlag = false;
bool Player::wallcollisionFlag = false;

void Player::Move(Input *input, const float& groundY)
{
	move = { 0,0,0 };
	if (input->PushKey(DIK_W) || input->PushKey(DIK_S) || input->PushKey(DIK_D) || input->PushKey(DIK_A))
	{
		// ˆÚ“®Œã‚ÌÀ•W‚ðŒvŽZ
		if (input->PushKey(DIK_W)) { move.z += 1.0f; }
		else if (input->PushKey(DIK_S)) { move.z -= 1.0f; }
		if (input->PushKey(DIK_D)) { move.x += 1.0f; }
		else if (input->PushKey(DIK_A)) { move.x -= 1.0f; }
	}

	if (input->PushKey(DIK_O))
	{
		move.y += 1.0f;
	}
	if (input->PushKey(DIK_L))
	{
		move.y -= 1.0f;
	}

	if (input->PushKey(DIK_SPACE))
	{
		move.y += 10.0f;
	}

	MyMath::Gravity(move, groundFlag);
	MyMath::Movement(pos, move);
}

void Player::MoveAdd(XMFLOAT3 Addpos)
{
	MyMath::Movement(pos, Addpos);
}

