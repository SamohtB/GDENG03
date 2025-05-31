#include "VertexBuffer.h"
#include "VertexTypes.h"
#include "GraphicsEngine.h"
#include "BatchUploader.h"

template <typename VertexType>
VertexBuffer::VertexBuffer(const std::vector<VertexType>& vertices)
{
    static_assert(std::is_base_of<IVertexType, VertexType>::value, "Invalid vertex type used!");

    VertexBufferInfo info = GraphicsEngine::GetInstance()->GetBatchUploader()->SchedVertexBuffer(vertices);

    this->m_vertexBuffer = info.buffer;
    this->m_vertexBufferView = info.view;
}

template VertexBuffer::VertexBuffer(const std::vector<POS_COL>&);
template VertexBuffer::VertexBuffer(const std::vector<POS_TEX_COL>&);
template VertexBuffer::VertexBuffer(const std::vector<POS_POS_COL_COL>&);


D3D12_VERTEX_BUFFER_VIEW* VertexBuffer::GetVertexBufferViewPointer()
{
    return &m_vertexBufferView;
}
