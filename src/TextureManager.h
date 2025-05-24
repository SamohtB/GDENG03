#pragma once
#include <string>
#include <vector>
#include <memory>

#include "stdafx.h"
#include <DirectXTex.h>

struct Texture 
{
    ComPtr<ID3D12Resource> resource;
    ComPtr<ID3D12Resource> uploadHeap;
};

class TextureManager
{
public:

    enum TextureType : int
    {
        UNSET = -1,
        CRATE = 0
    };

    using TexturePtr = std::shared_ptr<Texture>;
    using String = std::string;
    using TextureList = std::vector<TexturePtr>;
    using SRVMap = std::unordered_map<TextureType, UINT>;

    static TextureManager* GetInstance();
    static void Initialize();
    static void Destroy();

    void LoadTexture(const TextureType& type, const std::wstring& filepath);
    CD3DX12_GPU_DESCRIPTOR_HANDLE GetSRVHandle(TextureType type);
    ID3D12DescriptorHeap* GetSRVHeap();

private:
    TextureManager();
    void InitializeSRV();
    ~TextureManager() = default;
    TextureManager(TextureManager const&) {}
    TextureManager& operator=(TextureManager const&) {}

    static TextureManager* sharedInstance;

    const UINT MAX_SRV_COUNT = 64;
    ComPtr<ID3D12DescriptorHeap> m_srvHeap;
    UINT m_srvDescriptorSize = 0;
    UINT m_currentSRVOffset = 0;
    TextureList m_textureList; // just for storing
    SRVMap m_srvMap;

    UINT AllocateSRVSlot();
    D3D12_RESOURCE_DESC CreateResourceDescFromMetadata(const DirectX::TexMetadata& metadata);
};