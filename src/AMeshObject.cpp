#include "AMeshObject.h"

#include "DeviceContext.h"
#include "RenderSystem.h"
#include "RenderDevice.h"
#include "GraphicsEngine.h"
#include "GameObjectManager.h"
#include "TextureManager.h"
#include "MaterialManager.h"

AMeshObject::AMeshObject(String name, String shader, String material) : AGameObject(name), m_shader(shader), m_material(material) {}

void AMeshObject::Update(float deltaTime)
{
	this->OnUpdate(deltaTime);

    ObjectConstantsData objData = {};
    objData.modelMatrix = this->GetLocalMatrix();
    objData.objectId = this->GetId();

    GameObjectManager::GetInstance()->UpdateConstantBuffer(this->GetId(), objData);
}

void AMeshObject::Draw(DeviceContext* context)
{
    auto renderSystem = GraphicsEngine::GetInstance()->GetRenderSystem();
    auto frameIndex = renderSystem->GetCurrentFrameIndex();

    context->SetPSO(renderSystem->GetPipelineState(this->m_shader));
    context->SetObjectConstants(GameObjectManager::GetInstance()->GetObjectConstantsAddress(this->GetId(), frameIndex));
    context->SetFrameConstants(renderSystem->GetFrameConstantsAddress());
    context->SetTexture(GraphicsEngine::GetInstance()->GetTextureManager()->GetSRVStart());
    context->SetMaterialConstants(GraphicsEngine::GetInstance()->GetMaterialManager()->GetMaterialHandle(this->m_material, frameIndex));

    context->SetVertexBuffer(this->m_vertexBuffer->GetVertexBufferViewPointer());
    context->SetIndexBuffer(this->m_indexBuffer->GetIndexBufferViewPointer());

	context->SetTopology(this->m_topology);
    context->DrawIndexedTriangleStrip(m_indicesSize, 0, 0);
}

void AMeshObject::SetTopology(D3D12_PRIMITIVE_TOPOLOGY topology)
{
	this->m_topology = topology;
}

void AMeshObject::SetGeometry(std::vector<Vertex> vertices, std::vector<unsigned int> indices)
{
    m_vertexBuffer = std::make_unique<VertexBuffer>(vertices);
    m_indexBuffer = std::make_unique<IndexBuffer>(indices);
    m_indicesSize = static_cast<UINT>(indices.size());
}