#include "Material.h"
#include "GraphicsEngine.h"
#include "RenderSystem.h"
#include "TextureManager.h"

Material::Material(ID3D12Device* device, const String& name) : m_name(name)
{
    m_textureNames.fill(TextureNames::DEFAULT);
    m_handleCache.fill(0);
    m_constantBuffer = std::make_unique<MaterialConstantsBuffer>(device);
    m_isDirty = true;
}

Material::Material(ID3D12Device* device, const String& name, const String& albedoTexture, const Vector4& albedoColor, const String& normalTexture, float normalStrength,
    const String& metalTexture, float metalStrength, const String& roughTexture, float roughStrength, const String& aoTexture, float aoStrength) : m_name(name)
{
    m_textureNames.fill(TextureNames::DEFAULT);
    m_handleCache.fill(0);
    m_constantBuffer = std::make_unique<MaterialConstantsBuffer>(device);

    SetAlbedoTexture(albedoTexture, albedoColor);
    SetMap(MaterialMapType::NORMAL, normalTexture, normalStrength);
    SetMap(MaterialMapType::METAL, metalTexture, metalStrength);
    SetMap(MaterialMapType::ROUGH, roughTexture, roughStrength);
    SetMap(MaterialMapType::AO, aoTexture, aoStrength);

    m_isDirty = true;
}

void Material::SetAlbedoTexture(const String& textureName, const Vector4& color)
{
    UINT index = GraphicsEngine::GetInstance()->GetRenderSystem()->GetCurrentFrameIndex();
    auto textureManager = GraphicsEngine::GetInstance()->GetTextureManager();
    m_textureNames[MaterialMapType::ALBEDO] = textureName;
    m_handleCache[MaterialMapType::ALBEDO] = textureManager->GetTextureSRVIndex(textureName);
    m_baseColor = color;

    UpdateFlags();
    this->m_isDirty = true;
}

void Material::SetMap(MaterialMapType type, const String& textureName, float strength)
{
    assert(type != MaterialMapType::ALBEDO && "Use SetAlbedoTexture for ALBEDO");
    auto textureManager = GraphicsEngine::GetInstance()->GetTextureManager();

    m_textureNames[type] = textureName;
    m_handleCache[type] = textureManager->GetTextureSRVIndex(textureName);

    switch (type)
    {
    case MaterialMapType::NORMAL: m_normalStrength = strength; break;
    case MaterialMapType::METAL:  m_metalStrength = strength; break;
    case MaterialMapType::ROUGH:  m_roughStrength = strength; break;
    case MaterialMapType::AO:     m_aoStrength = strength; break;
    default: break;
    }

    UpdateFlags();
    this->m_isDirty = true;
}

uint32_t Material::GetSRVIndex(MaterialMapType type) const
{
    return m_handleCache[type];
}

D3D12_GPU_VIRTUAL_ADDRESS Material::GetCBufferAddress()
{
    return this->m_constantBuffer->GetGPUVirtualAddress();
}

void Material::UpdateMaterialConstants()
{
    MaterialConstantsData data{};
    data.albedoHandleIndex = m_handleCache[MaterialMapType::ALBEDO];
    data.baseColor = m_baseColor;
    data.normalHandleIndex = m_handleCache[MaterialMapType::NORMAL];
    data.normalStr = m_normalStrength;
    data.metalHandleIndex = m_handleCache[MaterialMapType::METAL];
    data.metalStr = m_metalStrength;
    data.roughHandleIndex = m_handleCache[MaterialMapType::ROUGH];
    data.roughStr = m_roughStrength;
    data.ambientOcclusionHandleIndex = m_handleCache[MaterialMapType::AO];
    data.ambientOcclussionStr = m_aoStrength;
    data.materialFlags = m_flags;

    m_constantBuffer->Update(data);
    this->m_isDirty = false;
}

bool Material::IsDirty() const
{
    return this->m_isDirty;
}

void Material::UpdateFlags()
{
    m_flags = 0;

    if (m_textureNames[MaterialMapType::ALBEDO] != TextureNames::DEFAULT)  m_flags |= MaterialFlags::HasAlbedoMap;
    if (m_textureNames[MaterialMapType::NORMAL] != TextureNames::DEFAULT)  m_flags |= MaterialFlags::HasNormalMap;
    if (m_textureNames[MaterialMapType::METAL] != TextureNames::DEFAULT)   m_flags |= MaterialFlags::HasMetallicMap;
    if (m_textureNames[MaterialMapType::ROUGH] != TextureNames::DEFAULT)   m_flags |= MaterialFlags::HasRoughnessMap;
    if (m_textureNames[MaterialMapType::AO] != TextureNames::DEFAULT)      m_flags |= MaterialFlags::HasAOMap;
}
