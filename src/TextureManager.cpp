#include "TextureManager.h"
#include "Debug.h"

TextureManager::TextureManager(std::shared_ptr<DescriptorHeapManager> heapManager, std::shared_ptr<BatchUploader> uploader)
    : m_heapManager(heapManager), m_uploader(uploader)
{
}    

void TextureManager::LoadInitialTextures()
{
    LoadTexture(TextureNames::ROCK_COLOR, L"Assets/Textures/rock_d.jpg");
    Debug::Log("Rock_Diffuse Texture Successfully Loaded");

    LoadTexture(TextureNames::ROCK_NORMAL, L"Assets/Textures/rock_n.jpg");
    Debug::Log("Rock_Normal Texture Successfully Loaded");

    LoadTexture(TextureNames::ROCK_ROUGH, L"Assets/Textures/rock_r.jpg");
    Debug::Log("Rock_Rough Texture Successfully Loaded");

    LoadTexture(TextureNames::ROCK_AO, L"Assets/Textures/rock_ao.jpg");
    Debug::Log("Rock_AO Texture Successfully Loaded");

    LoadTexture(TextureNames::METAL_COLOR, L"Assets/Textures/metal_plate_d.jpg");
    Debug::Log("metal_plate_d Texture Successfully Loaded");

    LoadTexture(TextureNames::METAL_NORMAL, L"Assets/Textures/metal_plate_n.jpg");
    Debug::Log("metal_plate_n Texture Successfully Loaded");

    LoadTexture(TextureNames::METAL_ROUGH, L"Assets/Textures/metal_plate_r.jpg");
    Debug::Log("metal_plate_r Texture Successfully Loaded");

    LoadTexture(TextureNames::METAL_METAL, L"Assets/Textures/metal_plate_m.jpg");
    Debug::Log("metal_plate_m Texture Successfully Loaded");

    LoadTexture(TextureNames::BRICKS_COLOR, L"Assets/Textures/bricks_d.jpg");
    Debug::Log("bricks_d Texture Successfully Loaded");

    LoadTexture(TextureNames::BRICKS_NORMAL, L"Assets/Textures/bricks_n.jpg");
    Debug::Log("bricks_n Texture Successfully Loaded");

    LoadTexture(TextureNames::BRICKS_ROUGH, L"Assets/Textures/bricks_r.jpg");
    Debug::Log("bricks_r Texture Successfully Loaded");

    LoadTexture(TextureNames::BRICKS_AO, L"Assets/Textures/bricks_ao.jpg");
    Debug::Log("bricks_ao Texture Successfully Loaded");
}

void TextureManager::LoadTexture(const String& textureName, const std::wstring& filePath)
{
    auto srvIndex = m_heapManager->AllocateSRVSlot();
    auto srvHandle = m_heapManager->GetSRVCPUHandleAt(srvIndex);
    auto buffer = m_uploader->SchedTexture(filePath, srvHandle);

    TexturePtr texture = std::make_unique<Texture>(buffer, srvIndex);
    this->m_srvMap[textureName] = srvIndex;
    this->m_textureMap[textureName] = std::move(texture);
}

D3D12_GPU_DESCRIPTOR_HANDLE TextureManager::GetSRVStart()
{
    return m_heapManager->GetSRVGPUHandleAt(0);
}

UINT TextureManager::GetTextureSRVIndex(const String& textureName)
{
    auto it = m_srvMap.find(textureName);
    if (it == m_srvMap.end())
    {
        Debug::LogError("[TextureManager] Warning: Texture '" + textureName + "' not found in SRV map");
        return 0;
    }

    return it->second;
}
