#pragma once

#include <d3dx12.h>
using namespace Microsoft::WRL;

class imguiManager
{
public:

	// imgui—p‚Ìƒq[ƒv¶¬(éŒ¾)
	static ComPtr<ID3D12DescriptorHeap> CreateDeateDescriptorHeapForImgui();
};

