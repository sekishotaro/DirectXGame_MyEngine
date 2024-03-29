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

	//借りてきたWinAppのインスタンスを記録
	this->winApp = winApp;

	result = DirectInput8Create(
		winApp->GetHInstance(), DIRECTINPUT_VERSION, IID_IDirectInput8, (void **)&dinput, nullptr);

	//キーボードデバイス生成
	result = dinput->CreateDevice(GUID_SysKeyboard, &devkeyboard, NULL);
	//マウス生成
	result = dinput->CreateDevice(GUID_SysMouse, &devMouse, NULL);

	if (devkeyboard != nullptr)
	{
		//入力形式のセット
		result = devkeyboard->SetDataFormat(&c_dfDIKeyboard);
		//排他制御レベルのセット
		result = devkeyboard->SetCooperativeLevel(winApp->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	}
	

	if (devMouse != nullptr)
	{
		//入力形式のセット
		result = devMouse->SetDataFormat(&c_dfDIMouse2);
		//排他制御レベルのセット
		result = devMouse->SetCooperativeLevel(winApp->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);

	}
}

void Input::Update()
{
	HRESULT result;

	//キーボード
	if (devkeyboard != nullptr)
	{
		//キーボード情報の取得を開始
		result = devkeyboard->Acquire();

		//前回のキー入力を保存
		memcpy(keyPre, key, sizeof(key));

		result = devkeyboard->GetDeviceState(sizeof(key), key);
	}

	//マウス
	if (devMouse != nullptr)
	{
		//マウス情報の取得を開始
		result = devMouse->Acquire();

		//前回の入力を保存
		mousePre = mouse;

		result = devMouse->GetDeviceState(sizeof(mouse), &mouse);
	}

	//ゲームパッド
	//ゲームパッドの生成
	parState = state;
	if (XInputGetState(0, &state) != ERROR_SUCCESS)
	{
		//assert(0);
	}
}

bool Input::PushKey(BYTE keyNumber)
{
	// 異常な引数を検出
	assert(0 <= keyNumber && keyNumber <= 256);

	//指定キーを押していればtrueを返す
	if (key[keyNumber])
	{
		return true;
	}

	//そうでなければfalseを返す
	return false;
}

bool Input::TriggerKey(BYTE keyNumber)
{
	// 異常な引数を検出
	assert(0 <= keyNumber && keyNumber <= 256);

	// 前回が0で、今回が0でなければトリガー
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
	bool result = false;

	if (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP && num == GAMEPAD_DPAD_UP)
	{
		result = true;
	}
	else if (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN && num == GAMEPAD_DPAD_DOWN)
	{
		result = true;
	}
	else if (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT && num == GAMEPAD_DPAD_LEFT)
	{
		result = true;
	}
	else if (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT && num == GAMEPAD_DPAD_RIGHT)
	{
		result = true;
	}
	else if (state.Gamepad.wButtons & XINPUT_GAMEPAD_START && num == GAMEPAD_START)
	{
		result = true;
	}
	else if (state.Gamepad.wButtons & XINPUT_GAMEPAD_BACK && num == GAMEPAD_BACK)
	{
		result = true;
	}
	else if (state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB && num == GAMEPAD_LEFT_THUMB)
	{
		result = true;
	}
	else if (state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB && num == GAMEPAD_RIGHT_THUMB)
	{
		result = true;
	}
	else if (state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER && num == GAMEPAD_LEFT_SHOULDER)
	{
		result = true;
	}
	else if (state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER && num == GAMEPAD_RIGHT_SHOULDER)
	{
		result = true;
	}
	else if (state.Gamepad.wButtons & XINPUT_GAMEPAD_A && num == Button_A)
	{
		result = true;
	}
	else if (state.Gamepad.wButtons & XINPUT_GAMEPAD_B && num == Button_B)
	{
		result = true;
	}
	else if (state.Gamepad.wButtons & XINPUT_GAMEPAD_X && num == Button_X)
	{
		result = true;
	}
	else if (state.Gamepad.wButtons & XINPUT_GAMEPAD_Y && num == Button_Y)
	{
		result = true;
	}
	else if (state.Gamepad.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD && num == GAMEPAD_LEFT_TRIGGER)
	{
		result = true;
	}
	else if (state.Gamepad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD && num == GAMEPAD_RIGHT_TRIGGER)
	{
		result = true;
	}

	return result;
}

bool Input::TriggerPadbutton(int num)
{
	bool result = false;
	if (parState.Gamepad.wButtons == state.Gamepad.wButtons) return result;
	result = PushPadbutton(num);
	return result;
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

