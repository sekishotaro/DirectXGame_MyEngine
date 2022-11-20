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
	HRESULT result;

	//�؂�Ă���WinApp�̃C���X�^���X���L�^
	this->winApp = winApp;

	result = DirectInput8Create(
		winApp->GetHInstance(), DIRECTINPUT_VERSION, IID_IDirectInput8, (void **)&dinput, nullptr);

	//�L�[�{�[�h�f�o�C�X����
	result = dinput->CreateDevice(GUID_SysKeyboard, &devkeyboard, NULL);
	//�}�E�X����
	result = dinput->CreateDevice(GUID_SysMouse, &devMouse, NULL);

	if (devkeyboard != nullptr)
	{
		//���͌`���̃Z�b�g
		result = devkeyboard->SetDataFormat(&c_dfDIKeyboard);
		//�r�����䃌�x���̃Z�b�g
		result = devkeyboard->SetCooperativeLevel(winApp->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	}
	

	if (devMouse != nullptr)
	{
		//���͌`���̃Z�b�g
		result = devMouse->SetDataFormat(&c_dfDIMouse2);
		//�r�����䃌�x���̃Z�b�g
		result = devMouse->SetCooperativeLevel(winApp->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);

	}
}

void Input::Update()
{
	HRESULT result;

	//�L�[�{�[�h
	if (devkeyboard != nullptr)
	{
		//�L�[�{�[�h���̎擾���J�n
		result = devkeyboard->Acquire();

		//�O��̃L�[���͂�ۑ�
		memcpy(keyPre, key, sizeof(key));

		result = devkeyboard->GetDeviceState(sizeof(key), key);
	}

	//�}�E�X
	if (devMouse != nullptr)
	{
		//�}�E�X���̎擾���J�n
		result = devMouse->Acquire();

		//�O��̓��͂�ۑ�
		mousePre = mouse;

		result = devMouse->GetDeviceState(sizeof(mouse), &mouse);
	}

	//�Q�[���p�b�h
		//�Q�[���p�b�h�̐���
	if (XInputGetState(0, &state) != ERROR_SUCCESS)
	{
		//assert(0);
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

bool Input::PushPadbutton(int num)
{
	if (state.Gamepad.wButtons == XINPUT_GAMEPAD_DPAD_UP && num == GAMEPAD_DPAD_UP)
	{
		return true;
	}
	else if (state.Gamepad.wButtons == XINPUT_GAMEPAD_DPAD_DOWN && num == GAMEPAD_DPAD_DOWN)
	{
		return true;
	}
	else if (state.Gamepad.wButtons == XINPUT_GAMEPAD_DPAD_LEFT && num == GAMEPAD_DPAD_LEFT)
	{
		return true;
	}
	else if (state.Gamepad.wButtons == XINPUT_GAMEPAD_DPAD_RIGHT && num == GAMEPAD_DPAD_RIGHT)
	{
		return true;
	}
	else if (state.Gamepad.wButtons == XINPUT_GAMEPAD_START && num == GAMEPAD_START)
	{
		return true;
	}
	else if (state.Gamepad.wButtons == XINPUT_GAMEPAD_BACK && num == GAMEPAD_BACK)
	{
		return true;
	}
	else if (state.Gamepad.wButtons == XINPUT_GAMEPAD_LEFT_THUMB && num == GAMEPAD_LEFT_THUMB)
	{
		return true;
	}
	else if (state.Gamepad.wButtons == XINPUT_GAMEPAD_RIGHT_THUMB && num == GAMEPAD_RIGHT_THUMB)
	{
		return true;
	}
	else if (state.Gamepad.wButtons == XINPUT_GAMEPAD_LEFT_SHOULDER && num == GAMEPAD_LEFT_SHOULDER)
	{
		return true;
	}
	else if (state.Gamepad.wButtons == XINPUT_GAMEPAD_RIGHT_SHOULDER && num == GAMEPAD_RIGHT_SHOULDER)
	{
		return true;
	}
	else if (state.Gamepad.wButtons == XINPUT_GAMEPAD_A && num == Button_A)
	{
		return true;
	}
	else if (state.Gamepad.wButtons == XINPUT_GAMEPAD_B && num == Button_B)
	{
		return true;
	}
	else if (state.Gamepad.wButtons == XINPUT_GAMEPAD_X && num == Button_X)
	{
		return true;
	}
	else if (state.Gamepad.wButtons == XINPUT_GAMEPAD_Y && num == Button_Y)
	{
		return true;
	}
	else if (state.Gamepad.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD && num == GAMEPAD_LEFT_TRIGGER)
	{
		return true;
	}
	else if (state.Gamepad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD && num == GAMEPAD_RIGHT_TRIGGER)
	{
		return true;
	}

	return false;
}

bool Input::LeftStickIn(PadStick stick)
{
	if (state.Gamepad.sThumbLX < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE && stick == LEFT)
	{
		return true;
	}
	else if (state.Gamepad.sThumbLX > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE && stick == RIGHT)
	{
		return true;
	}
	else if (state.Gamepad.sThumbLY < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE && stick == DOWN)
	{
		return true;
	}
	else if (state.Gamepad.sThumbLY > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE && stick == UP)
	{
		return true;
	}

	return false;
}

int Input::LeftStickInXNum()
{
	int result = 0;
	if (state.Gamepad.sThumbLX < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
	{
		result = (int)(state.Gamepad.sThumbLX + XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
	}
	else if (state.Gamepad.sThumbLX > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
	{
		result = (int)(state.Gamepad.sThumbLX - XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
	}
	return result;
}

int Input::LeftStickInYNum()
{
	int result = 0;
	if (state.Gamepad.sThumbLY < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
	{
		result = (int)(state.Gamepad.sThumbLY + XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
	}
	else if (state.Gamepad.sThumbLY > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
	{
		result = (int)(state.Gamepad.sThumbLY - XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
	}
	return result;
}

bool Input::RightStickIn(PadStick stick)
{
	if (state.Gamepad.sThumbRX < -(XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE + 200) && stick == LEFT)
	{
		return true;
	}
	else if (state.Gamepad.sThumbRX > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE + 200 && stick == RIGHT)
	{
		return true;
	}
	else if (state.Gamepad.sThumbRY < -(XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE + 1500) && stick == DOWN)
	{
		return true;
	}
	else if (state.Gamepad.sThumbRY > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE + 1500 && stick == UP)
	{
		return true;
	}

	return false;
}

