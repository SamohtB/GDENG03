#pragma once
#include "pch.h"
#include "ConstantBuffer.h"
#include "TextureNames.h"
#include "MaterialNames.h"

class Material
{
public:
	Material(ID3D12Device* device, const String& name);
    Material(ID3D12Device* device, const String& name, const String& albedoTexture, const Vector4& albedoColor, const String& normalTexture, float normalStrength,
        const String& metalTexture, float metalStrength, const String& roughTexture, float roughStrength, const String& aoTexture, float aoStrength);
	~Material() = default;

    void SetAlbedoTexture(const String& textureName, const Vector4& color);
    void SetMap(MaterialMapType type, const String& textureName, float strength);

    uint32_t GetSRVIndex(MaterialMapType type) const;
    D3D12_GPU_VIRTUAL_ADDRESS GetCBufferAddress(UINT frameIndex);

    void UpdateIfDirty();
    void BakeConstants(UINT index) const;

private:

    void UpdateFlags();

	std::unique_ptr<MaterialConstantsBuffer> m_constantBuffer;
    std::array<String, MATERIAL_MAP_COUNT> m_textureNames;
    std::array<uint32_t, MATERIAL_MAP_COUNT> m_handleCache;

    Vector4 m_baseColor = Vector4(1, 1, 1, 1);
    float m_normalStrength = 1.0f;
    float m_metalStrength = 0.0f;
    float m_roughStrength = 1.0f;
    float m_aoStrength = 1.0f;
    uint32_t m_flags = 0;

    bool m_isDirty = false;
    String m_name;
};

