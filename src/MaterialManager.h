#pragma once
#include "pch.h"
#include "FrameConstants.h"

#include "ConstantBuffer.h"
#include "MaterialTypes.h"
#include "Material.h"

class MaterialManager
{
public:
    MaterialManager(ID3D12Device* device);
    ~MaterialManager() = default;

    using MaterialPtr = std::unique_ptr<Material>;
    using MaterialMap = std::unordered_map<MaterialType, MaterialPtr>;
    using CBMap = std::unordered_map<MaterialType, std::array<UINT, FRAME_COUNT>>;

    void LoadInitialMaterials();
    void LoadMaterial(const MaterialType& type, const MaterialDescription& description);
    void UpdateMaterialConstants(const MaterialType& type, UINT frameIndex);
    D3D12_GPU_VIRTUAL_ADDRESS GetMaterialConstantsAddress(MaterialType type, UINT frameIndex);

private:
    UINT ReserveSlot();

    CBMap m_cbMap;
    MaterialMap m_materialMap;
    std::unique_ptr<MaterialConstantsBuffer> m_materialConstantsBuffer;
    
    UINT m_nextSlot = 0;
    const int MAX_MATERIAL_COUNT = 128;
};