#include "FenceManager.h"
#include "DxException.h"

FenceManager::FenceManager(ID3D12Device* device) 
    : m_nextFenceValue(1)
{
    for (UINT i = 0; i < FRAME_COUNT; ++i)
        m_fenceValues[i] = 0;

    THROW_IF_FAILED(device->CreateFence(m_fenceValues[0], D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence)), "Fence Creation Failed!");
    m_fenceValues[0] = m_nextFenceValue;
    m_nextFenceValue++;

    m_fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);

    if (m_fenceEvent == nullptr)
    {
        THROW_IF_FAILED(HRESULT_FROM_WIN32(GetLastError()));
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
        THROW_IF_FAILED(m_fence->SetEventOnCompletion(fenceValue, m_fenceEvent));

        // Wait for the GPU to signal the event
        WaitForSingleObjectEx(m_fenceEvent, INFINITE, FALSE);
    }
}

void FenceManager::Flush(ID3D12CommandQueue* commandQueue, UINT64& fenceValue)
{
    THROW_IF_FAILED(commandQueue->Signal(this->m_fence.Get(), ++fenceValue));

    if (this->m_fence->GetCompletedValue() < fenceValue)
    {
        THROW_IF_FAILED(this->m_fence->SetEventOnCompletion(fenceValue, this->m_fenceEvent));
        WaitForSingleObject(this->m_fenceEvent, INFINITE);
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
