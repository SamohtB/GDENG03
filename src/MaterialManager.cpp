#include "MaterialManager.h"
#include "Debug.h"

MaterialManager::MaterialManager(ID3D12Device* device)
{
    this->m_materialConstantsBuffer = std::make_unique<MaterialConstantsBuffer>(device, MAX_MATERIAL_COUNT * FRAME_COUNT);
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
    std::array<UINT, FRAME_COUNT> cbIndices{};

    MaterialConstantsData matData = {};
    matData.diffuseHandleIndex = description.albedoTextureIndex;
    matData.normalHandleIndex = description.normalTextureIndex;
    matData.normalStr = description.normalStr;
    matData.metalHandleIndex = description.metallicTextureIndex;
    matData.metalStr = description.metallicStr;
    matData.roughHandleIndex = description.roughTextureIndex;
    matData.roughStr = description.roughStr;
    matData.ambientOcclussionHandleIndex = description.ambientOcclusionTextureIndex;
    matData.ambientOcclussionStr = description.ambientOcclusionStr;
    matData.materialFlags = MaterialData::GetFlags(description);

    for (int i = 0; i < FRAME_COUNT; i++)
    {
        cbIndices[i] = ReserveSlot();
        m_materialConstantsBuffer->Update(matData, cbIndices[i]);
    }

    auto mat = std::make_unique<Material>(description, matData);

    this->m_cbMap[type] = cbIndices;
    this->m_materialMap[type] = std::move(mat);
}

void MaterialManager::UpdateMaterialConstants(const MaterialType& type, UINT frameIndex)
{
    UINT cbIndex = this->m_cbMap[type][frameIndex];
    auto data = this->m_materialMap[type]->GetMaterialData();
    m_materialConstantsBuffer->Update(data, cbIndex);
}

D3D12_GPU_VIRTUAL_ADDRESS MaterialManager::GetMaterialConstantsAddress(MaterialType type, UINT frameIndex)
{
    UINT cbIndex = m_cbMap[type][frameIndex];
    return this->m_materialConstantsBuffer->GetGPUVirtualAddress(cbIndex);
}

UINT MaterialManager::ReserveSlot()
{
    if (m_nextSlot < static_cast<UINT>(MAX_MATERIAL_COUNT))
        return m_nextSlot++;

    Debug::LogError(" MaterialManager::ReserveSlot - No available constant buffer slots left.");
    Debug::Assert(false, "Exceeded material constant buffer capacity!");
    return UINT_MAX;
}
