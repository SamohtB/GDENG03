#include "FenceManager.h"
#include "Helper.h"

FenceManager::FenceManager(ComPtr<ID3D12Device> device)
{
    ThrowIfFailed(device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence)));
    m_fenceValue = 1;

    m_fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);

    if (m_fenceEvent == nullptr)
    {
        ThrowIfFailed(HRESULT_FROM_WIN32(GetLastError()));
    }
}

UINT64 FenceManager::GetFenceValue()
{
    return m_fenceValue;
}

ComPtr<ID3D12Fence> FenceManager::GetFence()
{
    return m_fence;
}

HANDLE FenceManager::GetFenceEvent()
{
    return m_fenceEvent;
}

void FenceManager::IncrementFence()
{
    this->m_fenceValue++;
}

void FenceManager::CloseEvent()
{
    CloseHandle(m_fenceEvent);
}
