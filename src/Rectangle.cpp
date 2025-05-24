#include "Rectangle.h"

Rectangle::Rectangle(int id, String name, XMFLOAT2 offset, std::vector<XMFLOAT4> colors) : AGameObject(id, name)
{
    float x = offset.x;
    float y = offset.y;

    m_vertices =
    {
        // First Triangle
        { { -0.25f + x,  0.25f + y, 0.0f }, { 0.0f, 0.0f }, colors[0] },
        { {  0.25f + x,  0.25f + y, 0.0f }, { 1.0f, 0.0f }, colors[1] },
        { { -0.25f + x, -0.25f + y, 0.0f }, { 0.0f, 1.0f }, colors[2] },

        // Second triangle
        { { -0.25f + x, -0.25f + y, 0.0f }, { 0.0f, 1.0f }, colors[2] },
        { {  0.25f + x,  0.25f + y, 0.0f }, { 1.0f, 0.0f }, colors[1] },
        { {  0.25f + x, -0.25f + y, 0.0f }, { 1.0f, 1.0f }, colors[3] }
    };

    this->m_vertexBuffer = std::make_unique<VertexBuffer>(m_vertices);
}

void Rectangle::Update()
{
}

void Rectangle::Draw(ID3D12GraphicsCommandList* cmdList)
{
    /* Actual Draw Calls */
    cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    cmdList->IASetVertexBuffers(0, 1, this->m_vertexBuffer->GetVertexBufferViewPointer());
    cmdList->DrawInstanced(static_cast<UINT>(m_vertices.size()), 1, 0, 0);
}
