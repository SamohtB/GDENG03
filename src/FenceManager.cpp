#include "FenceManager.h"
#include "Helper.h"
#include "SwapChainManager.h"

FenceManager::FenceManager(ComPtr<ID3D12Device> device, SwapChainManager& swapChainManager) 
    : m_fenceValues{}
{
    UINT frameIndex = swapChainManager.GetCurrentFrameIndex();
    ThrowIfFailed(device->CreateFence(m_fenceValues[frameIndex], D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence)));
    this->m_fenceValues[frameIndex]++;

    m_fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);

    if (m_fenceEvent == nullptr)
    {
        ThrowIfFailed(HRESULT_FROM_WIN32(GetLastError()));
    }
}

UINT64 FenceManager::GetFenceValue(UINT frameIndex) const
{
    return m_fenceValues[frameIndex];
}

ComPtr<ID3D12Fence> FenceManager::GetFence() const
{
    return m_fence;
}

HANDLE FenceManager::GetFenceEvent() const
{
    return m_fenceEvent;
}

void FenceManager::SetFenceValue(UINT frameIndex, UINT64 value)
{
    this->m_fenceValues[frameIndex] = value;
}

void FenceManager::CloseEvent()
{
    CloseHandle(m_fenceEvent);
}
