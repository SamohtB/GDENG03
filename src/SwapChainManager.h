#pragma once
#include "stdafx.h"

class SwapChainManager
{
public:
	SwapChainManager(IDXGIFactory6* factory, ID3D12CommandQueue* commandQueue, UINT width, UINT height, HWND hwnd);
	~SwapChainManager() = default;

	IDXGISwapChain3* GetSwapChain() const;
	UINT GetCurrentFrameIndex() const;
	void UpdateFrameIndex();

private:
	UINT m_frameIndex;
	ComPtr<IDXGISwapChain3> m_swapChain;
};

 