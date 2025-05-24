#include "Triangle.h"
#include "GraphicsEngine.h"

Triangle::Triangle(int id, String name) : AGameObject(id, name)
{
    m_vertices =
    {
        { { 0.0f, 0.5f, 0.0f }, { 0.5f, 0.0f } , { 1.0f, 1.0f, 1.0f, 1.0f }},
        { { 0.5f, -0.5f, 0.0f }, { 1.0f, 1.0f } , { 1.0f, 1.0f, 1.0f, 1.0f }},
        { { -0.5f, -0.5f, 0.0f }, { 0.0f, 1.0f } , { 1.0f, 1.0f, 1.0f, 1.0f }}
    };

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
    if (this->m_texture != TextureType::UNSET)
    {
        auto handle = TextureManager::GetInstance()->GetSRVHandle(this->m_texture);
        cmdList->SetGraphicsRootDescriptorTable(0, // RootSig Param - set to enum later
            handle);
    }


    /* Actual Draw Calls */
    cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    cmdList->IASetVertexBuffers(0, 1, this->m_vertexBuffer->GetVertexBufferViewPointer());
    cmdList->DrawInstanced(static_cast<UINT>(m_vertices.size()), 1, 0, 0);
}
