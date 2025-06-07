#include "DeviceContext.h"
#include "Debug.h"

DeviceContext::DeviceContext(ID3D12Device* device)
{
    D3D12_COMMAND_QUEUE_DESC queueDesc = {};
    queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

    Debug::ThrowIfFailed(device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_commandQueue)), "Command Queue creation failed!");

    for (int i = 0; i < FRAME_COUNT; i++)
    {
        Debug::ThrowIfFailed(device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_commandAllocators[i])),
            "Command Allocator creation failed!");
    }

    Debug::ThrowIfFailed(device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_commandAllocators[0].Get(), 
        nullptr, IID_PPV_ARGS(&m_commandList)), "Command List creation failed!");

    Debug::ThrowIfFailed(this->m_commandList->Close());
}

void DeviceContext::ExecuteCommandList()
{
    Debug::ThrowIfFailed(this->m_commandList->Close());
    ID3D12CommandList* commandLists[] = { m_commandList.Get() };
    this->m_commandQueue->ExecuteCommandLists(1, commandLists);
}

void DeviceContext::ResetCommands(UINT frameIndex, ID3D12PipelineState* pipelineState)
{
    auto allocator = m_commandAllocators[frameIndex].Get();

    // Command list allocators can only be reset when the associated 
    // command lists have finished execution on the GPU; apps should use 
    // fences to determine GPU execution progress.
    Debug::ThrowIfFailed(allocator->Reset());
    // However, when ExecuteCommandList() is called on a particular command 
    // list, that command list can then be reset at any time and must be before 
    // re-recording.
    Debug::ThrowIfFailed(m_commandList->Reset(allocator, pipelineState));
}

void DeviceContext::SetRootSignature(ID3D12RootSignature* rootSignature)
{
    this->m_commandList->SetGraphicsRootSignature(rootSignature);
}

void DeviceContext::SetPSO(ID3D12PipelineState* pipelineState)
{
    this->m_commandList->SetPipelineState(pipelineState);
}

void DeviceContext::SetDescriptorHeaps(const std::vector<ID3D12DescriptorHeap*>& heaps)
{
    this->m_commandList->SetDescriptorHeaps(static_cast<UINT>(heaps.size()), heaps.data());
}

void DeviceContext::SetGlobalConstantBuffer(D3D12_GPU_VIRTUAL_ADDRESS address)
{
    this->m_commandList->SetGraphicsRootConstantBufferView(RootDescriptorIndex::GLOBAL_CONST, address);
}

void DeviceContext::SetMaterialBuffer(D3D12_GPU_DESCRIPTOR_HANDLE handle)
{
    this->m_commandList->SetGraphicsRootDescriptorTable(RootDescriptorIndex::MAT_CONST, handle);
}

void DeviceContext::SetViewport(CD3DX12_VIEWPORT* viewport)
{
    m_commandList->RSSetViewports(1, viewport);
}

void DeviceContext::SetScissorRect(CD3DX12_RECT* rect)
{
    this->m_commandList->RSSetScissorRects(1, rect);
}

/* Use before rendering */
void DeviceContext::TransitionToRenderTarget(ID3D12Resource* resource)
{
    CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
        resource,
        D3D12_RESOURCE_STATE_PRESENT,
        D3D12_RESOURCE_STATE_RENDER_TARGET
    );

    m_commandList->ResourceBarrier(1, &barrier);
}

/* Use after rendering */
void DeviceContext::TransitionToPresent(ID3D12Resource* resource)
{
    CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
        resource,
        D3D12_RESOURCE_STATE_RENDER_TARGET,
        D3D12_RESOURCE_STATE_PRESENT
    );

    m_commandList->ResourceBarrier(1, &barrier);
}

void DeviceContext::ClearRenderTargetColor(D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle, float red, float green, float blue, float alpha)
{
    const float clearColor[] = { red, green, blue, alpha };

    this->m_commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
    this->m_commandList->OMSetRenderTargets(1, &rtvHandle, FALSE, nullptr);
}

void DeviceContext::SetTexture(D3D12_GPU_DESCRIPTOR_HANDLE handle)
{
    this->m_commandList->SetGraphicsRootDescriptorTable(RootDescriptorIndex::TEXTURES, handle);
}

void DeviceContext::SetVertexBuffer(D3D12_VERTEX_BUFFER_VIEW* vertexBufferView)
{
    this->m_commandList->IASetVertexBuffers(0, 1, vertexBufferView);
}

void DeviceContext::SetIndexBuffer(D3D12_INDEX_BUFFER_VIEW* indexBufferView)
{
    this->m_commandList->IASetIndexBuffer(indexBufferView);
}

void DeviceContext::DrawTriangleList(UINT vertexCount, UINT startVertexIndex)
{
    this->m_commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    this->m_commandList->DrawInstanced(vertexCount, 1, startVertexIndex, 0);
}

void DeviceContext::DrawIndexedTriangleList(UINT indexCount, UINT startVertexIndex, UINT startIndexLocation)
{
    this->m_commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    this->m_commandList->DrawIndexedInstanced(indexCount, 1, startIndexLocation, startVertexIndex, 0);
}

ID3D12CommandQueue* DeviceContext::GetCommandQueue() const
{
    return this->m_commandQueue.Get();
}
