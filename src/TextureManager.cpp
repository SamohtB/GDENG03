#include "TextureManager.h"
#include "Debug.h"

TextureManager::TextureManager(std::shared_ptr<DescriptorHeapManager> heapManager, std::shared_ptr<BatchUploader> uploader)
    : m_heapManager(heapManager), m_uploader(uploader)
{
}    

void TextureManager::LoadInitialTextures()
{
    LoadTexture(TextureType::COLOR, L"Assets/Textures/rock_c.jpg");
    Debug::Log("Brick_Diffuse Texture Successfully Loaded");

    LoadTexture(TextureType::NORMAL, L"Assets/Textures/rock_n.jpg");
    Debug::Log("Brick_Normals Successfully Loaded");
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
