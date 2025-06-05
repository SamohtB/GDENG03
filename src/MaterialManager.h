#pragma once
#include <memory>
#include <array>
#include <unordered_map>
#include "Dx12Commons.h"

#include "DescriptorHeapManager.h"
#include "BatchUploader.h"
#include "MaterialTypes.h"
#include "Material.h"

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