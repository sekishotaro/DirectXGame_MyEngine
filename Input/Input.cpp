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
	//�Q�[���p�b�h�̐���
	result = dinput->CreateDevice(GUID_Joystick, &devGamePad, NULL);

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
	
	if (devGamePad != nullptr)
	{
		//���͌`���̃Z�b�g
		result = devGamePad->SetDataFormat(&c_dfDIJoystick);

		//�v���p�e�B�ݒ�
		//�����[�h���Βl���[�h�Ƃ��Đݒ�
		DIPROPDWORD diprop;
		ZeroMemory(&diprop, sizeof(diprop));
		diprop.diph.dwSize = sizeof(diprop);
		diprop.diph.dwHeaderSize = sizeof(diprop.diph);
		diprop.diph.dwHow = DIPH_DEVICE;
		diprop.diph.dwObj = 0;
		diprop.dwData = DIPROPAXISMODE_ABS;

		//�����[�h��ύX
		devGamePad->SetProperty(DIPROP_AXISMODE, &diprop.diph);

		//X���̒l�͈̔͐ݒ�
		DIPROPRANGE diprg;
		ZeroMemory(&diprg, sizeof(diprg));
		diprg.diph.dwSize = sizeof(diprg);
		diprg.diph.dwHeaderSize = sizeof(diprg.diph);
		diprg.diph.dwHow = DIPH_BYOFFSET;
		diprg.diph.dwObj = DIJOFS_X;
		diprg.lMin = -responsive_range;
		diprg.lMax = responsive_range;

		//X���̒l�͈̔͐ݒ�
		devGamePad->SetProperty(DIPROP_RANGE, &diprg.diph);

		// Y���̒l�͈̔͐ݒ�
		diprg.diph.dwObj = DIJOFS_Y;
		devGamePad->SetProperty(DIPROP_RANGE, &diprg.diph);

		//�r�����䃌�x���̃Z�b�g
		result = devGamePad->SetCooperativeLevel(winApp->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
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
	if (devGamePad != nullptr)
	{
		//�Q�[���p�b�h���̎擾���J�n
		result = devGamePad->Acquire();

		//�O��̓��͂�ۑ�
		gamePadPre = gamePad;

		result = devGamePad->GetDeviceState(sizeof(DIJOYSTATE), &gamePad);
		//�f�[�^�̃��Z�b�g
		for (int i = 0; i < 4; i++)
		{
			is_push[i] = false;
		}
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

bool Input::PushGamePadButton(GamePadButton gamePadButton)
{
	if (gamePad.rgbButtons[gamePadButton])
	{
		return true;
	}

	return false;
}

bool Input::TriggerGamePadButton(GamePadButton gamePadButton)
{
	if (gamePad.rgbButtons[gamePadButton] == 0x80 && gamePadPre.rgbButtons[gamePadButton] != 0x80)
	{
		return true;
	}

	return false;
}

bool Input::PushGamePadCrossButton(GamePadCrossButton gamePadCrossButton)
{
	if (gamePad.rgdwPOV[0] != 0xFFFFFFFF)
	{
		switch (gamePad.rgdwPOV[0])
		{
		case 0:
			is_push[GamePadCrossButton::Button_UP] = true;
			break;
		case 4500:
			is_push[GamePadCrossButton::Button_UP] = true;
			is_push[GamePadCrossButton::Button_RIGHT] = true;
			break;
		case 9000:
			is_push[GamePadCrossButton::Button_RIGHT] = true;
			break;
		case 13500:
			is_push[GamePadCrossButton::Button_RIGHT] = true;
			is_push[GamePadCrossButton::Button_DOWN] = true;
			break;
		case 18000:
			is_push[GamePadCrossButton::Button_DOWN] = true;
			break;
		case 22500:
			is_push[GamePadCrossButton::Button_DOWN] = true;
			is_push[GamePadCrossButton::Button_LEFT] = true;
			break;
		case 27000:
			is_push[GamePadCrossButton::Button_LEFT] = true;
			break;
		case 31500:
			is_push[GamePadCrossButton::Button_LEFT] = true;
			is_push[GamePadCrossButton::Button_UP] = true;
			break;
		}

		for (int i = Button_UP; i < Button_LEFT + 1; i++)
		{
			if (is_push[i] == true)
			{
				if (is_push[i] == is_push[gamePadCrossButton])
				{
					return true;
				}
			}
		}

	}
	return false;
}

bool Input::TriggerGamePadCrossButton(GamePadCrossButton gamePadCrossButton)
{
	if (gamePad.rgdwPOV[0] != 0xFFFFFFFF && gamePadPre.rgdwPOV[0] == 0xFFFFFFFF)
	{
		switch (gamePad.rgdwPOV[0])
		{
		case 0:
			is_push[GamePadCrossButton::Button_UP] = true;
			break;
		case 4500:
			is_push[GamePadCrossButton::Button_UP] = true;
			is_push[GamePadCrossButton::Button_RIGHT] = true;
			break;
		case 9000:
			is_push[GamePadCrossButton::Button_RIGHT] = true;
			break;
		case 13500:
			is_push[GamePadCrossButton::Button_RIGHT] = true;
			is_push[GamePadCrossButton::Button_DOWN] = true;
			break;
		case 18000:
			is_push[GamePadCrossButton::Button_DOWN] = true;
			break;
		case 22500:
			is_push[GamePadCrossButton::Button_DOWN] = true;
			is_push[GamePadCrossButton::Button_LEFT] = true;
			break;
		case 27000:
			is_push[GamePadCrossButton::Button_LEFT] = true;
			break;
		case 31500:
			is_push[GamePadCrossButton::Button_LEFT] = true;
			is_push[GamePadCrossButton::Button_UP] = true;
			break;
		}

		for (int i = Button_UP; i < Button_LEFT + 1; i++)
		{
			if (is_push[i] == true)
			{
				if (is_push[i] == is_push[gamePadCrossButton])
				{
					return true;
				}
			}
		}

	}
	return false;
}

bool Input::LeftStick(GamePadLeftStick stick)
{
	//�X�e�B�b�N�̕�������
	//�����������ׂ�
	if (gamePad.lX < -unresponsive_range && stick == L_LEFT)
	{
		return true;
	}
	else if (gamePad.lX > unresponsive_range && stick == L_RIGHT)
	{
		return true;
	}

	if (gamePad.lY < -unresponsive_range && stick == L_UP)
	{
		return true;
	}
	else if (gamePad.lY > unresponsive_range && stick == L_DOWN)
	{
		return true;
	}

	return false;
}

DirectX::XMFLOAT2 Input::LeftStickMove()
{
	//�X�e�B�b�N�̕�������
	float y_vec = static_cast<float>(-gamePad.lY / responsive_range);
	float x_vec = static_cast<float>(gamePad.lX / responsive_range);

	//��
	if (gamePad.lX > -unresponsive_range && gamePad.lX < unresponsive_range)
	{
		x_vec = 0.0f;
	}
	//�c
	if (gamePad.lY < unresponsive_range && gamePad.lY > -unresponsive_range)
	{
		y_vec = 0.0f;
	}

	return XMFLOAT2(x_vec, y_vec);
}

bool Input::RightStick(GamePadRightStick stick)
{
	//�X�e�B�b�N�̕�������
	//�����������ׂ�
	if (gamePad.lRx < -(unresponsive_range + 32767) && stick == R_LEFT)
	{
		return true;
	}
	else if (gamePad.lRx > (unresponsive_range + 32767) && stick == R_RIGHT)
	{
		return true;
	}

	if (gamePad.lRy < -(unresponsive_range + 32767) && stick == R_UP)
	{
		return true;
	}
	else if (gamePad.lRy > (unresponsive_range + 32767) && stick == R_DOWN)
	{
		return true;
	}

	return false;
}

