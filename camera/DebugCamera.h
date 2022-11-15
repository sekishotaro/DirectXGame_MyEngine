#pragma once
#include <DirectXMath.h>
#include "Camera.h"

class DebugCamera : public Camera
{
public: // �����o�֐�
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="window_width">��ʕ�</param>
	/// <param name="window_height">��ʍ���</param>
	DebugCamera(int window_width, int window_height);

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~DebugCamera() = default;

	/// <summary>
	/// ���t���[���X�V
	/// </summary>
	virtual void Update();

public:
	static XMFLOAT3 eye;
	static XMFLOAT3 target;
	static float dx;
	static float dy;
	static float dz;
	static XMFLOAT3 distance;
	static float rotaX;
	static float rotaY;
	static float dis;
};

