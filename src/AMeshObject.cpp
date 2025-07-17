#include "pch.h"
#include "AMeshObject.h"

#include "GameObjectManager.h"

#include "GraphicsEngine.h"

#include "DeviceContext.h"
#include "RenderSystem.h"
#include "RenderDevice.h"

#include "TextureManager.h"
#include "MaterialManager.h"
#include "LightManager.h"
#include "MeshManager.h"

#include "Debug.h"

AMeshObject::AMeshObject(String name, String mesh, String material) : AGameObject(name), m_mesh(mesh), m_material(material)
{
    m_gpuAddresses.resize(FRAME_COUNT);
}

void AMeshObject::Update(float deltaTime)
{
}

void AMeshObject::Draw(DeviceContext* context)
{
    auto renderSystem = GraphicsEngine::GetInstance()->GetRenderSystem();
    auto frameIndex = renderSystem->GetCurrentFrameIndex();
    auto shader = GraphicsEngine::GetInstance()->GetMaterialManager()->GetMaterialShader(this->m_material, frameIndex);
	auto meshData = GraphicsEngine::GetInstance()->GetMeshManager()->GetMeshData(this->m_mesh);

    context->SetPSO(renderSystem->GetPipelineState(shader));
    context->SetObjectConstants(this->m_gpuAddresses[frameIndex]);
    context->SetFrameConstants(renderSystem->GetFrameConstantsAddress());
    context->SetTexture(GraphicsEngine::GetInstance()->GetTextureManager()->GetSRVStart());
    context->SetMaterialConstants(GraphicsEngine::GetInstance()->GetMaterialManager()->GetMaterialHandle(this->m_material, frameIndex));
    context->SetLightConstants(GraphicsEngine::GetInstance()->GetLightManager()->GetLightBuffer(frameIndex));

    context->SetVertexBuffer(meshData->m_vertexBuffer->GetVertexBufferViewPointer());
    context->SetIndexBuffer(meshData->m_indexBuffer->GetIndexBufferViewPointer());

    context->SetTopology(this->m_topology);
    context->DrawIndexedTriangleStrip(meshData->m_indicesSize, 0, 0);
}

void AMeshObject::SetMaterial(String material)
{
    if (material != this->m_material)
    {
        this->m_material = material;
    }
}

String AMeshObject::GetMaterial() const
{
    return this->m_material;
}

void AMeshObject::SetGPUAddress(UINT frameIndex, D3D12_GPU_VIRTUAL_ADDRESS address)
{
    Debug::Assert(frameIndex < FRAME_COUNT, "Frame index out of bounds in AMeshObject::SetGPUAddress");
    this->m_gpuAddresses[frameIndex] = address;
}

void AMeshObject::SetTopology(D3D12_PRIMITIVE_TOPOLOGY topology)
{
    this->m_topology = topology;
}