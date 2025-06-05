#include "TextureManager.h"
#include "Debug.h"

TextureManager::TextureManager(std::shared_ptr<DescriptorHeapManager> heapManager, std::shared_ptr<BatchUploader> uploader)
    : m_heapManager(heapManager), m_uploader(uploader)
{
}    

void TextureManager::LoadInitialTextures()
{
    CreateDefaultWhiteTexture();

    LoadTexture(TextureType::ROCK_COLOR, L"Assets/Textures/rock_d.jpg");
    Debug::Log("Rock_Diffuse Texture Successfully Loaded");

    LoadTexture(TextureType::ROCK_NORMAL, L"Assets/Textures/rock_n.jpg");
    Debug::Log("Rock_Normal Texture Successfully Loaded");

    LoadTexture(TextureType::ROCK_ROUGH, L"Assets/Textures/rock_r.jpg");
    Debug::Log("Rock_Rough Texture Successfully Loaded");

    LoadTexture(TextureType::ROCK_AO, L"Assets/Textures/rock_ao.jpg");
    Debug::Log("Rock_AO Texture Successfully Loaded");

    LoadTexture(TextureType::METAL_COLOR, L"Assets/Textures/metal_plate_d.jpg");
    Debug::Log("metal_plate_d Texture Successfully Loaded");

    LoadTexture(TextureType::METAL_NORMAL, L"Assets/Textures/metal_plate_n.jpg");
    Debug::Log("metal_plate_n Texture Successfully Loaded");

    LoadTexture(TextureType::METAL_ROUGH, L"Assets/Textures/metal_plate_r.jpg");
    Debug::Log("metal_plate_r Texture Successfully Loaded");

    LoadTexture(TextureType::METAL_METAL, L"Assets/Textures/metal_plate_m.jpg");
    Debug::Log("metal_plate_m Texture Successfully Loaded");

    LoadTexture(TextureType::BRICKS_COLOR, L"Assets/Textures/bricks_d.jpg");
    Debug::Log("bricks_d Texture Successfully Loaded");

    LoadTexture(TextureType::BRICKS_NORMAL, L"Assets/Textures/bricks_n.jpg");
    Debug::Log("bricks_n Texture Successfully Loaded");

    LoadTexture(TextureType::BRICKS_ROUGH, L"Assets/Textures/bricks_r.jpg");
    Debug::Log("bricks_r Texture Successfully Loaded");

    LoadTexture(TextureType::BRICKS_AO, L"Assets/Textures/bricks_ao.jpg");
    Debug::Log("bricks_ao Texture Successfully Loaded");
}

void TextureManager::LoadTexture(const TextureType& type, const std::wstring& filePath)
{
    auto srvIndex = m_heapManager->AllocateSRVSlot();
    auto srvHandle = m_heapManager->GetShaderVisibleCPUHandleAt(srvIndex);
    auto buffer = m_uploader->SchedTexture(filePath, srvHandle);

    TexturePtr texture = std::make_unique<Texture>(buffer, srvIndex);
    this->m_srvMap[type] = srvIndex;
    this->m_textureMap[type] = std::move(texture);
}

D3D12_GPU_DESCRIPTOR_HANDLE TextureManager::GetTextureHandle(const TextureType& type)
{
    UINT index = m_srvMap[type];
    return m_heapManager->GetShaderVisibleGPUHandleAt(index);
}

void TextureManager::CreateDefaultWhiteTexture()
{
    auto srvIndex = m_heapManager->AllocateSRVSlot();
    auto srvHandle = m_heapManager->GetShaderVisibleCPUHandleAt(srvIndex);
    auto buffer = m_uploader->SchedWhitePixelTexture(srvHandle);

    TexturePtr whiteTex = std::make_unique<Texture>(buffer, srvIndex);
    this->m_srvMap[TextureType::DEFAULT_TEXTURE] = srvIndex;
    this->m_textureMap[TextureType::DEFAULT_TEXTURE] = std::move(whiteTex);

    Debug::Log("Default white texture generated and uploaded.");
}
