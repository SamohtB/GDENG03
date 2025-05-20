#include "VertexBuffer.h"
#include "VertexTypes.h"

template <typename VertexType>
VertexBuffer::VertexBuffer(ComPtr<ID3D12Device> device, const std::vector<VertexType>& vertices)
{
    static_assert(std::is_base_of<IVertexType, VertexType>::value, "Invalid vertex type used!");
    const UINT vertexBufferSize = static_cast<UINT>(vertices.size() * sizeof(VertexType));

    // Note: using upload heaps to transfer static data like vert buffers is not 
    // recommended. Every time the GPU needs it, the upload heap will be marshalled 
    // over. Please read up on Default Heap usage. An upload heap is used here for 
    // code simplicity and because there are very few verts to actually transfer.
    CD3DX12_HEAP_PROPERTIES heapProps(D3D12_HEAP_TYPE_UPLOAD);
    CD3DX12_RESOURCE_DESC resourceDesc = CD3DX12_RESOURCE_DESC::Buffer(vertexBufferSize);

    ThrowIfFailed(device->CreateCommittedResource(
        &heapProps,
        D3D12_HEAP_FLAG_NONE,
        &resourceDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&m_vertexBuffer)));

    // Copy the vertex data into upload heap
    UINT8* pVertexDataBegin;
    CD3DX12_RANGE readRange(0, 0);        // We do not intend to read from this resource on the CPU.
    ThrowIfFailed(m_vertexBuffer->Map(0, &readRange, reinterpret_cast<void**>(&pVertexDataBegin)));
    memcpy(pVertexDataBegin, vertices.data(), vertexBufferSize);
    m_vertexBuffer->Unmap(0, nullptr);

    // Initialize the vertex buffer view.
    m_vertexBufferView.BufferLocation = m_vertexBuffer->GetGPUVirtualAddress();
    m_vertexBufferView.StrideInBytes = sizeof(VertexType);
    m_vertexBufferView.SizeInBytes = vertexBufferSize;
}

template VertexBuffer::VertexBuffer(ComPtr<ID3D12Device>, const std::vector<Vertex3D>&);
template VertexBuffer::VertexBuffer(ComPtr<ID3D12Device>, const std::vector<TexturedVertex3D>&);


D3D12_VERTEX_BUFFER_VIEW* VertexBuffer::GetVertexBufferViewPointer()
{
    return &m_vertexBufferView;
}
