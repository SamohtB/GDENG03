#include "RenderTargetManager.h"

#include "DeviceManager.h"
#include "SwapChainManager.h"
#include "DescriptorHeapManager.h"

#include "Helper.h"

RenderTargetManager::RenderTargetManager(ComPtr<ID3D12Device> device, ComPtr<IDXGISwapChain3> swapChain, DescriptorHeapManager& heapManager)
{
    CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(heapManager.GetRTVHeapStart());

    for (UINT n = 0; n < FRAME_COUNT; n++)
    {
        ThrowIfFailed(swapChain->GetBuffer(n, IID_PPV_ARGS(&m_renderTargets[n])));
        device->CreateRenderTargetView(m_renderTargets[n].Get(), nullptr, rtvHandle);
        rtvHandle.Offset(1, heapManager.GetRTVDescriptorSize());
    }
}

ComPtr<ID3D12Resource> RenderTargetManager::GetRenderTarget(UINT index) const
{
	return m_renderTargets[index];
}
