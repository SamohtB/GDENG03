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
