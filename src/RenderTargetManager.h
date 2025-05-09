#pragma once
#include <vector>
#include "stdafx.h"

class DescriptorHeapManager;

class RenderTargetManager 
{
public:
    RenderTargetManager(ComPtr<ID3D12Device> device, ComPtr<IDXGISwapChain3> swapChain, DescriptorHeapManager& heapManager);
    ID3D12Resource* GetRenderTarget(UINT index) const;

private:
    std::vector<ComPtr<ID3D12Resource>> m_renderTargets;
};