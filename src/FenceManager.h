#pragma once
#include "stdafx.h"

class FenceManager
{
public:
	FenceManager(ComPtr<ID3D12Device> device);
	~FenceManager() = default;

	UINT64 GetFenceValue();
	ComPtr<ID3D12Fence> GetFence();
	HANDLE GetFenceEvent();
	void IncrementFence();
	void CloseEvent();

private:
	HANDLE m_fenceEvent;
	ComPtr<ID3D12Fence> m_fence;
	UINT64 m_fenceValue;
};

