#include "InterpolationCamera.h"
#include "Player.h"

float InterpolationCamera::rotaX = 270.0f;
float InterpolationCamera::rotaY = 70.0f;
float InterpolationCamera::dis = 10.0f;

void InterpolationCamera::StartInterpolationCamera(DebugCamera* camera)
{
	//Input::MousePos mpos = Input::GetInstance()->MousePosLoad();

	XMFLOAT3 cameraPos = Player::GetPos();
	//cameraPos.y += 10.0f;

	////ƒJƒƒ‰‚ÌˆÚ“®
	//if (Input::GetInstance()->PushKey(DIK_UP)) { distance.y += 1.0f; }
	//else if (Input::GetInstance()->PushKey(DIK_DOWN)) { distance.y -= 1.0f; }
	//if (Input::GetInstance()->PushKey(DIK_RIGHT)) { distance.x += 1.0f; }
	//else if (Input::GetInstance()->PushKey(DIK_LEFT)) { distance.x -= 1.0f; }
	//if (Input::GetInstance()->PushKey(DIK_E)) { distance.z += 1.0f; }
	//else if (Input::GetInstance()->PushKey(DIK_Z)) { distance.z -= 1.0f; }
	//if (Input::GetInstance()->RightStickIn(UP)) { distance.y += 1.0f; }
	//else if (Input::GetInstance()->RightStickIn(DOWN)) { distance.y -= 1.0f; }
	//if (Input::GetInstance()->RightStickIn(RIGHT)) { distance.x += 1.0f; }
	//else if (Input::GetInstance()->RightStickIn(LEFT)) { distance.x -= 1.0f; }
	//if (Input::GetInstance()->PushPadbutton(GAMEPAD_LEFT_TRIGGER)) { distance.z += 1.0f; }
	//else if (Input::GetInstance()->PushPadbutton(GAMEPAD_RIGHT_TRIGGER)) { distance.z -= 1.0f; }

	//cameraPos.x += distance.x;
	//cameraPos.y += distance.y;
	//cameraPos.z += distance.z;

	//if (Input::GetInstance()->PushKey(DIK_UP)) { rotaY -= 1.0f; }
	//else if (Input::GetInstance()->PushKey(DIK_DOWN)) { rotaY += 1.0f; }
	//if (Input::GetInstance()->PushKey(DIK_RIGHT)) { rotaX += 1.0f; }
	//else if (Input::GetInstance()->PushKey(DIK_LEFT)) { rotaX -= 1.0f; }
	//if (Input::GetInstance()->PushKey(DIK_E) && dis >= 5.0f) { dis -= 1.0f; }
	//else if (Input::GetInstance()->PushKey(DIK_Z) && dis <= 20.0f) { dis += 1.0f; }

	//if (Input::GetInstance()->RightStickIn(UP) && rotaY > 5) { rotaY -= 1.0f; }
	//else if (Input::GetInstance()->RightStickIn(DOWN) && rotaY < 175) { rotaY += 1.0f; }
	//if (Input::GetInstance()->RightStickIn(RIGHT)) { rotaX += 1.0f; }
	//else if (Input::GetInstance()->RightStickIn(LEFT)) { rotaX -= 1.0f; }
	//if (Input::GetInstance()->PushPadbutton(GAMEPAD_LEFT_TRIGGER) && dis >= 5.0f) { dis -= 1.0f; }
	//else if (Input::GetInstance()->PushPadbutton(GAMEPAD_RIGHT_TRIGGER) && dis <= 20.0f) { dis += 1.0f; }
	//if (Input::GetInstance()->PushPadbutton(GAMEPAD_RIGHT_SHOULDER)) { rotaX = Player::GEtTestFloatNum(); }




	rotaX -= 0.6f;
	rotaY += 0.01f;

	float radiusX = rotaX * 3.14f / 180.0f;
	float radiusY = rotaY * 3.14f / 180.0f;

	//‹…–ÊÀ•WŒn
	cameraPos.y += dis * cos(radiusY);
	cameraPos.x += dis * sin(radiusY) * cos(radiusX);
	cameraPos.z += dis * sin(radiusY) * sin(radiusX);

	camera->SetEye(cameraPos);

	XMFLOAT3 targetPos = Player::GetPos();
	targetPos.y += 6.0f;

	camera->SetTarget(targetPos);
	camera->UpdateOnly();


}

void InterpolationCamera::EndInterpolationCamera(DebugCamera* camera)
{
	XMFLOAT3 cameraPos = Player::GetPos();

	static float rotaX = 270.0f;
	static float rotaY = 70.0f;
	static float dis = 10.0f;

	dis += 0.05f;
	rotaY += 0.01f;

	float radiusX = rotaX * 3.14f / 180.0f;
	float radiusY = rotaY * 3.14f / 180.0f;

	//‹…–ÊÀ•WŒn
	cameraPos.y += dis * cos(radiusY);
	cameraPos.x += dis * sin(radiusY) * cos(radiusX);
	cameraPos.z += dis * sin(radiusY) * sin(radiusX);

	camera->SetEye(cameraPos);

	XMFLOAT3 targetPos = Player::GetPos();
	targetPos.y += 6.0f;

	camera->SetTarget(targetPos);
	camera->UpdateOnly();
}
