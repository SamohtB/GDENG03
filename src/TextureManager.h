#pragma once
#include <memory>
#include <unordered_map>

#include "DescriptorHeapManager.h"
#include "BatchUploader.h"
#include "TextureTypes.h"
#include "Texture.h"

class TextureManager
{
public:
    TextureManager(std::shared_ptr<DescriptorHeapManager> heapManager, std::shared_ptr<BatchUploader> uploader);
    ~TextureManager() = default;

    using TexturePtr = std::unique_ptr<Texture>;
    using TextureMap = std::unordered_map<TextureType, TexturePtr>;
    using SRVMap = std::unordered_map<TextureType, UINT>;

    void LoadInitialTextures();
    void LoadTexture(const TextureType& type, const std::wstring& filePath);

private:
    std::shared_ptr<DescriptorHeapManager> m_heapManager;
    std::shared_ptr<BatchUploader> m_uploader;
    SRVMap m_srvMap;
    TextureMap m_textureMap;
};