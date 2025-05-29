#include "Triangle.h"
#include "GraphicsEngine.h"
#include <iostream>

Triangle::Triangle(int id, String name, XMFLOAT2 offset, std::vector<XMFLOAT4> colors) : AGameObject(id, name)
{
    float x = offset.x;
    float y = offset.y;

    m_vertices =
    {
        { {  0.0f + x,  0.25f + y, 0.0f }, { 0.5f, 0.0f }, colors[0] },
        { {  0.25f + x, -0.25f + y, 0.0f }, { 1.0f, 1.0f }, colors[1] },
        { { -0.25f + x, -0.25f + y, 0.0f }, { 0.0f, 1.0f }, colors[2] },
    };

    /* POS_COLOR */
    //m_vertices = 
    //{
    //    { XMFLOAT3(0.0f, 0.5f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
    //    { XMFLOAT3(0.5f, -0.5f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
    //    { XMFLOAT3(-0.5f, -0.5f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) }

    this->m_vertexBuffer = std::make_unique<VertexBuffer>(m_vertices);
}

void Triangle::Update()
{
}

void Triangle::Draw(ID3D12GraphicsCommandList* cmdList)
{
    auto handle = TextureManager::GetInstance()->GetSRVHandle(TextureManager::TextureType::CRATE);
    cmdList->SetGraphicsRootDescriptorTable(0, handle);

    /* Actual Draw Calls */
    cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    cmdList->IASetVertexBuffers(0, 1, this->m_vertexBuffer->GetVertexBufferViewPointer());
    cmdList->DrawInstanced(static_cast<UINT>(m_vertices.size()), 1, 0, 0);
}
