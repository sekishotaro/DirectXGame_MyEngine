#include "DebugCamera.h"
#include "Input.h"
#include "Player.h"

using namespace DirectX;
float DebugCamera::dx = 0;
float DebugCamera::dy = 0;
float DebugCamera::dz = 0;
XMFLOAT3 DebugCamera::eye;
XMFLOAT3 DebugCamera::target;
XMVECTOR DebugCamera::distance;

DebugCamera::DebugCamera(int window_width, int window_height) : Camera(window_width, window_height)
{

}

void DebugCamera::Update()
{
	Input::MousePos mpos = Input::GetInstance()->MousePosLoad();

	XMFLOAT3 cameraPos = Player::GetPos();
	static XMFLOAT3 pos = { 0, 5.0f, -10.0};
	
	//ƒJƒƒ‰‚ÌˆÚ“®
	if (Input::GetInstance()->PushKey(DIK_UP)) { pos.y += 1.0f; }
	else if (Input::GetInstance()->PushKey(DIK_DOWN)) { pos.y -= 1.0f; }
	if (Input::GetInstance()->PushKey(DIK_RIGHT)) { pos.x += 1.0f; }
	else if (Input::GetInstance()->PushKey(DIK_LEFT)) { pos.x -= 1.0f; }
	if (Input::GetInstance()->PushKey(DIK_E)) { pos.z += 1.0f; }
	else if (Input::GetInstance()->PushKey(DIK_Z)) { pos.z -= 1.0f; }

	if (Input::GetInstance()->RightStickIn(UP)) { pos.y += 1.0f; }
	else if (Input::GetInstance()->RightStickIn(DOWN)) { pos.y -= 1.0f; }
	if (Input::GetInstance()->RightStickIn(RIGHT)) { pos.x += 1.0f; }
	else if (Input::GetInstance()->RightStickIn(LEFT)) { pos.x -= 1.0f; }
	if (Input::GetInstance()->PushPadbutton(GAMEPAD_LEFT_TRIGGER)) { pos.z += 1.0f; }
	else if (Input::GetInstance()->PushPadbutton(GAMEPAD_RIGHT_TRIGGER)) { pos.z -= 1.0f; }

	cameraPos.x += pos.x;
	cameraPos.y += pos.y;
	cameraPos.z += pos.z;

	SetEye(cameraPos);

	//ƒJƒƒ‰Z²ˆÚ“®
	{
		eye = Camera::GetEye();
		dz = mpos.lZ / 10.0f;
		eye.z += dz;
	}

	Camera::SetTarget(Player::GetPos());
	Camera::SetEye(eye);
	Camera::Update();
}
