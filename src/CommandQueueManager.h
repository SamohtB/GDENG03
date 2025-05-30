#pragma once
#include "DX12Common.h"

class CommandQueueManager
{
public:
	CommandQueueManager(ID3D12Device* device);
	~CommandQueueManager() = default;

	void ExecuteCommandList();
	void ResetCommands(UINT frameIndex, ID3D12PipelineState* pipelineState = nullptr);

	ID3D12CommandQueue* GetCommandQueue() const;
	ID3D12CommandAllocator* GetCommandAllocator(UINT frameIndex) const;
	ID3D12GraphicsCommandList* GetCommandList() const;

private:
	ComPtr<ID3D12CommandQueue> m_commandQueue;
	ComPtr<ID3D12CommandAllocator> m_commandAllocators[FRAME_COUNT];
	ComPtr<ID3D12GraphicsCommandList> m_commandList;
};

