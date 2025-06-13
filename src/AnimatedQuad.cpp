#include "AnimatedQuad.h"
#include "GraphicsEngine.h"
#include "RenderSystem.h"
#include "DeviceContext.h"
#include "RenderDevice.h"
#include "GameObjectManager.h"

AnimatedQuad::AnimatedQuad(String name) : AGameObject(name)
{
    std::vector<unsigned int> indices =
    {
        0, 1, 2,
        2, 1, 3
    };

    this->m_indexBuffer = std::make_unique<IndexBuffer>(indices);
    this->m_indicesSize = static_cast<UINT>(indices.size());

    /* vertices for Speed Up / Speed Down */
    //std::vector<POS_POS_COL_COL> vertices =
    //{
    //    { { -0.8f,  0.2f, 0.0f }, { -0.11f,  0.78f, 0.0f }, { 1.0f, 1.0f, 0.0f }, { 1.0f, 1.0f, 0.0f } },
    //    { {  0.0f,  0.2f, 0.0f }, {  0.88f,  0.77f, 0.0f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 1.0f } },
    //    { { -0.70f, -0.9f, 0.0f }, { -0.32f, -0.11f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f } },
    //    { {  0.3f, -0.3f, 0.0f }, {  0.75f, -0.73f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f, 0.0f } },
    //};

    /* vertices for folding animation */
    std::vector<POS_POS_COL_COL> vertices =
    {
         {{-0.67f, -0.89f, 0.0f}, { -0.31f, -0.09f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
         {{-0.88f,  0.22f, 0.0f}, { -0.01f,  0.78f, 0.0f}, {1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 0.0f}},
         {{ 1.00f, -0.28f, 0.0f}, {  0.01f, -0.71f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f}},
         {{-0.67f, -0.89f, 0.0f}, {  0.86f,  0.75f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}},
    };

    this->m_vertexBuffer = std::make_unique<VertexBuffer>(vertices);
}

void AnimatedQuad::Update(float deltaTime)
{
}

void AnimatedQuad::Draw(DeviceContext* context)
{
    auto renderSystem = GraphicsEngine::GetInstance()->GetRenderSystem();
    auto frameIndex = renderSystem->GetCurrentFrameIndex();

    /* Check Render System Start Frame {} for other settables */
    {
        context->SetPSO(renderSystem->GetPipelineState(this->m_shader));
        context->SetObjectConstants(GameObjectManager::GetInstance()->GetObjectConstantsAddress(this->GetId(), frameIndex));
    }

    context->SetVertexBuffer(this->m_vertexBuffer->GetVertexBufferViewPointer());
    context->SetIndexBuffer(this->m_indexBuffer->GetIndexBufferViewPointer());

    context->DrawIndexedTriangleList(static_cast<UINT>(m_indicesSize), 0, 0);
}
