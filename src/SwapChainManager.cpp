#include "SwapChainManager.h"
#include "Helper.h"

SwapChainManager::SwapChainManager(IDXGIFactory6* factory, ID3D12CommandQueue* commandQueue, UINT width, UINT height, HWND hwnd)
    : m_frameIndex(0)
{
    DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
    swapChainDesc.BufferCount = FRAME_COUNT; 
    swapChainDesc.Width = width;
    swapChainDesc.Height = height;
    swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swapChainDesc.SampleDesc.Count = 1;

    ComPtr<IDXGISwapChain1> swapChain;

    ThrowIfFailed(factory->CreateSwapChainForHwnd(
        commandQueue,
        hwnd,
        &swapChainDesc,
        nullptr,
        nullptr,
        &swapChain
    ));

    ThrowIfFailed(factory->MakeWindowAssociation(hwnd, DXGI_MWA_NO_ALT_ENTER));

    ThrowIfFailed(swapChain.As(&m_swapChain));
    UpdateFrameIndex();
}

IDXGISwapChain3* SwapChainManager::GetSwapChain() const
{
    return m_swapChain.Get();
}

UINT SwapChainManager::GetCurrentFrameIndex() const
{
    return m_frameIndex;
}

void SwapChainManager::UpdateFrameIndex()
{
    m_frameIndex = m_swapChain->GetCurrentBackBufferIndex();
}
