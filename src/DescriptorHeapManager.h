#pragma once
#include <vector>
#include "Dx12Commons.h"


class DescriptorHeapManager 
{
public:
    DescriptorHeapManager(ID3D12Device* device, UINT maxRTVCount = FRAME_COUNT, UINT maxSRVCount = SRV_MAX_COUNT);
    ~DescriptorHeapManager() = default;

    const std::vector<ID3D12DescriptorHeap*>& GetActiveHeaps() const;

    // RTV access
    UINT GetRTVDescriptorSize() const;
    ID3D12DescriptorHeap* GetRTVHeap() const;
    D3D12_CPU_DESCRIPTOR_HANDLE GetRTVCPUHandleAt(UINT frameIndex) const;

    // Shader Visible Heap access
    UINT GetSRVDescriptorSize() const;
    UINT AllocateSRVSlot();
    D3D12_CPU_DESCRIPTOR_HANDLE GetSRVCPUHandleAt(UINT index) const;
    D3D12_GPU_DESCRIPTOR_HANDLE GetSRVGPUHandleAt(UINT index) const;

private:
    std::vector<ID3D12DescriptorHeap*> m_activeHeaps;

    // RTV heap
    ComPtr<ID3D12DescriptorHeap> m_rtvHeap;
    UINT m_rtvDescriptorSize;
    UINT m_maxRTVCount;

    // SRV + CBV
    ComPtr<ID3D12DescriptorHeap> m_srvHeap;
    UINT m_srvDescriptorSize;
    UINT m_currentSRVOffset;
    UINT m_maxSRVCount;
};