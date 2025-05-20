#pragma once
#include "stdafx.h"

class CommandQueueManager
{
public:
	CommandQueueManager(ID3D12Device* device);
	~CommandQueueManager() = default;

	ID3D12CommandQueue* GetCommandQueue() const;
	ID3D12CommandAllocator* GetCommandAllocator(UINT frameIndex) const;
	ID3D12GraphicsCommandList* GetCommandList() const;
	void CreateCommandLists(ID3D12Device* device, ID3D12PipelineState* pipelineState, UINT frameIndex);

private:
	ComPtr<ID3D12CommandQueue> m_commandQueue;
	ComPtr<ID3D12CommandAllocator> m_commandAllocators[FRAME_COUNT];
	ComPtr<ID3D12GraphicsCommandList> m_commandList;
};

