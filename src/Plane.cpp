#include "Plane.h"
#include "DeviceContext.h"
#include "RenderSystem.h"
#include "RenderDevice.h"
#include "GraphicsEngine.h"
#include "GameObjectManager.h"

Plane::Plane(String name, Vector3 color) : AGameObject(name)
{
    std::vector<unsigned int> indices =
    {
        0, 1, 2,
        0, 2, 3,

        2, 1, 0,
        3, 2, 0
    };

    this->m_indexBuffer = std::make_unique<IndexBuffer>(indices);
    this->m_indicesSize = static_cast<UINT>(indices.size());

    std::vector<POS_COL> vertices = {
        {{-5.0f,  0.0f, -5.0f}, color},
        {{-5.0f,  0.0f,  5.0f}, color},
        {{ 5.0f,  0.0f,  5.0f}, color},
        {{ 5.0f,  0.0f, -5.0f}, color}
    };

    this->m_vertexBuffer = std::make_unique<VertexBuffer>(vertices);
    this->m_shader = ShaderType::DEFAULT_SHADER;
}

void Plane::Update(float deltaTime)
{
    /* Constant Buffer Stuff */
    ObjectConstantsData objData = {};
    objData.modelMatrix = this->GetLocalMatrix();
    objData.objectId = this->GetId();

    GameObjectManager::GetInstance()->UpdateConstantBuffer(this->GetId(), objData);
}

void Plane::Draw(DeviceContext* context)
{
    auto renderSystem = GraphicsEngine::GetInstance()->GetRenderSystem();
    auto frameIndex = renderSystem->GetCurrentFrameIndex();

    /* Root Parameters must be set after setting PSO */
    {
        context->SetPSO(renderSystem->GetPipelineState(this->m_shader));
        context->SetObjectConstants(GameObjectManager::GetInstance()->GetObjectConstantsAddress(this->GetId(), frameIndex));
        context->SetFrameConstants(renderSystem->GetFrameConstantsAddress());
        /* Set Textures And Mats here */
    }

    context->SetVertexBuffer(this->m_vertexBuffer->GetVertexBufferViewPointer());
    context->SetIndexBuffer(this->m_indexBuffer->GetIndexBufferViewPointer());

    context->DrawIndexedTriangleList(m_indicesSize, 0, 0);
}
