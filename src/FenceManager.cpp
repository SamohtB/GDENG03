#include "FenceManager.h"
#include "Debug.h"

FenceManager::FenceManager(ID3D12Device* device) 
    : m_nextFenceValue(1)
{
    for (UINT i = 0; i < FRAME_COUNT; ++i)
        m_fenceValues[i] = 0;

    Debug::ThrowIfFailed(device->CreateFence(m_fenceValues[0], D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence)), "Fence creation failed!");
    m_fenceValues[0] = m_nextFenceValue;
    m_nextFenceValue++;

    m_fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);

    if (m_fenceEvent == nullptr)
    {
        Debug::ThrowIfFailed(HRESULT_FROM_WIN32(GetLastError()));
    }
}

void FenceManager::WaitForFrameGPU(UINT frameIndex)
{
    UINT64 fenceValue = m_fenceValues[frameIndex];
    auto val = m_fence->GetCompletedValue();

    // GPU has not reached this fence yet — must wait
    if (val < fenceValue)
    {
        Debug::ThrowIfFailed(m_fence->SetEventOnCompletion(fenceValue, m_fenceEvent));
        WaitForSingleObjectEx(m_fenceEvent, INFINITE, FALSE);
        Debug::Log("Waiting For: " + std::to_string(fenceValue));
    }
}

void FenceManager::SignalCurrentFrameGPU(ID3D12CommandQueue* queue, UINT frameIndex)
{
    UINT64 value = m_nextFenceValue++;
    m_fenceValues[frameIndex] = value;
    Debug::ThrowIfFailed(queue->Signal(this->m_fence.Get(), value));
    Debug::Log("Signaling: " + std::to_string(value));
}

void FenceManager::ShutDown()
{
    CloseHandle(m_fenceEvent);
}
