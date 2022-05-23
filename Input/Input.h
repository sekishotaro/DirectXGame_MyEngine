#include <windows.h>
#include <wrl.h>
#define DIRECTINPUT_VERSION  0x0800   //DirectInput�̃o�[�W�����w��
#include <dinput.h>
#include "WinApp.h"
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

private: //�����o�ϐ�
	//�L�[�{�[�h�̃f�o�C�X
	ComPtr<IDirectInputDevice8> devkeyboard;
	//�}�E�X�̃f�o�C�X
	ComPtr<IDirectInputDevice8> devMouse;

	//DirectInput�̃C���X�^���X����
	ComPtr<IDirectInput8> dinput;

	//�S�L�[�̏��
	BYTE key[256] = {};
	//�O��̑S�L�[�̏��
	BYTE keyPre[256] = {};
	DIMOUSESTATE2 mousePre;
	DIMOUSESTATE2 mouse;


	//WindowsAPI
	WinApp *winApp = nullptr;
	
};