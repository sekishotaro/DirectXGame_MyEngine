#pragma once

#include <d3dx12.h>
using namespace Microsoft::WRL;

class imguiManager
{
public:

	// imgui�p�̃q�[�v����(�錾)
	static ComPtr<ID3D12DescriptorHeap> CreateDeateDescriptorHeapForImgui();
};

