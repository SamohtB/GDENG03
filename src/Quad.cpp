#include "Quad.h"
#include "DeviceContext.h"
#include "RenderDevice.h"
#include "GraphicsEngine.h"
#include "TextureManager.h"

Quad::Quad(int id, String name, XMFLOAT2 offset) : AGameObject(id, name)
{
    float x = offset.x;
    float y = offset.y;

    m_indices =
    {
        0, 1, 2,
        2, 1, 3
    };

    this->m_indexBuffer = std::make_unique<IndexBuffer>(m_indices);

    m_vertices =
    {
        { { -0.25f + x,  0.25f + y, 0.0f }, { 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f } }, // 0
        { {  0.25f + x,  0.25f + y, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f, 0.0f } }, // 1
        { { -0.25f + x, -0.25f + y, 0.0f }, { 0.0f, 1.0f }, { 1.0f, 0.0f, 1.0f } }, // 2
        { {  0.25f + x, -0.25f + y, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f, 1.0f } }  // 3
    };

    this->m_vertexBuffer = std::make_unique<VertexBuffer>(m_vertices);

    this->m_shader = ShaderType::TEXTURED;
    this->m_texture = TextureType::CRATE;
}

void Quad::Update()
{
}

void Quad::Draw(DeviceContext* context)
{
    

    auto renderSystem = GraphicsEngine::GetInstance()->GetRenderSystem();
    auto textureManager = GraphicsEngine::GetInstance()->GetTextureManager();

    /* Check Render System Start Frame {} for other settables */
    {
        context->SetPSO(renderSystem->GetPipelineState(this->m_shader));
        context->SetTexture(0, textureManager->GetTextureHandle(this->m_texture));
    }
    
    context->SetVertexBuffer(this->m_vertexBuffer->GetVertexBufferViewPointer());
    context->SetIndexBuffer(this->m_indexBuffer->GetIndexBufferViewPointer());

    context->DrawIndexedTriangleList(static_cast<UINT>(m_indices.size()), 0, 0);
}
