#pragma once

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>

#include "Input.h"
#include "FbxObject3d.h"

class Player
{
private: // エイリアス
// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	//移動
	static void Move(Input *input, const float& groundY);
	
	static void MoveAdd(XMFLOAT3 Addpos);
	//移動量取得
	static XMFLOAT3 GetMove() { return move; }

	static void AddPos(XMFLOAT3 move) { pos.x += move.x; pos.y += move.y; pos.z += move.z; }

	static void SetPosY(float& posy) { pos.y = posy; }
	static void SetPos(XMFLOAT3& pos1) { pos = pos1; }

	static void SetWallColl(bool& wallcoll) { wallcollisionFlag = wallcoll; }

	static bool GetWallColl() { return wallcollisionFlag; }

	static XMFLOAT3 GetPos() { return pos; }

	static XMFLOAT3 GetSize() { return size; }

	static void Jump( float& y);

private:
	static XMFLOAT3 pos;
	static XMFLOAT3 move;
	static XMFLOAT3 rot;
	static XMFLOAT3 size;
	static bool wallcollisionFlag;
	static bool jumpFlag;

public:
	static bool groundFlag;
};

