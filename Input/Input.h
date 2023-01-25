#pragma once
#include "WinApp.h"
#include <windows.h>
#include <dinput.h>
#include <wrl.h>
#include <DirectXMath.h>
#include <Xinput.h>

#pragma comment (lib, "xinput.lib")

//#define DIRECTINPUT_VERSION  0x0800   //DirectInput�̃o�[�W�����w��

enum MouseButton
{
	Mouse_Left,			//��
	Mouse_Right,		//�E
	Mouse_Middle		//�^��
};

enum PadButton
{
	GAMEPAD_DPAD_UP,
	GAMEPAD_DPAD_DOWN,
	GAMEPAD_DPAD_LEFT,
	GAMEPAD_DPAD_RIGHT,
	GAMEPAD_START,
	GAMEPAD_BACK,
	GAMEPAD_LEFT_THUMB,
	GAMEPAD_RIGHT_THUMB,
	GAMEPAD_LEFT_SHOULDER,
	GAMEPAD_RIGHT_SHOULDER,
	Button_A,
	Button_B,
	Button_X,
	Button_Y,
	GAMEPAD_LEFT_TRIGGER,
	GAMEPAD_RIGHT_TRIGGER,
};

enum PadStick
{
	UP,
	RIGHT,
	DOWN,
	LEFT,
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


	//�p�b�h�̃{�^������
	bool PushPadbutton(int num);

	//�p�b�h�̃g���K�[����
	bool TriggerPadbutton(int num);

	//�p�b�h�̍��X�e�B�b�N����
	bool LeftStickIn(PadStick stick);
	//�p�b�h�̍��X�e�B�b�N�̓��͒l;
	int LeftStickInXNum();
	int LeftStickInYNum();
	//�p�b�h�̉E�X�e�B�b�N����
	bool RightStickIn(PadStick stick);

	//�R�s�[�R���X�g���N�^�̋֎~
	Input(const Input& input) = delete;
	//������Z�q�̋֎~
	Input& operator = (const Input& input) = delete;

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

	//�R���g���[���[
	XINPUT_STATE state;
	XINPUT_STATE parState;
	float stickDistance = 50.0f;
	#define XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE  7849
	#define XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE 8689

	//WindowsAPI
	WinApp *winApp = nullptr;
	POINT p;
	static XMFLOAT2 mousePos;
	
};