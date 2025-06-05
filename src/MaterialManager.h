#pragma once
#include <memory>
#include <array>
#include <unordered_map>
#include "Dx12Commons.h"

#include "DescriptorHeapManager.h"
#include "BatchUploader.h"
#include "MaterialTypes.h"
#include "Material.h"

namespace MaterialData
{
    static const std::unordered_map<MaterialType, MaterialDescription> Materials = {
        { DEFAULT_MAT,      { DEFAULT_TEXTURE, DEFAULT_TEXTURE, 0.0f, DEFAULT_TEXTURE, 0.0f } },
        { ROCK_0,           { ROCK_COLOR, DEFAULT_TEXTURE, 0.0f, DEFAULT_TEXTURE, 0.0f } },
        { ROCK_1,           { ROCK_COLOR, ROCK_NORMAL, 0.5f, DEFAULT_TEXTURE, 0.0f } },
        { ROCK_2,           { ROCK_COLOR, ROCK_NORMAL, 0.5f, ROCK_ROUGH, 0.8f } },
        { METAL_PLATE_0,    { METAL_COLOR, DEFAULT_TEXTURE, 0.0f, DEFAULT_TEXTURE, 0.0f } },
        { METAL_PLATE_1,    { METAL_COLOR, METAL_NORMAL, 0.9f, DEFAULT_TEXTURE, 0.0f } },
        { METAL_PLATE_2,    { METAL_COLOR, METAL_NORMAL, 0.9f, METAL_ROUGH, 0.2f } },
        { BRICKS_0,         { BRICKS_COLOR, DEFAULT_TEXTURE, 0.0f, DEFAULT_TEXTURE, 0.0f } },
        { BRICKS_1,         { BRICKS_COLOR, BRICKS_NORMAL, 0.0f, DEFAULT_TEXTURE, 0.0f } },
        { BRICKS_2,         { BRICKS_COLOR, BRICKS_NORMAL, 0.9f, BRICKS_ROUGH, 0.8f } },
    };
}

class MaterialManager
{
public:
    MaterialManager(std::shared_ptr<DescriptorHeapManager> heapManager, std::shared_ptr<BatchUploader> uploader);
    ~MaterialManager() = default;

    using MaterialPtr = std::unique_ptr<Material>;
    using MaterialMap = std::unordered_map<MaterialType, MaterialPtr>;
    using CBVMap = std::unordered_map<MaterialType, std::array<UINT, FRAME_COUNT>>;

    void LoadInitialMaterials();
    void LoadMaterial(const MaterialType& type, const MaterialDescription& description);
    D3D12_GPU_DESCRIPTOR_HANDLE GetMaterialHandle(const MaterialType& type, UINT frameIndex);

private:
    std::shared_ptr<DescriptorHeapManager> m_heapManager;
    std::shared_ptr<BatchUploader> m_uploader;
    CBVMap m_cbvMap;
    MaterialMap m_materialMap;
};