#include "AnimatedQuad.h"

AnimatedQuad::AnimatedQuad(int id, String name) : AGameObject(id, name)
{
    m_vertices =
    {
        // First Triangle
        { { -0.5f,  0.5f, 0.0f }, { -0.11f,  0.78f, 0.0f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f } },
        { {  0.5f,  0.5f, 0.0f }, { 0.88f,  0.77f, 0.0f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f } },
        { { -0.5f, -0.5f, 0.0f }, { -0.32f,  -0.11f, 0.0f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f } },

        // Second triangle
        { { -0.5f, -0.5f, 0.0f }, { -0.32f,  -0.11f, 0.0f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f } },
        { {  0.5f,  0.5f, 0.0f }, { 0.88f,  0.77f, 0.0f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f } },
        { {  0.5f, -0.5f, 0.0f }, { 0.75f,  -0.73f, 0.0f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f } },
    };

    this->m_vertexBuffer = std::make_unique<VertexBuffer>(m_vertices);
}

void AnimatedQuad::Update()
{
}

void AnimatedQuad::Draw(ID3D12GraphicsCommandList* cmdList)
{
    /* Actual Draw Calls */
    cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    cmdList->IASetVertexBuffers(0, 1, this->m_vertexBuffer->GetVertexBufferViewPointer());
    cmdList->DrawInstanced(static_cast<UINT>(m_vertices.size()), 1, 0, 0);
}
