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

    LoadTexture(TextureType::METAL_COLOR, L"Assets/Textures/metal_plate_d.jpg");
    Debug::Log("metal_plate_d Texture Successfully Loaded");

    LoadTexture(TextureType::METAL_NORMAL, L"Assets/Textures/metal_plate_n.jpg");
    Debug::Log("metal_plate_n Texture Successfully Loaded");

    LoadTexture(TextureType::METAL_ROUGH, L"Assets/Textures/metal_plate_r.jpg");
    Debug::Log("metal_plate_r Texture Successfully Loaded");

    LoadTexture(TextureType::METAL_METAL, L"Assets/Textures/metal_plate_m.jpg");
    Debug::Log("metal_plate_m Texture Successfully Loaded");
}

void TextureManager::LoadTexture(const TextureType& type, const std::wstring& filePath)
{
    auto srvIndex = m_heapManager->AllocateSRVSlot();
    auto srvHandle = m_heapManager->GetSRVCPUHandleAt(srvIndex);
    auto buffer = m_uploader->SchedTexture(filePath, srvHandle);

    TexturePtr texture = std::make_unique<Texture>(buffer, srvIndex);
    this->m_srvMap[type] = srvIndex;
    this->m_textureMap[type] = std::move(texture);
}

D3D12_GPU_DESCRIPTOR_HANDLE TextureManager::GetTextureHandle(const TextureType& type)
{
    UINT index = m_srvMap[type];
    return m_heapManager->GetSRVGPUHandleAt(index);
}

void TextureManager::CreateDefaultWhiteTexture()
{
    auto srvIndex = m_heapManager->AllocateSRVSlot();
    auto srvHandle = m_heapManager->GetSRVCPUHandleAt(srvIndex);

    ComPtr<ID3D12Resource> whiteTextureBuffer = m_uploader->SchedWhitePixelTexture(srvHandle);

    TexturePtr whiteTex = std::make_unique<Texture>(whiteTextureBuffer, srvIndex);
    this->m_srvMap[TextureType::UNSET] = srvIndex;
    this->m_textureMap[TextureType::UNSET] = std::move(whiteTex);

    Debug::Log("Default white texture generated and uploaded.");
}
