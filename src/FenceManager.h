#pragma once
#include "Dx12Commons.h"

class SwapChainManager;

class FenceManager
{
public:
	FenceManager(ID3D12Device* device);
	~FenceManager() = default;

	void WaitForFrameGPU(UINT frameIndex);
	void SignalCurrentFrameGPU(ID3D12CommandQueue* queue, UINT frameIndex);
	void ShutDown();

private:
	HANDLE m_fenceEvent;
	ComPtr<ID3D12Fence> m_fence;
	UINT64 m_fenceValues[FRAME_COUNT];
	UINT64 m_nextFenceValue;

	friend SwapChainManager;
};

