#include "imguiManager.h"
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx12.h"
#include "DirectXCommon.h"

ComPtr<ID3D12DescriptorHeap> imguiManager::CreateDeateDescriptorHeapForImgui()
{
    ComPtr<ID3D12DescriptorHeap> ret;
    D3D12_DESCRIPTOR_HEAP_DESC desc = {};
    desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    desc.NodeMask = 0;
    desc.NumDescriptors = 1;
    desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

    DirectXCommon::GetDev()->CreateDescriptorHeap(&desc, IID_PPV_ARGS(ret.ReleaseAndGetAddressOf()));
    return ret;
}
