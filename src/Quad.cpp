#include "Quad.h"
#include "TextureManager.h"

Quad::Quad(int id, String name, XMFLOAT2 offset) : AGameObject(id, name)
{
    float x = offset.x;
    float y = offset.y;

    m_indices =
    {
        0, 1, 2,
        2, 1, 3
    };

    m_vertices =
    {
        { { -0.25f + x,  0.25f + y, 0.0f }, { 0.0f, 1.0f, 0.0f } }, // 0
        { {  0.25f + x,  0.25f + y, 0.0f }, { 1.0f, 1.0f, 0.0f } }, // 1
        { { -0.25f + x, -0.25f + y, 0.0f }, { 1.0f, 0.0f, 1.0f } }, // 2
        { {  0.25f + x, -0.25f + y, 0.0f }, { 0.0f, 1.0f, 1.0f } }  // 3
    };

    this->m_vertexBuffer = std::make_unique<VertexBuffer>(m_vertices, m_indices);
}

void Quad::Update()
{
}

void Quad::Draw(DeviceContext* dvcContext)
{
    /* Actual Draw Calls */
    cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    cmdList->IASetVertexBuffers(0, 1, this->m_vertexBuffer->GetVertexBufferViewPointer());
    cmdList->IASetIndexBuffer(this->m_vertexBuffer->GetIndexBufferViewPointer());
    cmdList->DrawIndexedInstanced(static_cast<UINT>(this->m_indices.size()), 1, 0, 0, 0);
}
