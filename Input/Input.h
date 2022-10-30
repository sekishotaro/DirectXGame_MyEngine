#pragma once
#include "WinApp.h"
#include <windows.h>
#include <dinput.h>
#include <wrl.h>
#include <DirectXMath.h>

#define DIRECTINPUT_VERSION  0x0800   //DirectInput�̃o�[�W�����w��

enum MouseButton
{
	Mouse_Left,			//��
	Mouse_Right,		//�E
	Mouse_Middle		//�^��
};

enum GamePadButton
{
	Buttun_A,
	Buttun_B,
	Buttun_X,
	Buttun_Y,
	Buttun_LB,
	Buttun_RB,

};

enum GamePadLeftStick
{
	L_UP,
	L_DOWN,
	L_RIGHT,
	L_LEFT,
};



//����
class Input
{
public:
	struct MousePos
	{
		LONG lX;
		LONG lY;
		LONG lZ;
	};

private: // �G�C���A�X
// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;

private:
	Input();

	~Input();

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
	/// �}�E�X�̃{�^����������Ă��邩�`�F�b�N
	/// </summary>
	/// <param name="mouse_button">�}�E�X</param>
	/// <returns>������Ă��邩�ǂ���</returns>
	bool PushMouseButton(MouseButton mouse_button);

	MousePos MousePosLoad();

	/// <summary>
	/// �}�E�X�̈ʒu
	/// </summary>
	/// <returns></returns>
	static XMFLOAT2 GetMousePos() { return mousePos; }

	//�R�s�[�R���X�g���N�^�̋֎~
	Input(const Input& input) = delete;
	//������Z�q�̋֎~
	Input& operator = (const Input& input) = delete;

	//�Q�[���p�b�h�{�^��
	bool PushGamePadButton(GamePadButton gamePadButton);

	//�Q�[���p�b�h���X�e�B�b�N
	bool LeftStick(GamePadLeftStick stick);

private: //�����o�ϐ�
	//DirectInput�̃C���X�^���X����
	ComPtr<IDirectInput8> dinput;

	//�L�[�{�[�h�̃f�o�C�X
	ComPtr<IDirectInputDevice8> devkeyboard;
	//�S�L�[�̏��
	BYTE key[256] = {};
	//�O��̑S�L�[�̏��
	BYTE keyPre[256] = {};

	//�}�E�X�̃f�o�C�X
	ComPtr<IDirectInputDevice8> devMouse;
	//�}�E�X�̔���
	DIMOUSESTATE2 mouse;
	//�O�t���[���̔���
	DIMOUSESTATE2 mousePre;

	//�Q�[���p�b�h�̃f�o�C�X
	ComPtr<IDirectInputDevice8> devGamePad;
	//�Q�[���p�b�h�̔���
	DIJOYSTATE gamePad = {};
	//�O�Q�[���p�b�h�̔���
	DIJOYSTATE gamePadPre = {};

	//WindowsAPI
	WinApp *winApp = nullptr;
	POINT p;
	static XMFLOAT2 mousePos;
	
};