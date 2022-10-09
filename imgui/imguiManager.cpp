#include "imguiManager.h"
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx12.h"
#include "DirectXCommon.h"
#include "SafeDelete.h"
#include "WinApp.h"

ComPtr<ID3D12DescriptorHeap> imguiManager::heapImgui;
ComPtr<ID3D12DescriptorHeap> imguiManager::heapForImgui;

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

ComPtr<ID3D12DescriptorHeap> imguiManager::GetHeapForImgui()
{
    return heapImgui;
}

void imguiManager::Initialize()
{
    //imgui�̏�����
    heapImgui = CreateDeateDescriptorHeapForImgui();

    if (heapImgui == nullptr)
    {
        assert(0);
    }

    if (ImGui::CreateContext() == nullptr)
    {
        assert(0);
        return;
    }
    //[ImGui::]
    bool blnResult = ImGui_ImplWin32_Init(WinApp::GetHwnd());
    if (!blnResult)
    {
        assert(0);
        return;
    }

    blnResult = ImGui_ImplDX12_Init(
        DirectXCommon::GetDev(),                                    //DirectX�f�o�C�X
        3,                                                          //������P590�L��
        DXGI_FORMAT_R8G8B8A8_UNORM,                                 //�������ݗpRTV�̃t�H�[�}�b�g
        GetHeapForImgui().Get(),                                    //imgui�p�f�X�N���v�^�q�[�v
        GetHeapForImgui()->GetCPUDescriptorHandleForHeapStart(),    //CPU�n���h��
        GetHeapForImgui()->GetGPUDescriptorHandleForHeapStart()     //GPU�n���h��
    );
}

void imguiManager::PraDraw()
{
    ImGui_ImplDX12_NewFrame();
    ImGui_ImplWin32_NewFrame();
}

void imguiManager::PosDraw()
{
    ImGui::End();
    ImGui::Render();
    DirectXCommon::GetCmdList()->SetDescriptorHeaps(1, GetHeapForImgui().GetAddressOf());
    ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), DirectXCommon::GetCmdList());
}
