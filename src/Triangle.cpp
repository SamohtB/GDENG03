#include "Triangle.h"
#include "VertexBuffer.h"
#include "GraphicsEngine.h"

Triangle::Triangle(int id, String name, ComPtr<ID3D12Device> device) : AGameObject(id, name)
{
    m_vertices =
    {
        { { 0.0f, 0.5f, 0.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } },
        { { 0.5f, -0.5f, 0.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
        { { -0.5f, -0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } }
    };

    this->m_vertexBuffer = std::make_unique<VertexBuffer>(device, m_vertices);
}

void Triangle::Update()
{
}

void Triangle::Draw(ID3D12GraphicsCommandList* cmdList)
{
    cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    cmdList->IASetVertexBuffers(0, 1, this->m_vertexBuffer->GetVertexBufferViewPointer());
    cmdList->DrawInstanced(static_cast<UINT>(m_vertices.size()), 1, 0, 0);
}
