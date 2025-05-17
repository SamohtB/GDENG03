#pragma once
#include "stdafx.h"

class SwapChainManager;

class FenceManager
{
public:
	FenceManager(ComPtr<ID3D12Device> device, SwapChainManager& swapChainManager);
	~FenceManager() = default;

	UINT64 GetFenceValue(UINT frameIndex) const;
	ComPtr<ID3D12Fence> GetFence() const;
	HANDLE GetFenceEvent() const;
	void SetFenceValue(UINT frameIndex, UINT64 value);
	void CloseEvent();

private:
	HANDLE m_fenceEvent;
	ComPtr<ID3D12Fence> m_fence;
	UINT64 m_fenceValues[FRAME_COUNT];

	friend SwapChainManager;
};

