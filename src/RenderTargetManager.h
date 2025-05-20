#pragma once
#include <vector>
#include "stdafx.h"

class DescriptorHeapManager;

class RenderTargetManager 
{
public:
    RenderTargetManager(ID3D12Device* device, IDXGISwapChain3* swapChain, DescriptorHeapManager& heapManager);
    ~RenderTargetManager() = default;

    ID3D12Resource* GetRenderTarget(UINT index) const;

private:
    ComPtr<ID3D12Resource> m_renderTargets[FRAME_COUNT];

};