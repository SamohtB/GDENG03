#include "pch.h"
#include "MeshComponent.h"

#include "GameObjectManager.h"
#include "GraphicsEngine.h"

#include "DeviceContext.h"
#include "RenderSystem.h"
#include "RenderDevice.h"

#include "TextureManager.h"
#include "MaterialManager.h"
#include "LightManager.h"
#include "MeshManager.h"

#include "EngineGUIManager.h"
#include "AUIScreen.h"
#include "MaterialEditor.h"

#include "Debug.h"

MeshComponent::MeshComponent(String name, String mesh, std::weak_ptr<AGameObject> owner) 
	: AComponent(name, ComponentType::Renderer, owner), m_mesh(mesh), m_context(nullptr)
{
	m_material = MaterialType::DEFAULT;
	m_gpuAddresses.resize(FRAME_COUNT);
	m_topology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
}

void MeshComponent::SetDeviceContext(DeviceContext* context)
{
	this->m_context = context;
}

void MeshComponent::Perform()
{
    auto renderSystem = GraphicsEngine::GetInstance()->GetRenderSystem();
    auto frameIndex = renderSystem->GetCurrentFrameIndex();
    auto shader = GraphicsEngine::GetInstance()->GetMaterialManager()->GetMaterialShader(this->m_material, frameIndex);
    auto meshData = GraphicsEngine::GetInstance()->GetMeshManager()->GetMeshData(this->m_mesh);

    m_context->SetPSO(renderSystem->GetPipelineState(shader));
    m_context->SetObjectConstants(this->m_gpuAddresses[frameIndex]);
    m_context->SetFrameConstants(renderSystem->GetFrameConstantsAddress());
    m_context->SetTexture(GraphicsEngine::GetInstance()->GetTextureManager()->GetSRVStart());
    m_context->SetMaterialConstants(GraphicsEngine::GetInstance()->GetMaterialManager()->GetMaterialHandle(this->m_material, frameIndex));
    m_context->SetLightConstants(GraphicsEngine::GetInstance()->GetLightManager()->GetLightBuffer(frameIndex));

    m_context->SetVertexBuffer(meshData->m_vertexBuffer->GetVertexBufferViewPointer());
    m_context->SetIndexBuffer(meshData->m_indexBuffer->GetIndexBufferViewPointer());

    m_context->SetTopology(this->m_topology);
    m_context->DrawIndexedTriangleStrip(meshData->m_indicesSize, 0, 0);
}

void MeshComponent::DrawUI()
{
    if (ImGui::CollapsingHeader("Mesh Renderer", ImGuiTreeNodeFlags_DefaultOpen))
    {
        static int selectedMeshIndex = 0;
        static int selectedMaterialIndex = 0;

		auto meshManager = GraphicsEngine::GetInstance()->GetMeshManager();
        auto materialManager = GraphicsEngine::GetInstance()->GetMaterialManager();

        // === Mesh Dropdown ===
        const auto& meshNames = meshManager->GetAllMeshNames();

        for (int i = 0; i < meshNames.size(); ++i)
        {
            if (this->m_mesh == meshNames[i])
            {
                selectedMeshIndex = i;
                break;
            }
        }

        if (!meshNames.empty()) 
        {
            if (ImGui::Combo("Mesh", &selectedMeshIndex, meshNames.data(), static_cast<int>(meshNames.size()))) 
            {
                m_mesh = meshNames[selectedMeshIndex];
            }
        }

        // === Material Dropdown ===
        const auto& materialNames = materialManager->GetAllMaterialNames();

        for (int i = 0; i < materialNames.size(); ++i)
        {
            if (this->m_material == materialNames[i])
            {
                selectedMaterialIndex = i;
                break;
            }
        }

        if (!materialNames.empty()) 
        {
            if (ImGui::Combo("Material", &selectedMaterialIndex, materialNames.data(), static_cast<int>(materialNames.size()))) 
            {
                m_material = materialNames[selectedMaterialIndex];
            }

            if (ImGui::Button("Edit Material"))
            {
                auto materialEditorBase = EngineGUIManager::GetInstance()->GetUI(UINames::MATERIAL_EDITOR);
                auto* materialEditor = static_cast<MaterialEditor*>(materialEditorBase);

                if (materialEditor)
                {
                    materialEditor->SetVisible(true);
                    materialEditor->SetCurrentMaterial(this->m_material);
                }
            }
        }

        // === Detach Component Button ===
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        if (ImGui::Button("Delete Mesh Component"))
        {
			this->DetachSelfFromOwner();
        }
    }
}

void MeshComponent::SetMaterial(String material)
{
    if (material != this->m_material)
    {
        this->m_material = material;
    }
}

String MeshComponent::GetMaterial() const
{
    return this->m_material;
}

void MeshComponent::SetGPUAddress(UINT frameIndex, D3D12_GPU_VIRTUAL_ADDRESS address)
{
    Debug::Assert(frameIndex < FRAME_COUNT, "Frame index out of bounds in MeshComponent::SetGPUAddress");
    this->m_gpuAddresses[frameIndex] = address;
}

void MeshComponent::SetTopology(D3D12_PRIMITIVE_TOPOLOGY topology)
{
    this->m_topology = topology;
}

String MeshComponent::GetMeshType() const
{
    return this->m_mesh;
}