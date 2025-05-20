#include "CommandQueueManager.h"
#include "Helper.h"

CommandQueueManager::CommandQueueManager(ID3D12Device* device)
{
    D3D12_COMMAND_QUEUE_DESC queueDesc = {};
    queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

    ThrowIfFailed(device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_commandQueue)));

    for (int i = 0; i < FRAME_COUNT; i++)
    {
        ThrowIfFailed(device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_commandAllocators[i])));
    }
    
}

ID3D12CommandQueue* CommandQueueManager::GetCommandQueue() const
{
    return this->m_commandQueue.Get();
}

ID3D12CommandAllocator* CommandQueueManager::GetCommandAllocator(UINT frameIndex) const
{
    return this->m_commandAllocators[frameIndex].Get();
}

ID3D12GraphicsCommandList* CommandQueueManager::GetCommandList() const
{
    return this->m_commandList.Get();
}

void CommandQueueManager::CreateCommandLists(ID3D12Device* device, ID3D12PipelineState* pipelineState, UINT frameIndex)
{
    ThrowIfFailed(device->CreateCommandList(0, 
        D3D12_COMMAND_LIST_TYPE_DIRECT, 
        m_commandAllocators[frameIndex].Get(),
        pipelineState, 
        IID_PPV_ARGS(&m_commandList)));

    ThrowIfFailed(m_commandList->Close());
}