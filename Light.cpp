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

	//ビュー行列の計算
	UpdateViewMatrix();

	// 射影行列の計算
	UpdateProjectionMatrix();

	// ビュープロジェクションの合成
	matViewProjection = matView * matProjection;
}

void Light::Update()
{
	if (viewDirty || projectionDirty) {
		// 再計算必要なら
		if (viewDirty) {
			// ビュー行列更新
			UpdateViewMatrix();
			viewDirty = false;
		}

		// 再計算必要なら
		if (projectionDirty) {
			// ビュー行列更新
			UpdateProjectionMatrix();
			projectionDirty = false;
		}
		// ビュープロジェクションの合成
		matViewProjection = matView * matProjection;
	}
}

void Light::UpdateViewMatrix()
{
	//ライトの座標
	XMVECTOR lightPosition = XMLoadFloat3(&pos);
	//ライトの向き座標
	XMVECTOR lightTargetPosition = XMLoadFloat3(&target);
	//上方向
	XMVECTOR upVector = XMLoadFloat3(&up);

	//ライトをカメラにした時のの視線
	XMVECTOR lightAxisZ = XMVectorSubtract(lightTargetPosition, lightPosition);
	// 0ベクトルだと向きが定まらないので除外
	assert(!XMVector3Equal(lightAxisZ, XMVectorZero()));
	assert(!XMVector3IsInfinite(lightAxisZ));
	assert(!XMVector3Equal(upVector, XMVectorZero()));
	assert(!XMVector3IsInfinite(upVector));
	//ベクトルを正規化
	lightAxisZ = XMVector3Normalize(lightAxisZ);
	// ライトのX軸（右方向）
	XMVECTOR lightAxisX;
	// X軸は上方向→Z軸の外積で求まる
	lightAxisX = XMVector3Cross(upVector, lightAxisZ);
	// ベクトルを正規化
	lightAxisX = XMVector3Normalize(lightAxisX);

	// ライトのY軸（上方向）
	XMVECTOR lightAxisY;
	// Y軸はZ軸→X軸の外積で求まる
	lightAxisY = XMVector3Cross(lightAxisZ, lightAxisX);
	
	// ここまでで直交した3方向のベクトルが揃う
	//（ワールド座標系でのライトの右方向、上方向、前方向）	

	// ライト回転行列
	XMMATRIX matCameraRot;
	// ライト座標系→ワールド座標系の変換行列
	matCameraRot.r[0] = lightAxisX;
	matCameraRot.r[1] = lightAxisY;
	matCameraRot.r[2] = lightAxisZ;
	matCameraRot.r[3] = XMVectorSet(0, 0, 0, 1);
	// 転置により逆行列（逆回転）を計算
	matView = XMMatrixTranspose(matCameraRot);

	// 視点座標に-1を掛けた座標
	XMVECTOR reverseEyePosition = XMVectorNegate(lightPosition);
	// ライトの位置からワールド原点へのベクトル（カメラ座標系）
	XMVECTOR tX = XMVector3Dot(lightAxisX, reverseEyePosition);	// X成分
	XMVECTOR tY = XMVector3Dot(lightAxisY, reverseEyePosition);	// Y成分
	XMVECTOR tZ = XMVector3Dot(lightAxisZ, reverseEyePosition);	// Z成分
	// 一つのベクトルにまとめる
	XMVECTOR translation = XMVectorSet(tX.m128_f32[0], tY.m128_f32[1], tZ.m128_f32[2], 1.0f);
	// ビュー行列に平行移動成分を設定
	matView.r[3] = translation;

#pragma region 全方向ビルボード行列の計算
	// ビルボード行列
	matBillboard.r[0] = lightAxisX;
	matBillboard.r[1] = lightAxisY;
	matBillboard.r[2] = lightAxisZ;
	matBillboard.r[3] = XMVectorSet(0, 0, 0, 1);
#pragma region

#pragma region Y軸回りビルボード行列の計算
	// ライトX軸、Y軸、Z軸
	XMVECTOR ybillLightAxisX, ybillLightAxisY, ybillLightAxisZ;

	// X軸は共通
	ybillLightAxisX = lightAxisX;
	// Y軸はワールド座標系のY軸
	ybillLightAxisY = XMVector3Normalize(upVector);
	// Z軸はX軸→Y軸の外積で求まる
	ybillLightAxisZ = XMVector3Cross(ybillLightAxisX, ybillLightAxisY);

	// Y軸回りビルボード行列
	matBillboardY.r[0] = ybillLightAxisX;
	matBillboardY.r[1] = ybillLightAxisY;
	matBillboardY.r[2] = ybillLightAxisZ;
	matBillboardY.r[3] = XMVectorSet(0, 0, 0, 1);
#pragma endregion
}

void Light::UpdateProjectionMatrix()
{
	// 射影行列計算
	matProjection = XMMatrixOrthographicLH(
		window_width,
		window_height,
		0.1f, 1000.0f
	);
}
