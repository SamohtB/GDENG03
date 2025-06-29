#include "TextureManager.h"
#include "Debug.h"

TextureManager::TextureManager(std::shared_ptr<DescriptorHeapManager> heapManager, std::shared_ptr<BatchUploader> uploader)
    : m_heapManager(heapManager), m_uploader(uploader)
{
}    

void TextureManager::LoadInitialTextures()
{
    LoadTexture(TextureNames::DEFAULT, L"Assets/Textures/default.png");

    LoadTexture(TextureNames::ROCK_COLOR, L"Assets/Textures/rock_d.jpg");
    LoadTexture(TextureNames::ROCK_NORMAL, L"Assets/Textures/rock_n.jpg");
    LoadTexture(TextureNames::ROCK_ROUGH, L"Assets/Textures/rock_r.jpg");
    LoadTexture(TextureNames::ROCK_AO, L"Assets/Textures/rock_ao.jpg");

    LoadTexture(TextureNames::METAL_COLOR, L"Assets/Textures/metal_plate_d.jpg");
    LoadTexture(TextureNames::METAL_NORMAL, L"Assets/Textures/metal_plate_n.jpg");
    LoadTexture(TextureNames::METAL_ROUGH, L"Assets/Textures/metal_plate_r.jpg");
    LoadTexture(TextureNames::METAL_METAL, L"Assets/Textures/metal_plate_m.jpg");

    LoadTexture(TextureNames::BRICKS_COLOR, L"Assets/Textures/bricks_d.jpg");
    LoadTexture(TextureNames::BRICKS_NORMAL, L"Assets/Textures/bricks_n.jpg");
    LoadTexture(TextureNames::BRICKS_ROUGH, L"Assets/Textures/bricks_r.jpg");
    LoadTexture(TextureNames::BRICKS_AO, L"Assets/Textures/bricks_ao.jpg");

}

void TextureManager::LoadTexture(const String& textureName, const std::wstring& filePath)
{
    auto srvIndex = m_heapManager->AllocateSRVSlot();
    auto srvHandle = m_heapManager->GetSRVCPUHandleAt(srvIndex);
    auto buffer = m_uploader->SchedTexture(filePath, srvHandle);

    TexturePtr texture = std::make_unique<Texture>(buffer, srvIndex);
    this->m_srvMap[textureName] = srvIndex;
    this->m_textureMap[textureName] = std::move(texture);
    Debug::Log(textureName + " Successfully Loaded");
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
