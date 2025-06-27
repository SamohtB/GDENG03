#include "AMeshObject.h"

#include "GraphicsEngine.h"
#include "TextureManager.h"
#include "MaterialManager.h"
#include "DeviceContext.h"
#include "RenderSystem.h"

#include "GameObjectManager.h"

AMeshObject::AMeshObject(String name, String shaderName) 
    : AGameObject(name), m_shaderName(shaderName), m_indicesSize(0), m_topology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST) {}

void AMeshObject::Update(float deltaTime)
{
}

void AMeshObject::Draw(DeviceContext* context)
{
    auto graphicsEngine = GraphicsEngine::GetInstance();
    auto renderSystem = graphicsEngine->GetRenderSystem();
    auto materialManager = graphicsEngine->GetMaterialManager();
    auto frameIndex = renderSystem->GetCurrentFrameIndex();

    context->SetPSO(renderSystem->GetPipelineState(this->m_shaderName));
    context->SetTexture(graphicsEngine->GetTextureManager()->GetSRVStart());
    context->SetObjectConstants(this->m_constantBuffer->GetGPUVirtualAddress(frameIndex));
    context->SetFrameConstants(renderSystem->GetFrameConstantsAddress());
    context->SetMaterialConstants(materialManager->GetMaterialDataAddress(this->m_materialName, frameIndex));

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