#include "MaterialManager.h"
#include "ConstantBuffer.h"
#include "Debug.h"

MaterialManager::MaterialManager(std::shared_ptr<DescriptorHeapManager> heapManager, std::shared_ptr<BatchUploader> uploader) 
	: m_heapManager(heapManager), m_uploader(uploader)
{
}

void MaterialManager::LoadInitialMaterials()
{
    LoadMaterial(MaterialType::DEFAULT_MAT, MaterialData::Materials.at(MaterialType::DEFAULT_MAT));
    Debug::Log("DEFAULT_MAT Material Loaded");

    LoadMaterial(MaterialType::ROCK_0, MaterialData::Materials.at(MaterialType::ROCK_0));
    Debug::Log("ROCK_0 Material Loaded");

    LoadMaterial(MaterialType::ROCK_1, MaterialData::Materials.at(MaterialType::ROCK_1));
    Debug::Log("ROCK_1 Material Loaded");

    LoadMaterial(MaterialType::METAL_PLATE_0, MaterialData::Materials.at(MaterialType::METAL_PLATE_0));
    Debug::Log("METAL_PLATE_0 Material Loaded");

    LoadMaterial(MaterialType::METAL_PLATE_1, MaterialData::Materials.at(MaterialType::METAL_PLATE_1));
    Debug::Log("METAL_PLATE_1 Material Loaded");

    LoadMaterial(MaterialType::BRICKS_0, MaterialData::Materials.at(MaterialType::BRICKS_0));
    Debug::Log("BRICKS_0 Material Loaded");

    LoadMaterial(MaterialType::BRICKS_1, MaterialData::Materials.at(MaterialType::BRICKS_1));
    Debug::Log("BRICKS_1 Material Loaded");
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