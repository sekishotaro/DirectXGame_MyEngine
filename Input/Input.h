#pragma once
#include "WinApp.h"
#include <windows.h>
#include <dinput.h>
#include <wrl.h>
#include <DirectXMath.h>

#define DIRECTINPUT_VERSION  0x0800   //DirectInputのバージョン指定

enum MouseButton
{
	Mouse_Left,			//左
	Mouse_Right,		//右
	Mouse_Middle		//真ん中
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



//入力
class Input
{
public:
	struct MousePos
	{
		LONG lX;
		LONG lY;
		LONG lZ;
	};

private: // エイリアス
// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;

private:
	Input();

	~Input();

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
	/// マウスのボタンが押されているかチェック
	/// </summary>
	/// <param name="mouse_button">マウス</param>
	/// <returns>押されているかどうか</returns>
	bool PushMouseButton(MouseButton mouse_button);

	MousePos MousePosLoad();

	/// <summary>
	/// マウスの位置
	/// </summary>
	/// <returns></returns>
	static XMFLOAT2 GetMousePos() { return mousePos; }

	//コピーコンストラクタの禁止
	Input(const Input& input) = delete;
	//代入演算子の禁止
	Input& operator = (const Input& input) = delete;

	//ゲームパッドボタン
	bool PushGamePadButton(GamePadButton gamePadButton);

	//ゲームパッド左スティック
	bool LeftStick(GamePadLeftStick stick);

private: //メンバ変数
	//DirectInputのインスタンス生成
	ComPtr<IDirectInput8> dinput;

	//キーボードのデバイス
	ComPtr<IDirectInputDevice8> devkeyboard;
	//全キーの状態
	BYTE key[256] = {};
	//前回の全キーの状態
	BYTE keyPre[256] = {};

	//マウスのデバイス
	ComPtr<IDirectInputDevice8> devMouse;
	//マウスの判定
	DIMOUSESTATE2 mouse;
	//前フレームの判定
	DIMOUSESTATE2 mousePre;

	//ゲームパッドのデバイス
	ComPtr<IDirectInputDevice8> devGamePad;
	//ゲームパッドの判定
	DIJOYSTATE gamePad = {};
	//前ゲームパッドの判定
	DIJOYSTATE gamePadPre = {};

	//WindowsAPI
	WinApp *winApp = nullptr;
	POINT p;
	static XMFLOAT2 mousePos;
	
};