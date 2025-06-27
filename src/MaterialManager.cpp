#include "MaterialManager.h"
#include "Debug.h"

MaterialManager::MaterialManager(ComPtr<ID3D12Device> device) : m_device(device)
{
}

void MaterialManager::LoadInitialMaterials()
{
    CreateMaterial(MaterialNames::DEFAULT,
        TextureNames::DEFAULT, Vector4(1, 1, 1, 1),
        TextureNames::DEFAULT, 0.0f,
        TextureNames::DEFAULT, 0.0f,
        TextureNames::DEFAULT, 0.0f,
        TextureNames::DEFAULT, 0.0f);

    CreateMaterial(MaterialNames::ROCK,
        TextureNames::ROCK_COLOR, Vector4(1, 1, 1, 1),
        TextureNames::ROCK_NORMAL, 1.0f,
        TextureNames::DEFAULT, 0.0f,
        TextureNames::ROCK_ROUGH, 1.0f,
        TextureNames::ROCK_AO, 1.0f);

    CreateMaterial(MaterialNames::METAL,
        TextureNames::METAL_COLOR, Vector4(1, 1, 1, 1),
        TextureNames::METAL_NORMAL, 1.0f,
        TextureNames::METAL_METAL, 0.8f,
        TextureNames::METAL_ROUGH, 1.0f,
        TextureNames::DEFAULT, 0.0f);

    CreateMaterial(MaterialNames::BRICKS,
        TextureNames::BRICKS_COLOR, Vector4(1, 1, 1, 1),
        TextureNames::BRICKS_NORMAL, 1.0f,
        TextureNames::DEFAULT, 0.0f,
        TextureNames::BRICKS_ROUGH, 1.0f,
        TextureNames::BRICKS_AO, 1.0f);
    
}

void MaterialManager::CreateMaterial(const std::string& materialName)
{
    if (m_materialMap.find(materialName) != m_materialMap.end())
    {
        Debug::LogWarning("Duplicated Material! " + materialName);
    }

    auto material = std::make_unique<Material>(m_device.Get(), materialName);
    m_materialMap[materialName] = std::move(material);
    Debug::Log("Created " + materialName);
}

void MaterialManager::CreateMaterial(const std::string& materialName, const std::string& albedoTex, const Vector4& albedoColor, const std::string& normalTex, float normalStrength, const std::string& metalTex, float metalStrength, const std::string& roughTex, float roughStrength, const std::string& aoTex, float aoStrength)
{
    if (m_materialMap.find(materialName) != m_materialMap.end())
    {
        Debug::LogWarning("Duplicated Material! " + materialName);
    }

    auto material = std::make_unique<Material>(m_device.Get(), materialName, albedoTex, albedoColor, normalTex, normalStrength,
        metalTex, metalStrength, roughTex, roughStrength, aoTex, aoStrength);
    m_materialMap[materialName] = std::move(material);
    Debug::Log("Created " + materialName);
}

Material* MaterialManager::GetMaterial(const String& materialName)
{
    return this->m_materialMap[materialName].get();
}

D3D12_GPU_VIRTUAL_ADDRESS MaterialManager::GetMaterialDataAddress(const String& materialName)
{
    return this->m_materialMap[materialName]->GetCBufferAddress();
}

