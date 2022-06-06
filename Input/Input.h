#include <windows.h>
#include <wrl.h>
#define DIRECTINPUT_VERSION  0x0800   //DirectInput�̃o�[�W�����w��
#include <dinput.h>
#include "WinApp.h"
#include <DirectXMath.h>
#pragma once

enum MouseButton
{
	Mouse_Left,			//��
	Mouse_Right,		//�E
};

//����
class Input
{
public:
	//namespace�ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
private: // �G�C���A�X
// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;

public: //�����o�֐�

	static Input *GetInstance();

	//������
	void Initialize(WinApp *winApp);

	//�X�V
	void Update();

	///<summary>
	/// �L�[�̉������`�F�b�N
	///</summary>
	/// <param name="keyNumber">�L�[�ԍ�( DIK_0 ��)</param>
	/// <returns>������Ă��邩</returns>
	bool PushKey(BYTE keyNumber);

	///<summary>
	/// �L�[�̃g���K�[���`�F�b�N
	///</summary>
	/// <param name="keyNumber">�L�[�ԍ�( DIK_0 ��)</param>
	/// <returns>������Ă��邩</returns>
	bool TriggerKey(BYTE keyNumber);

	/// <summary>
	/// �}�E�X���N���b�N�̉������`�F�b�N
	/// </summary>
	/// <param name="mouse">�}�E�X�X�e�[�^�X</param>
	/// <returns>������Ă��邩�ǂ���</returns>
	bool PushLeftMouseButton();

	/// <summary>
	/// �}�E�X�E�N���b�N�̉������`�F�b�N
	/// </summary>
	/// <param name="mouse">�}�E�X�X�e�[�^�X</param>
	/// <returns>������Ă��邩�ǂ���</returns>
	bool PushRightMouseButton();


	/// <summary>
	/// �}�E�X�̃{�^����������Ă��邩�`�F�b�N
	/// </summary>
	/// <param name="mouse_button">�}�E�X</param>
	/// <returns>������Ă��邩�ǂ���</returns>
	bool PushMouseButton(MouseButton mouse_button);

	void MousePosLoad();

	/// <summary>
	/// �}�E�X�̈ʒu
	/// </summary>
	/// <returns></returns>
	static XMFLOAT2 GetMousePos() { return mousePos; }

private: //�����o�ϐ�
	//�L�[�{�[�h�̃f�o�C�X
	ComPtr<IDirectInputDevice8> devkeyboard;
	//�}�E�X�̃f�o�C�X
	ComPtr<IDirectInputDevice8> devMouse;

	//DirectInput�̃C���X�^���X����
	ComPtr<IDirectInput8> dinput;

	//�S�L�[�̏��
	BYTE key[256] = {};
	DIMOUSESTATE2 mouse;

	//�O��̑S�L�[�̏��
	BYTE keyPre[256] = {};
	DIMOUSESTATE2 mousePre;

	//WindowsAPI
	WinApp *winApp = nullptr;

	POINT p;
	static XMFLOAT2 mousePos;
	
};