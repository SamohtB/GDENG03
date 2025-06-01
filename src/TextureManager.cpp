#include "TextureManager.h"
#include "Debug.h"

TextureManager::TextureManager(std::shared_ptr<DescriptorHeapManager> heapManager, std::shared_ptr<BatchUploader> uploader)
    : m_heapManager(heapManager), m_uploader(uploader)
{
}    

void TextureManager::LoadInitialTextures()
{
    LoadTexture(TextureType::CRATE, L"Assets/Textures/crate.png");
    Debug::Log("Crate Texture Successfully Loaded");
}

void TextureManager::LoadTexture(const TextureType& type, const std::wstring& filePath)
{
    auto srvIndex = m_heapManager->AllocateSRVSlot();
    auto srvHandle = m_heapManager->GetCPUHandleAt(srvIndex);
    auto buffer = m_uploader->SchedTexture(filePath, srvHandle);

    TexturePtr texture = std::make_unique<Texture>(buffer, srvIndex);
    this->m_srvMap[type] = srvIndex;
    this->m_textureMap[type] = std::move(texture);
}