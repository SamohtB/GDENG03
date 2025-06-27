#include "AMeshObject.h"

#include "GraphicsEngine.h"
#include "TextureManager.h"
#include "MaterialManager.h"
#include "DeviceContext.h"
#include "RenderSystem.h"

#include "GameObjectManager.h"

AMeshObject::AMeshObject(String name, String shaderName, String materialName)
    : AGameObject(name), m_shaderName(shaderName), m_materialName(materialName), m_indicesSize(0), m_topology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST) 
{
    this->m_constantBuffer = std::make_unique<ObjectConstantsBuffer>(GraphicsEngine::GetInstance()->GetRenderSystem()->GetD3DDevicePtr().Get());
}

void AMeshObject::Update(float deltaTime)
{
}

void AMeshObject::Draw(DeviceContext* context)
{
    auto graphicsEngine = GraphicsEngine::GetInstance();
    auto renderSystem = graphicsEngine->GetRenderSystem();
    auto materialManager = graphicsEngine->GetMaterialManager();

    /* Update Constant Buffers */
    ObjectConstantsData data;
    data.modelMatrix = this->GetLocalMatrix();
    this->m_constantBuffer->Update(data);

    context->SetPSO(renderSystem->GetPipelineState(this->m_shaderName));
    context->SetTexture(graphicsEngine->GetTextureManager()->GetSRVStart());
    context->SetObjectConstants(this->m_constantBuffer->GetGPUVirtualAddress());
    context->SetFrameConstants(renderSystem->GetFrameConstantsAddress());
    context->SetMaterialConstants(materialManager->GetMaterialDataAddress(this->m_materialName));

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