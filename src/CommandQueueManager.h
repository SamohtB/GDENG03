#pragma once
#include "stdafx.h"

class CommandQueueManager
{
public:
	CommandQueueManager(ComPtr<ID3D12Device> device);
	~CommandQueueManager() = default;

	ComPtr<ID3D12CommandQueue> GetCommandQueue() const;
	ComPtr<ID3D12CommandAllocator> GetCommandAllocator(UINT frameIndex) const;
	ComPtr<ID3D12GraphicsCommandList> GetCommandList() const;
	void CreateCommandLists(ComPtr<ID3D12Device> device, ComPtr<ID3D12PipelineState> pipelineState, UINT frameIndex);

private:
	ComPtr<ID3D12CommandQueue> m_commandQueue;
	ComPtr<ID3D12CommandAllocator> m_commandAllocators[FRAME_COUNT];
	ComPtr<ID3D12GraphicsCommandList> m_commandList;
};

