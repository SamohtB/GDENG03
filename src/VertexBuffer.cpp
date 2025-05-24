#include "VertexBuffer.h"
#include "VertexTypes.h"
#include "GraphicsEngine.h"

template <typename VertexType>
VertexBuffer::VertexBuffer(const std::vector<VertexType>& vertices)
{
    static_assert(std::is_base_of<IVertexType, VertexType>::value, "Invalid vertex type used!");

    auto device = GraphicsEngine::GetInstance()->GetRenderSystem()->GetD3DDevice();
    auto list = GraphicsEngine::GetInstance()->GetRenderSystem()->GetCommandList();
    const UINT vertexBufferSize = static_cast<UINT>(vertices.size() * sizeof(VertexType));

    CD3DX12_HEAP_PROPERTIES defaultHeapProps(D3D12_HEAP_TYPE_DEFAULT);
    CD3DX12_HEAP_PROPERTIES uploadHeapProps(D3D12_HEAP_TYPE_UPLOAD);

    CD3DX12_RESOURCE_DESC resourceDesc = CD3DX12_RESOURCE_DESC::Buffer(vertexBufferSize);

    ThrowIfFailed(device->CreateCommittedResource(
        &defaultHeapProps,
        D3D12_HEAP_FLAG_NONE,
        &resourceDesc,
        D3D12_RESOURCE_STATE_COPY_DEST,
        nullptr,
        IID_PPV_ARGS(&m_vertexBuffer)));

    UINT64 uploadBufferSize = GetRequiredIntermediateSize(m_vertexBuffer.Get(), 0, 1);
    CD3DX12_RESOURCE_DESC cdDesc = CD3DX12_RESOURCE_DESC::Buffer(uploadBufferSize);

    /* TO DO: Move uploads somewhere else to that it can be cleared after GPU upload */ 
    ThrowIfFailed(device->CreateCommittedResource(
        &uploadHeapProps,
        D3D12_HEAP_FLAG_NONE,
        &cdDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&m_vertexUploadHeap)));

    // Use UpdateSubresources to copy into default heap
    D3D12_SUBRESOURCE_DATA vertexData = {};
    vertexData.pData = vertices.data();
    vertexData.RowPitch = vertexBufferSize;
    vertexData.SlicePitch = vertexBufferSize;

    UpdateSubresources(list, m_vertexBuffer.Get(), m_vertexUploadHeap.Get(), 0, 0, 1, &vertexData);

    // Transition the default heap to VERTEX_AND_CONSTANT_BUFFER state
    CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
        m_vertexBuffer.Get(),
        D3D12_RESOURCE_STATE_COPY_DEST,
        D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);

    list->ResourceBarrier(1, &barrier);

    // Initialize the vertex buffer view.
    m_vertexBufferView.BufferLocation = m_vertexBuffer->GetGPUVirtualAddress();
    m_vertexBufferView.StrideInBytes = sizeof(VertexType);
    m_vertexBufferView.SizeInBytes = vertexBufferSize;
}

template VertexBuffer::VertexBuffer(const std::vector<Vertex3D>&);
template VertexBuffer::VertexBuffer(const std::vector<TexturedVertex3D>&);


D3D12_VERTEX_BUFFER_VIEW* VertexBuffer::GetVertexBufferViewPointer()
{
    return &m_vertexBufferView;
}
