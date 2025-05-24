#pragma once
#include "stdafx.h"

class SwapChainManager;

class FenceManager
{
public:
	FenceManager(ID3D12Device* device, SwapChainManager& swapChainManager);
	~FenceManager() = default;

	void WaitForFrameGPU(UINT frameIndex);
	void InitSignalFrames(ID3D12CommandQueue* commandQueue);

	UINT64 GetFenceValue(UINT frameIndex) const;
	ID3D12Fence* GetFence() const;
	HANDLE GetFenceEvent() const;
	UINT64 IncrementFenceValueAtIndex(UINT frameIndex);
	void CloseEvent();

private:
	HANDLE m_fenceEvent;
	ComPtr<ID3D12Fence> m_fence;
	UINT64 m_fenceValues[FRAME_COUNT];
	UINT64 m_nextFenceValue;

	friend SwapChainManager;
};

