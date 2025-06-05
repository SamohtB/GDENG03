#include "MaterialManager.h"
#include "ConstantBuffer.h"
#include "Debug.h"

MaterialManager::MaterialManager(std::shared_ptr<DescriptorHeapManager> heapManager, std::shared_ptr<BatchUploader> uploader) 
	: m_heapManager(heapManager), m_uploader(uploader)
{
}

void MaterialManager::LoadInitialMaterials()
{
    LoadMaterial(MaterialType::DEFAULT_MAT, rock_mat_0);
    Debug::Log("Default Material Loaded");
    LoadMaterial(MaterialType::ROCK, rock_mat_2);
    Debug::Log("Rock Material Loaded");
    LoadMaterial(MaterialType::METAL_PLATE, metal_plate_mat_2);
    Debug::Log("Metal Plate Material Loaded");
}

void MaterialManager::LoadMaterial(const MaterialType& type, const MaterialDescription& description)
{
    std::vector<ComPtr<ID3D12Resource>> cbvBuffers;
    std::array<UINT, FRAME_COUNT> cbvIndices{};

    for (int i = 0; i < FRAME_COUNT; i++)
    {
        UINT cbvIndex = m_heapManager->AllocateCBVSlot();
        auto cbvHandle = m_heapManager->GetShaderVisibleCPUHandleAt(cbvIndex);

        cbvBuffers.push_back(m_uploader->CreateCBV(cbvHandle));
        cbvIndices[i] = cbvIndex;
    }
    

    MaterialPtr material = std::make_unique<Material>(cbvBuffers, description);
    this->m_cbvMap[type] = cbvIndices;
    this->m_materialMap[type] = std::move(material);
}

D3D12_GPU_DESCRIPTOR_HANDLE MaterialManager::GetMaterialHandle(const MaterialType& type, UINT frameIndex)
{
    UINT index = this->m_cbvMap[type][frameIndex];
    return m_heapManager->GetShaderVisibleGPUHandleAt(index);
}