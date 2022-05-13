#pragma once

#include <d3dx12.h>
using namespace Microsoft::WRL;

class imguiManager
{
public:

	// imgui用のヒープ生成(宣言)
	static ComPtr<ID3D12DescriptorHeap> CreateDeateDescriptorHeapForImgui();
};

