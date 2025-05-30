#pragma once
#include "stdafx.h"

class DescriptorHeapManager 
{
public:
    DescriptorHeapManager(ID3D12Device* device);
    ~DescriptorHeapManager() = default;

    UINT GetRTVDescriptorSize() const;
    ID3D12DescriptorHeap* GetRTVHeap() const;
    ID3D12DescriptorHeap* GetCBVHeap() const;

private:
    ComPtr<ID3D12DescriptorHeap> m_rtvHeap;
    ComPtr<ID3D12DescriptorHeap> m_cbvHeap;
    UINT m_rtvDescriptorSize;
};