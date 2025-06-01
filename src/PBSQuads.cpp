#include "PBSQuads.h"
#include "DeviceContext.h"
#include "RenderDevice.h"
#include "GraphicsEngine.h"
#include "TextureManager.h"
#include "GeoMath.h"

PBSQuads::PBSQuads(int id, String name, Vector2 offset) : AGameObject(id, name)
{
    float x = offset.x;
    float y = offset.y;

    float aspectRatio = 1024.0f / 768.0f;

    m_indices =
    {
        0, 1, 2,
        2, 1, 3
    };

    this->m_indexBuffer = std::make_unique<IndexBuffer>(m_indices);

    std::vector<Vector3> position = {
        { -0.25f + x * aspectRatio,  0.25f + y, 0.0f },
        {  0.25f + x * aspectRatio,  0.25f + y, 0.0f },
        { -0.25f + x * aspectRatio, -0.25f + y, 0.0f },
        {  0.25f + x * aspectRatio, -0.25f + y, 0.0f }
    };

    std::vector<Vector2> texcoords = {
        { 0.0f, 0.0f },
        { 1.0f, 0.0f },
        { 0.0f, 1.0f },
        { 1.0f, 1.0f },
    };

    std::vector<Vector3> normals =
    {
        { 0.0f, 0.0f, 1.0f },
        { 0.0f, 0.0f, 1.0f },
        { 0.0f, 0.0f, 1.0f },
        { 0.0f, 0.0f, 1.0f }
    };

    std::vector<Vector3> tangents(4);
    std::vector<Vector3> bitangents(4);

    GeoMath::CalculateTangentFrame(m_indices, position.data(), normals.data(), texcoords.data(),
        position.size(), tangents.data(), bitangents.data());

    for (size_t i = 0; i < position.size(); ++i)
    {
        m_vertices.emplace_back(
            position[i],
            texcoords[i],
            normals[i],
            tangents[i],
            bitangents[i]
        );
    }

    this->m_vertexBuffer = std::make_unique<VertexBuffer>(m_vertices);

    this->m_shader = ShaderType::PBS;
    this->m_texture = TextureType::COLOR;
    this->m_normal = TextureType::NORMAL;
}

void PBSQuads::Update()
{
}

void PBSQuads::Draw(DeviceContext* context)
{
    auto renderSystem = GraphicsEngine::GetInstance()->GetRenderSystem();
    auto textureManager = GraphicsEngine::GetInstance()->GetTextureManager();

    /* Check Render System Start Frame {} for other settables */
    {
        context->SetPSO(renderSystem->GetPipelineState(this->m_shader));
        context->SetTexture(0, textureManager->GetTextureHandle(this->m_texture));
        context->SetTexture(1, textureManager->GetTextureHandle(this->m_normal));
        context->SetTexture(2, textureManager->GetTextureHandle(TextureType::ROUGH));
    }

    context->SetVertexBuffer(this->m_vertexBuffer->GetVertexBufferViewPointer());
    context->SetIndexBuffer(this->m_indexBuffer->GetIndexBufferViewPointer());

    context->DrawIndexedTriangleList(static_cast<UINT>(m_indices.size()), 0, 0);
}
