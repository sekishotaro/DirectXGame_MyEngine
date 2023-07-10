#pragma once
#include "Camera.h"

class TitleCamera : public Camera
{
public: // ÉÅÉìÉoä÷êî
	TitleCamera(int window_width, int window_height);

	virtual ~TitleCamera() = default;

	void Update(const XMFLOAT3 &targetPos);

	XMFLOAT3 SphereCoordinateSystem(const XMFLOAT3& targetPos);

	float AngleNormalize(const float rot);
private:
	float dis = 20.0f;
	XMFLOAT2 rota = { 180.0f, 70.0f };
};

