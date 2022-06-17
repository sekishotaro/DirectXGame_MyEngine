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

	// �}�E�X�̒��{�^����������Ă�����J��������s�ړ�������
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

	//�J����Z���ړ�
	{
		eye = Camera::GetEye();
		dz = mpos.lZ / 10.0f;
		eye.z += dz;
	}

	//�J�����̉�]
	if (Input::GetInstance()->PushMouseButton(Mouse_Left))
	{
		float angleX = mpos.lX / 2.0f;
		float angleY = mpos.lY / 2.0f;

		//�����_���W
		XMFLOAT3 targetpos = Camera::GetTarget();
		//�J�������W
		XMFLOAT3 eyepos = Camera::GetEye();
		//�J�����ƒ����_�̋���
		XMVECTOR distance = { targetpos.x - eyepos.x, targetpos.y - eyepos.y, targetpos.z - eyepos.z };


	}

	Camera::SetTarget(target);
	Camera::SetEye(eye);
	Camera::Update();
}
