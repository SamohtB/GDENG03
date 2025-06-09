#include "Cube.h"
#include "DeviceContext.h"
#include "RenderDevice.h"
#include "GraphicsEngine.h"
#include "GameObjectManager.h"
#include "Random.h"

Cube::Cube(String name) : AGameObject(name)
{
    std::vector<unsigned int> indices =
    {
        0, 1, 2,
        0, 2, 3,

        4, 5, 6,
        4, 6, 7,

        3, 2, 5,
        3, 5, 4,

        7, 6, 1,
        7, 1, 0,

        1, 6, 5,
        1, 5, 2,

        7, 0, 3,
        7, 3, 4
    };

    this->m_indexBuffer = std::make_unique<IndexBuffer>(indices);
    this->m_indicesSize = static_cast<UINT>(indices.size());

    std::vector<POS_COL> vertices = {

        {{-0.5f, -0.5f, -0.5f} , {1.0f, 0.0f, 0.0f}},
        {{-0.5f,  0.5f, -0.5f},  {1.0f, 1.0f, 0.0f}},
        {{ 0.5f,  0.5f, -0.5f},  {1.0f, 1.0f, 0.0f}},
        {{ 0.5f, -0.5f, -0.5f},  {1.0f, 0.0f, 0.0f}},

        {{ 0.5f, -0.5f, 0.5f},  {0.0f, 1.0f, 0.0f}},
        {{ 0.5f,  0.5f, 0.5f},  {0.0f, 1.0f, 1.0f}},
        {{-0.5f,  0.5f, 0.5f},  {0.0f, 0.0f, 1.0f}},
        {{-0.5f, -0.5f, 0.5f},  {1.0f, 1.0f, 1.0f}},
    };

    this->m_vertexBuffer = std::make_unique<VertexBuffer>(vertices);
    this->m_shader = ShaderType::DEFAULT_SHADER;
}

void Cube::Update(float deltaTime)
{
    m_ticks += deltaTime;

    float x = sinf(m_ticks * 0.7f + GetId());
    float y = cosf(m_ticks * 1.1f + GetId());
    float z = sinf(m_ticks * 0.3f + GetId() * 0.5f);

    Vector3 axis(x, y, z);
    axis.Normalize();

    this->Rotate(axis.x * deltaTime, axis.y * deltaTime, axis.z * deltaTime);

    ObjectConstantsData objData = {};
    objData.modelMatrix = this->GetLocalMatrix();
    objData.objectId = this->GetId();

    GameObjectManager::GetInstance()->UpdateConstantBuffer(this->GetId(), objData);
}

void Cube::Draw(DeviceContext* context)
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

void Cube::SetRandomRotation()
{
    float pitch = Random::Range(0.0f, DirectX::XM_2PI); 
    float yaw = Random::Range(0.0f, DirectX::XM_2PI);
    float roll = Random::Range(0.0f, DirectX::XM_2PI);

    this->SetRotation(pitch, yaw, roll);
}
