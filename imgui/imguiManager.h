#pragma once

#include <d3dx12.h>
using namespace Microsoft::WRL;

class imguiManager
{
public:

	// imgui用のヒープ生成(宣言)
	static ComPtr<ID3D12DescriptorHeap> CreateDeateDescriptorHeapForImgui();

	//imgui用のヒープアクセサー
	static ComPtr<ID3D12DescriptorHeap> GetHeapForImgui();

	/// <summary>
	/// 初期化
	/// </summary>
	static void Initialize(); 

	static void PraDraw();

	static void PosDraw();

	static ComPtr<ID3D12DescriptorHeap> heapImgui;

	static ComPtr<ID3D12DescriptorHeap> heapForImgui;	//ヒープ保持用
};

