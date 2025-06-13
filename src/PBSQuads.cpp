#include "PBSQuads.h"
#include "DeviceContext.h"
#include "RenderSystem.h"
#include "RenderDevice.h"
#include "GraphicsEngine.h"
#include "TextureManager.h"
#include "MaterialManager.h"
#include "Math.h"
#include "Debug.h"

PBSQuads::PBSQuads(String name, Vector2 offset, MaterialType matType) : AGameObject(name)
{
    float x = offset.x;
    float y = offset.y;

    float aspectRatio = 1024.0f / 768.0f;

    std::vector<unsigned int> indices =
    {
        0, 1, 2,
        2, 1, 3
    };

    this->m_indexBuffer = std::make_unique<IndexBuffer>(indices);
    this->m_indicesSize = static_cast<UINT>(indices.size());

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

    GeoMath::CalculateTangentFrame(indices, position.data(), normals.data(), texcoords.data(),
        position.size(), tangents.data(), bitangents.data());

    std::vector<POS_TEX_NOR_TAN_BIT> vertices;

    for (size_t i = 0; i < position.size(); ++i)
    {
        vertices.emplace_back(
            position[i],
            texcoords[i],
            normals[i],
            tangents[i],
            bitangents[i]
        );
    }

    this->m_vertexBuffer = std::make_unique<VertexBuffer>(vertices);

    this->m_shader = ShaderType::PBS;
    this->m_material = matType;
}

void PBSQuads::Update(float deltaTime)
{
}

void PBSQuads::Draw(DeviceContext* context)
{
    auto graphicsEngine = GraphicsEngine::GetInstance();
    auto renderSystem = graphicsEngine->GetRenderSystem();

    /* Check Render System Start Frame {} for other settables */
    {
        context->SetPSO(renderSystem->GetPipelineState(this->m_shader));
        context->SetTexture(graphicsEngine->GetTextureManager()->GetSRVStart());
        //context->SetMaterialConstants(graphicsEngine->GetMaterialConstantsAddress(this->m_material));
    }

    context->SetVertexBuffer(this->m_vertexBuffer->GetVertexBufferViewPointer());
    context->SetIndexBuffer(this->m_indexBuffer->GetIndexBufferViewPointer());

    context->DrawIndexedTriangleList(m_indicesSize, 0, 0);
}
