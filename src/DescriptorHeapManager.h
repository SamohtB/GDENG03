#pragma once
#include "Dx12Commons.h"

class DescriptorHeapManager 
{
public:
    DescriptorHeapManager(ID3D12Device* device, UINT maxRTVCount = FRAME_COUNT, UINT maxSRVCount = 64);
    ~DescriptorHeapManager() = default;

    const std::vector<ID3D12DescriptorHeap*>& GetActiveHeaps() const;

    // RTV access
    UINT GetRTVDescriptorSize() const;
    ID3D12DescriptorHeap* GetRTVHeap() const;
    D3D12_CPU_DESCRIPTOR_HANDLE GetRTVHandleFromFrame(UINT frameIndex) const;

    // SRV access
    UINT GetSRVDescriptorSize() const;
    ID3D12DescriptorHeap* GetSRVHeap() const;
    UINT AllocateSRVSlot();
    D3D12_CPU_DESCRIPTOR_HANDLE GetCPUHandleAt(UINT index) const;
    D3D12_GPU_DESCRIPTOR_HANDLE GetSRVHandleFromIndex(UINT index) const;

private:
    std::vector<ID3D12DescriptorHeap*> m_activeHeaps;

    // RTV heap
    ComPtr<ID3D12DescriptorHeap> m_rtvHeap;
    UINT m_rtvDescriptorSize;
    UINT m_maxRTVCount;

    // SRV heap
    ComPtr<ID3D12DescriptorHeap> m_srvHeap;
    UINT m_srvDescriptorSize;
    UINT m_currentSRVOffset;
    UINT m_maxSRVCount;

    /* future DSV and CBV here */
};