#pragma once
#include <memory>
#include <unordered_map>

#include "DescriptorHeapManager.h"
#include "BatchUploader.h"
#include "Texture.h"
#include "TextureNames.h"

class TextureManager
{
public:
    TextureManager(std::shared_ptr<DescriptorHeapManager> heapManager, std::shared_ptr<BatchUploader> uploader);
    ~TextureManager() = default;

    using TexturePtr = std::unique_ptr<Texture>;
    using TextureMap = std::unordered_map<String, TexturePtr>;
    using SRVMap = std::unordered_map<String, UINT>;

    void LoadInitialTextures();
    void LoadTexture(const String& textureName, const std::wstring& filePath);
    D3D12_GPU_DESCRIPTOR_HANDLE GetSRVStart();
    UINT GetTextureSRVIndex(const String& textureName);

private:
    std::shared_ptr<DescriptorHeapManager> m_heapManager;
    std::shared_ptr<BatchUploader> m_uploader;
    SRVMap m_srvMap;
    TextureMap m_textureMap;
};