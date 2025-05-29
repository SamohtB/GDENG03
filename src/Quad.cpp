#include "Quad.h"
#include "TextureManager.h"

Quad::Quad(int id, String name, XMFLOAT2 offset) : AGameObject(id, name)
{
    float x = offset.x;
    float y = offset.y;

    m_vertices =
    {
        // First Triangle
        { { -0.25f + x,  0.25f + y, 0.0f }, { 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f } },
        { {  0.25f + x,  0.25f + y, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f, 1.0f } },
        { { -0.25f + x, -0.25f + y, 0.0f }, { 0.0f, 1.0f }, { 1.0f, 1.0f, 1.0f } },

        // Second triangle
        { { -0.25f + x, -0.25f + y, 0.0f }, { 0.0f, 1.0f }, { 1.0f, 1.0f, 1.0f } },
        { {  0.25f + x,  0.25f + y, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f, 1.0f } },
        { {  0.25f + x, -0.25f + y, 0.0f }, { 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f } }
    };

    this->m_vertexBuffer = std::make_unique<VertexBuffer>(m_vertices);
}

void Quad::Update()
{
}

void Quad::Draw(ID3D12GraphicsCommandList* cmdList)
{
    auto handle = TextureManager::GetInstance()->GetSRVHandle(TextureManager::TextureType::CRATE);
    cmdList->SetGraphicsRootDescriptorTable(0, handle);

    /* Actual Draw Calls */
    cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    cmdList->IASetVertexBuffers(0, 1, this->m_vertexBuffer->GetVertexBufferViewPointer());
    cmdList->DrawInstanced(static_cast<UINT>(m_vertices.size()), 1, 0, 0);
}
