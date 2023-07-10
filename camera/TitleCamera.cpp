#include "TitleCamera.h"

TitleCamera::TitleCamera(int window_width, int window_height) : Camera(window_width, window_height)
{

}

void TitleCamera::Update(const XMFLOAT3& targetPos)
{
	XMFLOAT3 cameraPos = {};
	rota.x = AngleNormalize(rota.x);
	rota.x += 1.0f;
	cameraPos = SphereCoordinateSystem(targetPos);
	SetEye(cameraPos);
	//常時自機にターゲット
	XMFLOAT3 pos = targetPos;
	pos.y += 6.0f;
	Camera::SetTarget(pos);
	Camera::Update();
}

Camera::XMFLOAT3 TitleCamera::SphereCoordinateSystem(const XMFLOAT3& targetPos)
{
	XMFLOAT2 radius = { rota.x * 3.14f / 180.0f, rota.y * 3.14f / 180.0f };
	XMFLOAT3 cameraPos = {};

	cameraPos = targetPos;

	//球面座標系
	cameraPos.y += dis * (float)cos(radius.y);
	cameraPos.x += dis * (float)sin(radius.y) * (float)cos(radius.x);
	cameraPos.z += dis * (float)sin(radius.y) * (float)sin(radius.x);

	return cameraPos;
}

float TitleCamera::AngleNormalize(const float rot)
{
	float Rot = rot;
	if (rot >= 360.0f)
	{
		Rot -= 360.0f;
	}
	if (rot <= 0.0f)
	{
		Rot += 360.0f;
	}

	return Rot;
}
