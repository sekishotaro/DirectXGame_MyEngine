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
	Button_A,
	Button_B,
	Button_X,
	Button_Y,
	Button_LB,
	Button_RB,
	Button_BACK,
	Button_START,
	Button_L_STICK,
	Button_R_STICK,
	Button_LT,				//�����Ȃ�
	Button_RT,				//�����Ȃ�
	Button_XBOX,			//�����Ȃ�
};

enum GamePadCrossButton
{
	Button_UP,
	Button_RIGHT,
	Button_DOWN,
	Button_LEFT,
};

enum GamePadLeftStick
{
	L_UP,
	L_RIGHT,
	L_DOWN,
	L_LEFT,
};

enum GamePadRightStick
{
	R_UP,
	R_RIGHT,
	R_DOWN,
	R_LEFT,
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

	//�Q�[���p�b�h�{�^���������s��
	bool TriggerGamePadButton(GamePadButton gamePadButton);

	//�Q�[���p�b�h�\���{�^��
	bool PushGamePadCrossButton(GamePadCrossButton gamePadCrossButton);

	//�Q�[���p�b�h�\���{�^���������i�V
	bool TriggerGamePadCrossButton(GamePadCrossButton gamePadCrossButton);

	//�Q�[���p�b�h���X�e�B�b�N
	bool LeftStick(GamePadLeftStick stick);

	XMFLOAT2 LeftStickMove();

	//�Q�[���p�b�h�E�X�e�B�b�N
	bool RightStick(GamePadRightStick stick);

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
	//�{�^���f�[�^
	bool is_push[4] = {};
	//�X�e�B�b�N�̔����͈�
	LONG responsive_range = 100;
	//�X�e�B�b�N�̖������͈�
	LONG unresponsive_range = 20;

	//WindowsAPI
	WinApp *winApp = nullptr;
	POINT p;
	static XMFLOAT2 mousePos;
	
};