#include "AnimatedQuad.h"
#include "GraphicsEngine.h"
#include "DeviceContext.h"

AnimatedQuad::AnimatedQuad(int id, String name, Vector2 offset) : AGameObject(id, name)
{
    std::vector<unsigned int> indices =
    {
        0, 1, 2,
        2, 1, 3
    };

    this->m_indexBuffer = std::make_unique<IndexBuffer>(indices);
    this->m_indicesSize = static_cast<UINT>(indices.size());

    /* vertices for Speed Up / Speed Down */
    std::vector<POS_POS_COL_COL> vertices =
    {
        { { -0.8f,  0.2f, 0.0f }, { -0.11f,  0.78f, 0.0f }, { 1.0f, 1.0f, 0.0f }, { 1.0f, 1.0f, 0.0f } },
        { {  0.0f,  0.2f, 0.0f }, {  0.88f,  0.77f, 0.0f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 1.0f } },
        { { -0.70f, -0.9f, 0.0f }, { -0.32f, -0.11f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f } },
        { {  0.3f, -0.3f, 0.0f }, {  0.75f, -0.73f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f, 0.0f } },
    };

    /* vertices for folding animation */
    //std::vector<POS_POS_COL_COL> vertices =
    //{
    //    { { -0.8f,  -1.0f, 0.0f }, { -0.5f,  -0.5f, 0.0f }, { 1.0f, 1.0f, 0.0f }, { 1.0f, 1.0f, 0.0f } },
    //    { { -1.0f,  0.0f, 0.0f }, {  -0.1f,  0.7f, 0.0f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 1.0f } },
    //    { {  1.0f, -0.2f, 0.0f }, { 0.2f, -0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f, 0.0f } },
    //    { { -0.8f, -1.0f, 0.0f }, {  0.7f, 0.7f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f } },
    //};

    this->m_vertexBuffer = std::make_unique<VertexBuffer>(vertices);
}

void AnimatedQuad::Update()
{
}

void AnimatedQuad::Draw(DeviceContext* context)
{
    auto renderSystem = GraphicsEngine::GetInstance()->GetRenderSystem();

    /* Check Render System Start Frame {} for other settables */
    {
        context->SetPSO(renderSystem->GetPipelineState(this->m_shader));
        context->SetGlobalConstantBuffer(renderSystem->GetGlobalBufferAddress()); // needs to be set after setting PSO
    }

    context->SetVertexBuffer(this->m_vertexBuffer->GetVertexBufferViewPointer());
    context->SetIndexBuffer(this->m_indexBuffer->GetIndexBufferViewPointer());

    context->DrawIndexedTriangleList(static_cast<UINT>(m_indicesSize), 0, 0);
}
