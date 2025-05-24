#include "FenceManager.h"
#include "Helper.h"
#include "SwapChainManager.h"

FenceManager::FenceManager(ID3D12Device* device, SwapChainManager& swapChainManager) 
    : m_nextFenceValue(1)
{

    for (UINT i = 0; i < FRAME_COUNT; ++i)
        m_fenceValues[i] = 0;

    UINT frameIndex = swapChainManager.GetCurrentFrameIndex();
    ThrowIfFailed(device->CreateFence(m_fenceValues[frameIndex], D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence)));
    m_fenceValues[frameIndex] = m_nextFenceValue;
    m_nextFenceValue++;

    m_fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);

    if (m_fenceEvent == nullptr)
    {
        ThrowIfFailed(HRESULT_FROM_WIN32(GetLastError()));
    }
}

void FenceManager::WaitForFrameGPU(UINT frameIndex)
{
    UINT64 fenceValue = m_fenceValues[frameIndex];

    auto val = m_fence->GetCompletedValue();

    // GPU has not reached this fence yet — must wait
    if (val < fenceValue)
    {
        // Ask the GPU to signal an OS event when it hits this fence value
        ThrowIfFailed(m_fence->SetEventOnCompletion(fenceValue, m_fenceEvent));

        // Wait for the GPU to signal the event
        WaitForSingleObjectEx(m_fenceEvent, INFINITE, FALSE);
    }
}

void FenceManager::InitSignalFrames(ID3D12CommandQueue* commandQueue)
{
    for (int i = 0; i < FRAME_COUNT; ++i)
    {
        commandQueue->Signal(m_fence.Get(), m_fenceValues[i]);
        m_fenceValues[i]++;
    }
}

UINT64 FenceManager::GetFenceValue(UINT frameIndex) const
{
    return m_fenceValues[frameIndex];
}

ID3D12Fence* FenceManager::GetFence() const
{
    return m_fence.Get();
}

HANDLE FenceManager::GetFenceEvent() const
{
    return m_fenceEvent;
}

UINT64 FenceManager::IncrementFenceValueAtIndex(UINT frameIndex)
{
    UINT64 value = m_nextFenceValue++;
    m_fenceValues[frameIndex] = value;
    return value;
}

void FenceManager::CloseEvent()
{
    CloseHandle(m_fenceEvent);
}
