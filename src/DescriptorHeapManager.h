#pragma once
#include <vector>
#include "Dx12Commons.h"


class DescriptorHeapManager 
{
public:
    DescriptorHeapManager(ID3D12Device* device, UINT maxRTVCount = FRAME_COUNT, UINT maxSRVCount = SRV_MAX_COUNT, UINT maxCBVCount = CBV_MAX_COUNT);
    ~DescriptorHeapManager() = default;

    const std::vector<ID3D12DescriptorHeap*>& GetActiveHeaps() const;

    // RTV access
    UINT GetRTVDescriptorSize() const;
    ID3D12DescriptorHeap* GetRTVHeap() const;
    D3D12_CPU_DESCRIPTOR_HANDLE GetRTVCPUHandleAt(UINT frameIndex) const;

    // Shader Visible Heap access
    UINT GetShaderVisibleDescriptorSize() const;
    UINT AllocateSRVSlot();
    UINT AllocateCBVSlot();
    D3D12_CPU_DESCRIPTOR_HANDLE GetShaderVisibleCPUHandleAt(UINT index) const;
    D3D12_GPU_DESCRIPTOR_HANDLE GetShaderVisibleGPUHandleAt(UINT index) const;
   

private:
    std::vector<ID3D12DescriptorHeap*> m_activeHeaps;

    // RTV heap
    ComPtr<ID3D12DescriptorHeap> m_rtvHeap;
    UINT m_rtvDescriptorSize;
    UINT m_maxRTVCount;

    // SRV + CBV
    ComPtr<ID3D12DescriptorHeap> m_shaderVisibleHeap;
    UINT m_shaderVisibleDescriptorSize;
    UINT m_currentSRVOffset;
    UINT m_currentCBVOffset;
    UINT m_maxSRVCount;
    UINT m_maxCBVCount;
    UINT m_cbvEnd;
};