#pragma once
#include "stdafx.h"

class DescriptorHeapManager 
{
public:
    DescriptorHeapManager(ComPtr<ID3D12Device> device);
    ~DescriptorHeapManager() = default;

    D3D12_CPU_DESCRIPTOR_HANDLE GetRTVHeapStart();
    UINT GetRTVDescriptorSize() const;
    ComPtr<ID3D12DescriptorHeap> GetRTVHeap() const;

private:
    ComPtr<ID3D12DescriptorHeap> m_rtvHeap;
    UINT m_rtvDescriptorSize;
};