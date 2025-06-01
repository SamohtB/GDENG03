#include "Quad.h"
#include "DeviceContext.h"



Quad::Quad(int id, String name, XMFLOAT2 offset) : AGameObject(id, name)
{
    float x = offset.x;
    float y = offset.y;

    m_indices =
    {
        0, 1, 2,
        2, 1, 3
    };

    this->m_indexBuffer = std::make_unique<IndexBuffer>(m_indices);

    m_vertices =
    {
        { { -0.25f + x,  0.25f + y, 0.0f }, { 0.0f, 1.0f, 0.0f } }, // 0
        { {  0.25f + x,  0.25f + y, 0.0f }, { 1.0f, 1.0f, 0.0f } }, // 1
        { { -0.25f + x, -0.25f + y, 0.0f }, { 1.0f, 0.0f, 1.0f } }, // 2
        { {  0.25f + x, -0.25f + y, 0.0f }, { 0.0f, 1.0f, 1.0f } }  // 3
    };

    this->m_vertexBuffer = std::make_unique<VertexBuffer>(m_vertices);
}

void Quad::Update()
{
}

void Quad::Draw(DeviceContext* context)
{
    context->SetVertexBuffer(this->m_vertexBuffer->GetVertexBufferViewPointer());
    context->SetIndexBuffer(this->m_indexBuffer->GetIndexBufferViewPointer());

    context->DrawIndexedTriangleList(static_cast<UINT>(m_indices.size()), 0, 0);
}
