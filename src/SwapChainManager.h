#pragma once
#include "stdafx.h"

class SwapChainManager
{
public:
	SwapChainManager(ComPtr<IDXGIFactory6> factory, ComPtr<ID3D12CommandQueue> commandQueue, UINT width, UINT height);
	~SwapChainManager() = default;

	ComPtr<IDXGISwapChain3> GetSwapChain() const { return m_swapChain; };
private:
	UINT m_frameIndex;
	ComPtr<IDXGISwapChain3> m_swapChain;
};

 