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
	//ゲームパッドの生成
	result = dinput->CreateDevice(GUID_Joystick, &devGamePad, NULL);

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
	
	if (devGamePad != nullptr)
	{
		//入力形式のセット
		result = devGamePad->SetDataFormat(&c_dfDIJoystick);

		//プロパティ設定
		//軸モードを絶対値モードとして設定
		DIPROPDWORD diprop;
		ZeroMemory(&diprop, sizeof(diprop));
		diprop.diph.dwSize = sizeof(diprop);
		diprop.diph.dwHeaderSize = sizeof(diprop.diph);
		diprop.diph.dwHow = DIPH_DEVICE;
		diprop.diph.dwObj = 0;
		diprop.dwData = DIPROPAXISMODE_ABS;

		//軸モードを変更
		devGamePad->SetProperty(DIPROP_AXISMODE, &diprop.diph);

		//X軸の値の範囲設定
		DIPROPRANGE diprg;
		ZeroMemory(&diprg, sizeof(diprg));
		diprg.diph.dwSize = sizeof(diprg);
		diprg.diph.dwHeaderSize = sizeof(diprg.diph);
		diprg.diph.dwHow = DIPH_BYOFFSET;
		diprg.diph.dwObj = DIJOFS_X;
		diprg.lMin = -1000;
		diprg.lMax = 1000;

		//X軸の値の範囲設定
		devGamePad->SetProperty(DIPROP_RANGE, &diprg.diph);

		// Y軸の値の範囲設定
		diprg.diph.dwObj = DIJOFS_Y;
		devGamePad->SetProperty(DIPROP_RANGE, &diprg.diph);

		//排他制御レベルのセット
		result = devGamePad->SetCooperativeLevel(winApp->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
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
	if (devGamePad != nullptr)
	{
		//ゲームパッド情報の取得を開始
		result = devGamePad->Acquire();

		//前回の入力を保存
		gamePadPre = gamePad;

		result = devGamePad->GetDeviceState(sizeof(DIJOYSTATE), &gamePad);
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

bool Input::PushGamePadButton(GamePadButton gamePadButton)
{
	if (gamePad.rgbButtons[gamePadButton])
	{
		return true;
	}

	return false;
}

bool Input::LeftStick(GamePadLeftStick stick)
{
}

