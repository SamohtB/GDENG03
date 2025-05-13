#include "CommandQueueManager.h"
#include "Helper.h"

CommandQueueManager::CommandQueueManager(ComPtr<ID3D12Device> device)
{
    D3D12_COMMAND_QUEUE_DESC queueDesc = {};
    queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

    ThrowIfFailed(device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_commandQueue)));
    ThrowIfFailed(device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_commandAllocator)));
}

ComPtr<ID3D12CommandQueue> CommandQueueManager::GetCommandQueue() const
{
    return this->m_commandQueue;
}

ComPtr<ID3D12CommandAllocator> CommandQueueManager::GetCommandAllocator() const
{
    return this->m_commandAllocator;
}

ComPtr<ID3D12GraphicsCommandList> CommandQueueManager::GetCommandList() const
{
    return this->m_commandList;
}

void CommandQueueManager::CreateCommandLists(ComPtr<ID3D12Device> device, ComPtr<ID3D12PipelineState> pipelineState)
{
    ThrowIfFailed(device->CreateCommandList(0, 
        D3D12_COMMAND_LIST_TYPE_DIRECT, 
        m_commandAllocator.Get(), 
        pipelineState.Get(), 
        IID_PPV_ARGS(&m_commandList)));

    ThrowIfFailed(m_commandList->Close());
}