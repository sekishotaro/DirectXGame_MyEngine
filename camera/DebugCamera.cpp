#include "DebugCamera.h"
#include "Input.h"

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

	// マウスの中ボタンが押されていたらカメラを並行移動させる
	if (Input::GetInstance()->PushMouseButton(Mouse_Middle))
	{
		dx = mpos.lX / 2.0f;
		dy = mpos.lY / 2.0f;
		
		eye = Camera::GetEye();
		eye.x += dx;
		eye.y += dy;

		target = Camera::GetTarget();
		target.x += dx;
		target.y += dy;
	}

	//カメラZ軸移動
	{
		eye = Camera::GetEye();
		dz = mpos.lZ / 10.0f;
		eye.z += dz;
	}

	//カメラの回転
	if (Input::GetInstance()->PushMouseButton(Mouse_Left))
	{
		float angleX = mpos.lX / 2.0f;
		float angleY = mpos.lY / 2.0f;

		//注視点座標
		XMFLOAT3 targetpos = Camera::GetTarget();
		//カメラ座標
		XMFLOAT3 eyepos = Camera::GetEye();
		//カメラと注視点の距離
		XMVECTOR distance = { targetpos.x - eyepos.x, targetpos.y - eyepos.y, targetpos.z - eyepos.z };


	}

	Camera::SetTarget(target);
	Camera::SetEye(eye);
	Camera::Update();
}
