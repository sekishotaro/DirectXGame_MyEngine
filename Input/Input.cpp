#include "Input.h"
#include <cassert>

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

Input::Input()
{
}

Input::~Input()
{
}

Input *Input::GetInstance()
{
	static Input instance;

	return &instance;
}

void Input::Initialize(WinApp *winApp)
{
	//�؂�Ă���WinApp�̃C���X�^���X���L�^
	this->winApp = winApp;

	HRESULT result;

	result = DirectInput8Create(
		winApp->GetHInstance(), DIRECTINPUT_VERSION, IID_IDirectInput8, (void **)&dinput, nullptr);

	//�L�[�{�[�h�f�o�C�X����
	result = dinput->CreateDevice(GUID_SysKeyboard, &devkeyboard, NULL);
	//���͌`���̃Z�b�g
	result = devkeyboard->SetDataFormat(&c_dfDIKeyboard);
	//�r�����䃌�x���̃Z�b�g
	result = devkeyboard->SetCooperativeLevel(
		winApp->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);

	//�}�E�X����
	result = dinput->CreateDevice(GUID_SysMouse, &devMouse, NULL);
	//���͌`���̃Z�b�g
	result = devMouse->SetDataFormat(&c_dfDIMouse2);
	//�r�����䃌�x���̃Z�b�g
	result = devMouse->SetCooperativeLevel(
		winApp->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
}

void Input::Update()
{
	HRESULT result;

	//�L�[�{�[�h
	{
		//�L�[�{�[�h���̎擾���J�n
		result = devkeyboard->Acquire();

		//�O��̃L�[���͂�ۑ�
		memcpy(keyPre, key, sizeof(key));

		result = devkeyboard->GetDeviceState(sizeof(key), key);
	}

	//�}�E�X
	{
		//�}�E�X���̎擾���J�n
		result = devMouse->Acquire();

		//�O��̓��͂�ۑ�
		mousePre = mouse;

		result = devMouse->GetDeviceState(sizeof(mouse), &mouse);
	}
	
}

bool Input::PushKey(BYTE keyNumber)
{
	// �ُ�Ȉ��������o
	assert(0 <= keyNumber && keyNumber <= 256);

	//�w��L�[�������Ă����true��Ԃ�
	if (key[keyNumber])
	{
		return true;
	}

	//�����łȂ����false��Ԃ�
	return false;
}

bool Input::TriggerKey(BYTE keyNumber)
{
	// �ُ�Ȉ��������o
	assert(0 <= keyNumber && keyNumber <= 256);

	// �O��0�ŁA����0�łȂ���΃g���K�[
	if (!keyPre[keyNumber] && key[keyNumber]) {
		return true;
	}

	return false;
}

bool Input::PushLeftMouseButton()
{
	if (mouse.rgbButtons[0])
	{
		return true;
	}

	return false;
}


bool Input::PushMouseButton(MouseButton mouse_button)
{
	if (mouse.rgbButtons[mouse_button])
	{
		return true;
	}

	return false;
}

Input::MousePos Input::MousePosLoad()
{
	MousePos mpos;
	mpos.lX = mouse.lX;
	mpos.lY = mouse.lY;
	mpos.lZ = mouse.lZ;
	return mpos;
}

