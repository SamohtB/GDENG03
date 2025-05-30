#include "CommandQueueManager.h"
#include "DxException.h"

CommandQueueManager::CommandQueueManager(ID3D12Device* device)
{
    D3D12_COMMAND_QUEUE_DESC queueDesc = {};
    queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

    THROW_IF_FAILED(device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_commandQueue)), "Command Queue Creation Failed!");

    for (int i = 0; i < FRAME_COUNT; i++)
    {
        THROW_IF_FAILED(device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_commandAllocators[i])),
            "Allocator Creation Failed!");
    }

    THROW_IF_FAILED(device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_commandAllocators[0].Get(), nullptr, IID_PPV_ARGS(&m_commandList)),
        "CommandListCreation Failed");

    THROW_IF_FAILED(m_commandList->Close());

}

void CommandQueueManager::ExecuteCommandList()
{
    ID3D12CommandList* ppCommandLists[] = { m_commandList.Get() };
    this->m_commandQueue->ExecuteCommandLists(1, ppCommandLists);
}

void CommandQueueManager::ResetCommands(UINT frameIndex, ID3D12PipelineState* pipelineState)
{
    assert(frameIndex < FRAME_COUNT && "Frame index out of bounds!");
    auto allocator = m_commandAllocators[frameIndex].Get();

    // Command list allocators can only be reset when the associated 
    // command lists have finished execution on the GPU; apps should use 
    // fences to determine GPU execution progress.
    THROW_IF_FAILED(allocator->Reset());
    // However, when ExecuteCommandList() is called on a particular command 
    // list, that command list can then be reset at any time and must be before 
    // re-recording.
    THROW_IF_FAILED(m_commandList->Reset(allocator, pipelineState));
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