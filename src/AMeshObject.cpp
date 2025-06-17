#include "AMeshObject.h"

#include "DeviceContext.h"
#include "RenderSystem.h"
#include "RenderDevice.h"
#include "GraphicsEngine.h"
#include "GameObjectManager.h"

AMeshObject::AMeshObject(String name, Vector3 color, ShaderType shader) : AGameObject(name), m_shader(shader), m_color(color) {}

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

    context->SetVertexBuffer(this->m_vertexBuffer->GetVertexBufferViewPointer());
    context->SetIndexBuffer(this->m_indexBuffer->GetIndexBufferViewPointer());

	context->SetTopology(this->m_topology);
    context->DrawIndexedTriangleStrip(m_indicesSize, 0, 0);
}

void AMeshObject::SetTopology(D3D12_PRIMITIVE_TOPOLOGY topology)
{
	this->m_topology = topology;
}

template<typename T>
void AMeshObject::SetGeometry(std::vector<T> vertices, std::vector<unsigned int> indices)
{
    m_vertexBuffer = std::make_unique<VertexBuffer>(vertices);
    m_indexBuffer = std::make_unique<IndexBuffer>(indices);
    m_indicesSize = static_cast<UINT>(indices.size());
}

/* Add Other Preset Input Layouts */
template void AMeshObject::SetGeometry(std::vector<POS_COL>, std::vector<unsigned int>);