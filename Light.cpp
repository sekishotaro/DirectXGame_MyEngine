#include "Light.h"

using namespace DirectX;

//XMFLOAT3 Light::pos = { 0.0f, 50.0f, -30.0f };
//XMFLOAT3 Light::target = { 0.0f, 0.0f, 0.0f };
float Light::window_width =  1280.0f;
float Light::window_height =  720.0f;

Light::Light(const int& window_width, const int& window_height)
{
	aspectRatio = (float)window_width / window_height;

	this->window_width = window_width;
	this->window_height = window_height;

	//�r���[�s��̌v�Z
	UpdateViewMatrix();

	// �ˉe�s��̌v�Z
	UpdateProjectionMatrix();

	// �r���[�v���W�F�N�V�����̍���
	matViewProjection = matView * matProjection;
}

void Light::Update()
{
	if (viewDirty || projectionDirty) {
		// �Čv�Z�K�v�Ȃ�
		if (viewDirty) {
			// �r���[�s��X�V
			UpdateViewMatrix();
			viewDirty = false;
		}

		// �Čv�Z�K�v�Ȃ�
		if (projectionDirty) {
			// �r���[�s��X�V
			UpdateProjectionMatrix();
			projectionDirty = false;
		}
		// �r���[�v���W�F�N�V�����̍���
		matViewProjection = matView * matProjection;
	}
}

void Light::UpdateViewMatrix()
{
	//���C�g�̍��W
	XMVECTOR lightPosition = XMLoadFloat3(&pos);
	//���C�g�̌������W
	XMVECTOR lightTargetPosition = XMLoadFloat3(&target);
	//�����
	XMVECTOR upVector = XMLoadFloat3(&up);

	//���C�g���J�����ɂ������̂̎���
	XMVECTOR lightAxisZ = XMVectorSubtract(lightTargetPosition, lightPosition);
	// 0�x�N�g�����ƌ�������܂�Ȃ��̂ŏ��O
	assert(!XMVector3Equal(lightAxisZ, XMVectorZero()));
	assert(!XMVector3IsInfinite(lightAxisZ));
	assert(!XMVector3Equal(upVector, XMVectorZero()));
	assert(!XMVector3IsInfinite(upVector));
	//�x�N�g���𐳋K��
	lightAxisZ = XMVector3Normalize(lightAxisZ);
	// ���C�g��X���i�E�����j
	XMVECTOR lightAxisX;
	// X���͏������Z���̊O�ςŋ��܂�
	lightAxisX = XMVector3Cross(upVector, lightAxisZ);
	// �x�N�g���𐳋K��
	lightAxisX = XMVector3Normalize(lightAxisX);

	// ���C�g��Y���i������j
	XMVECTOR lightAxisY;
	// Y����Z����X���̊O�ςŋ��܂�
	lightAxisY = XMVector3Cross(lightAxisZ, lightAxisX);
	
	// �����܂łŒ�������3�����̃x�N�g��������
	//�i���[���h���W�n�ł̃��C�g�̉E�����A������A�O�����j	

	// ���C�g��]�s��
	XMMATRIX matCameraRot;
	// ���C�g���W�n�����[���h���W�n�̕ϊ��s��
	matCameraRot.r[0] = lightAxisX;
	matCameraRot.r[1] = lightAxisY;
	matCameraRot.r[2] = lightAxisZ;
	matCameraRot.r[3] = XMVectorSet(0, 0, 0, 1);
	// �]�u�ɂ��t�s��i�t��]�j���v�Z
	matView = XMMatrixTranspose(matCameraRot);

	// ���_���W��-1���|�������W
	XMVECTOR reverseEyePosition = XMVectorNegate(lightPosition);
	// ���C�g�̈ʒu���烏�[���h���_�ւ̃x�N�g���i�J�������W�n�j
	XMVECTOR tX = XMVector3Dot(lightAxisX, reverseEyePosition);	// X����
	XMVECTOR tY = XMVector3Dot(lightAxisY, reverseEyePosition);	// Y����
	XMVECTOR tZ = XMVector3Dot(lightAxisZ, reverseEyePosition);	// Z����
	// ��̃x�N�g���ɂ܂Ƃ߂�
	XMVECTOR translation = XMVectorSet(tX.m128_f32[0], tY.m128_f32[1], tZ.m128_f32[2], 1.0f);
	// �r���[�s��ɕ��s�ړ�������ݒ�
	matView.r[3] = translation;

#pragma region �S�����r���{�[�h�s��̌v�Z
	// �r���{�[�h�s��
	matBillboard.r[0] = lightAxisX;
	matBillboard.r[1] = lightAxisY;
	matBillboard.r[2] = lightAxisZ;
	matBillboard.r[3] = XMVectorSet(0, 0, 0, 1);
#pragma region

#pragma region Y�����r���{�[�h�s��̌v�Z
	// ���C�gX���AY���AZ��
	XMVECTOR ybillLightAxisX, ybillLightAxisY, ybillLightAxisZ;

	// X���͋���
	ybillLightAxisX = lightAxisX;
	// Y���̓��[���h���W�n��Y��
	ybillLightAxisY = XMVector3Normalize(upVector);
	// Z����X����Y���̊O�ςŋ��܂�
	ybillLightAxisZ = XMVector3Cross(ybillLightAxisX, ybillLightAxisY);

	// Y�����r���{�[�h�s��
	matBillboardY.r[0] = ybillLightAxisX;
	matBillboardY.r[1] = ybillLightAxisY;
	matBillboardY.r[2] = ybillLightAxisZ;
	matBillboardY.r[3] = XMVectorSet(0, 0, 0, 1);
#pragma endregion
}

void Light::UpdateProjectionMatrix()
{
	// �ˉe�s��v�Z
	matProjection = XMMatrixOrthographicLH(
		window_width,
		window_height,
		0.1f, 1000.0f
	);
}
