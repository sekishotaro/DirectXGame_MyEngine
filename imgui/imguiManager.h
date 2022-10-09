#pragma once

#include <d3dx12.h>
using namespace Microsoft::WRL;

class imguiManager
{
public:

	// imgui�p�̃q�[�v����(�錾)
	static ComPtr<ID3D12DescriptorHeap> CreateDeateDescriptorHeapForImgui();

	//imgui�p�̃q�[�v�A�N�Z�T�[
	static ComPtr<ID3D12DescriptorHeap> GetHeapForImgui();

	/// <summary>
	/// ������
	/// </summary>
	static void Initialize(); 

	static void PraDraw();

	static void PosDraw();

	static ComPtr<ID3D12DescriptorHeap> heapImgui;

	static ComPtr<ID3D12DescriptorHeap> heapForImgui;	//�q�[�v�ێ��p
};

