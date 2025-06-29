#pragma once
#include "pch.h"
#include "FrameConstants.h"

#include "ConstantBuffer.h"
#include "MaterialNames.h"
#include "Material.h"

class MaterialManager
{
public:
    MaterialManager(ComPtr<ID3D12Device> device);
    ~MaterialManager() = default;

    using MaterialPtr = std::unique_ptr<Material>;
    using MaterialMap = std::unordered_map<String, MaterialPtr>;    

    void LoadInitialMaterials();
    void CreateMaterial(const std::string& materialName);
    void CreateMaterial(const std::string& materialName, const std::string& albedoTex, const Vector4& albedoColor, const std::string& normalTex, float normalStrength, 
        const std::string& metalTex, float metalStrength, const std::string& roughTex, float roughStrength, const std::string& aoTex, float aoStrength);
    
    Material* GetMaterial(const String& materialName);
    D3D12_GPU_VIRTUAL_ADDRESS GetMaterialDataAddress(const String& materialName);

    void BakeConstants();

private:
    ComPtr<ID3D12Device> m_device;
    MaterialMap m_materialMap;
};