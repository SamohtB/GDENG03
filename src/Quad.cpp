#include "Quad.h"
#include "DeviceContext.h"
#include "RenderSystem.h"
#include "RenderDevice.h"
#include "GraphicsEngine.h"
#include "TextureManager.h"

Quad::Quad(String name, Vector2 offset) : AGameObject(name)
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

    m_vertices =
    {
        { { -0.25f + x * aspectRatio,  0.25f + y, 0.0f }, { 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f } }, // 0
        { {  0.25f + x * aspectRatio,  0.25f + y, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f, 0.0f } }, // 1
        { { -0.25f + x * aspectRatio, -0.25f + y, 0.0f }, { 0.0f, 1.0f }, { 1.0f, 0.0f, 1.0f } }, // 2
        { {  0.25f + x * aspectRatio, -0.25f + y, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f, 1.0f } }  // 3
    };

    this->m_vertexBuffer = std::make_unique<VertexBuffer>(m_vertices);

    this->m_shader = ShaderType::TEXTURED;
    this->m_texture = TextureType::ROCK_COLOR;
}

void Quad::Update(float deltaTime)
{
}

void Quad::Draw(DeviceContext* context)
{
    auto renderSystem = GraphicsEngine::GetInstance()->GetRenderSystem();

    {
        context->SetPSO(renderSystem->GetPipelineState(this->m_shader));
    }
    
    context->SetVertexBuffer(this->m_vertexBuffer->GetVertexBufferViewPointer());
    context->SetIndexBuffer(this->m_indexBuffer->GetIndexBufferViewPointer());

    context->DrawIndexedTriangleList(static_cast<UINT>(m_indices.size()), 0, 0);
}
