#include <windows.h>
#include <wrl.h>
#define DIRECTINPUT_VERSION  0x0800   //DirectInputのバージョン指定
#include <dinput.h>
#include "WinApp.h"
#pragma once

enum MouseButton
{
	Mouse_Left,			//左
	Mouse_Right,		//右
};



//入力
class Input
{
public:
	//namespace省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public: //メンバ関数

	static Input *GetInstance();

	//初期化
	void Initialize(WinApp *winApp);

	//更新
	void Update();

	///<summary>
	/// キーの押下をチェック
	///</summary>
	/// <param name="keyNumber">キー番号( DIK_0 等)</param>
	/// <returns>押されているか</returns>
	bool PushKey(BYTE keyNumber);

	///<summary>
	/// キーのトリガーをチェック
	///</summary>
	/// <param name="keyNumber">キー番号( DIK_0 等)</param>
	/// <returns>押されているか</returns>
	bool TriggerKey(BYTE keyNumber);

	/// <summary>
	/// マウス左クリックの押下をチェック
	/// </summary>
	/// <param name="mouse">マウスステータス</param>
	/// <returns>押されているかどうか</returns>
	bool PushLeftMouseButton();

	/// <summary>
	/// マウス右クリックの押下をチェック
	/// </summary>
	/// <param name="mouse">マウスステータス</param>
	/// <returns>押されているかどうか</returns>
	bool PushRightMouseButton();


	/// <summary>
	/// マウスのボタンが押されているかチェック
	/// </summary>
	/// <param name="mouse_button">マウス</param>
	/// <returns>押されているかどうか</returns>
	bool PushMouseButton(MouseButton mouse_button);

private: //メンバ変数
	//キーボードのデバイス
	ComPtr<IDirectInputDevice8> devkeyboard;
	//マウスのデバイス
	ComPtr<IDirectInputDevice8> devMouse;

	//DirectInputのインスタンス生成
	ComPtr<IDirectInput8> dinput;

	//全キーの状態
	BYTE key[256] = {};
	//前回の全キーの状態
	BYTE keyPre[256] = {};
	DIMOUSESTATE2 mousePre;
	DIMOUSESTATE2 mouse;


	//WindowsAPI
	WinApp *winApp = nullptr;
	
};