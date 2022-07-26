#include "Player.h"
#include "MyMath.h"

Player::XMFLOAT3 Player::pos = { 0.0f, 0.0f, 0.0f };
Player::XMFLOAT3 Player::rot = { 0.0f, 0.0f, 0.0f };
Player::XMFLOAT3 Player::size = { 1.0f, 1.0f, 1.0f };

Player::XMFLOAT3 Player::Move(Input *input, bool& flag)
{
	if (input->PushKey(DIK_W) || input->PushKey(DIK_S) || input->PushKey(DIK_D) || input->PushKey(DIK_A))
	{
		// ˆÚ“®Œã‚ÌÀ•W‚ðŒvŽZ
		if (input->PushKey(DIK_W)) { pos.z += 1.0f; }
		else if (input->PushKey(DIK_S)) { pos.z -= 1.0f; }
		if (input->PushKey(DIK_D)) { pos.x += 1.0f; }
		else if (input->PushKey(DIK_A)) { pos.x -= 1.0f; }
	}


	MyMath::Gravity(pos,flag);

	return pos;
}

