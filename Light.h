#pragma once

#include <DirectXMath.h>

class Light
{
private: // �G�C���A�X
// Microsoft::WRL::���ȗ�
	//template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	Light(const int& window_width, const int& window_height);

	virtual void Update();

	/// <summary>
	/// �r���[�s����X�V
	/// </summary>
	void UpdateViewMatrix();

	/// <summary>
	/// �ˉe�s����X�V
	/// </summary>
	void UpdateProjectionMatrix();

	/// <summary>
	/// �r���[�s��̎擾
	/// </summary>
	/// <returns>�r���[�s��</returns>
	inline const XMMATRIX& GetViewMatrix() {
		return matView;
	}

	/// <summary>
	/// �ˉe�s��̎擾
	/// </summary>
	/// <returns>�ˉe�s��</returns>
	inline const XMMATRIX& GetProjectionMatrix() {
		return matProjection;
	}

	/// <summary>
	/// �r���[�ˉe�s��̎擾
	/// </summary>
	/// <returns>�r���[�ˉe�s��</returns>
	inline const XMMATRIX& GetViewProjectionMatrix() {
		return matViewProjection;
	}

	/// <summary>
	/// �r���{�[�h�s��̎擾
	/// </summary>
	/// <returns>�r���{�[�h�s��</returns>
	inline const XMMATRIX& GetBillboardMatrix() {
		return matBillboard;
	}

	const XMFLOAT3& GetPosition() { return pos; viewDirty = true;}
	void SetPosition(const XMFLOAT3& pos) { this->pos = pos; }
	const XMFLOAT3& GetTarget() { return target; }
	void SetTarget(const XMFLOAT3& target) { this->target = target; viewDirty = true;}


protected:

	// �r���[�s��
	XMMATRIX matView = DirectX::XMMatrixIdentity();
	// �r���{�[�h�s��
	XMMATRIX matBillboard = DirectX::XMMatrixIdentity();
	// Y�����r���{�[�h�s��
	XMMATRIX matBillboardY = DirectX::XMMatrixIdentity();
	// �ˉe�s��
	XMMATRIX matProjection = DirectX::XMMatrixIdentity();
	// �r���[�ˉe�s��
	XMMATRIX matViewProjection = DirectX::XMMatrixIdentity();
	// �r���[�s��_�[�e�B�t���O
	bool viewDirty = false;
	// �ˉe�s��_�[�e�B�t���O
	bool projectionDirty = false;
	//���C�g�̈ʒu
	XMFLOAT3 pos = { 0.0f, 10.0f, -10.0f };
	//���C�g�̌���  
	XMFLOAT3 target = { 0.0f, 0.0f, 0.0f };
	//������x�N�g��
	const XMFLOAT3 up = { 0,1,0 };
	// �A�X�y�N�g��
	float aspectRatio = 1.0f;

	static float window_height;
	static float window_width;
};

