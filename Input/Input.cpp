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
	//借りてきたWinAppのインスタンスを記録
	this->winApp = winApp;

	HRESULT result;

	result = DirectInput8Create(
		winApp->GetHInstance(), DIRECTINPUT_VERSION, IID_IDirectInput8, (void **)&dinput, nullptr);

	//キーボードデバイス生成
	result = dinput->CreateDevice(GUID_SysKeyboard, &devkeyboard, NULL);
	//入力形式のセット
	result = devkeyboard->SetDataFormat(&c_dfDIKeyboard);
	//排他制御レベルのセット
	result = devkeyboard->SetCooperativeLevel(
		winApp->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);

	//マウス生成
	result = dinput->CreateDevice(GUID_SysMouse, &devMouse, NULL);
	//入力形式のセット
	result = devMouse->SetDataFormat(&c_dfDIMouse2);
	//排他制御レベルのセット
	result = devMouse->SetCooperativeLevel(
		winApp->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
}

void Input::Update()
{
	HRESULT result;

	//キーボード
	{
		//キーボード情報の取得を開始
		result = devkeyboard->Acquire();

		//前回のキー入力を保存
		memcpy(keyPre, key, sizeof(key));

		result = devkeyboard->GetDeviceState(sizeof(key), key);
	}

	//マウス
	{
		//マウス情報の取得を開始
		result = devMouse->Acquire();

		//前回の入力を保存
		mousePre = mouse;

		result = devMouse->GetDeviceState(sizeof(mouse), &mouse);
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

