#pragma once
#include "stdafx.h"

class CommandQueueManager
{
public:
	CommandQueueManager(ComPtr<ID3D12Device> device);
	~CommandQueueManager() = default;

	ComPtr<ID3D12CommandQueue> GetCommandQueue() const;
	ComPtr<ID3D12CommandAllocator> GetCommandAllocator() const;
	ComPtr<ID3D12GraphicsCommandList> GetCommandList() const;
	void CreateCommandLists(ComPtr<ID3D12Device> device, ComPtr<ID3D12PipelineState> pipelineState);

private:
	ComPtr<ID3D12CommandQueue> m_commandQueue;
	ComPtr<ID3D12CommandAllocator> m_commandAllocator;
	ComPtr<ID3D12GraphicsCommandList> m_commandList;
};

