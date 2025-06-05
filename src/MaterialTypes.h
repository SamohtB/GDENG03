#pragma once
#include "TextureTypes.h"
#include <iostream>
#include <unordered_map>

enum MaterialType : int
{
	DEFAULT_MAT = 0,
	ROCK_0 = 1,
	ROCK_1,
	METAL_PLATE_0,
	METAL_PLATE_1,
	BRICKS_0,
	BRICKS_1
};

struct MaterialDescription
{
    uint32_t albedoTextureIndex;
    uint32_t normalTextureIndex;
    float normalStr;
    uint32_t metallicTextureIndex;
    float metallicStr;
    uint32_t roughTextureIndex;
    float roughStr;
    uint32_t ambientOcclusionTextureIndex;
    float ambientOcclusionStr;

    MaterialDescription() = default;

    MaterialDescription(
        uint32_t albedoIdx,
        uint32_t normalIdx, float normalStrength,
        uint32_t metallicIdx, float metallicStrength,
        uint32_t roughIdx, float roughStrength,
        uint32_t aoIdx, float aoStrength)
        : albedoTextureIndex(albedoIdx),
        normalTextureIndex(normalIdx), normalStr(normalStrength),
        metallicTextureIndex(metallicIdx), metallicStr(metallicStrength),
        roughTextureIndex(roughIdx), roughStr(roughStrength),
        ambientOcclusionTextureIndex(aoIdx), ambientOcclusionStr(aoStrength)
    {
    }
};



namespace MaterialData
{
    enum Flags : uint32_t
    {
        HasAlbedoMap = 1 << 0,
        HasNormalMap = 1 << 1,
        HasMetallicMap = 1 << 2,
        HasRoughnessMap = 1 << 3,
        HasAOMap = 1 << 4,
    };

    inline uint32_t GetFlags(const MaterialDescription& mat)
    {
        uint32_t flags = 0;

        if (mat.albedoTextureIndex != DEFAULT_TEXTURE)      flags |= HasAlbedoMap;
        if (mat.normalTextureIndex != DEFAULT_TEXTURE)      flags |= HasNormalMap;
        if (mat.metallicTextureIndex != DEFAULT_TEXTURE)    flags |= HasMetallicMap;
        if (mat.roughTextureIndex != DEFAULT_TEXTURE)       flags |= HasRoughnessMap;
        if (mat.ambientOcclusionTextureIndex != DEFAULT_TEXTURE) flags |= HasAOMap;

        return flags;
    }

    static const std::unordered_map<MaterialType, MaterialDescription> Materials = {
        //                     Albedo       Normal        NormalStr  Metallic       MetallicStr  Roughness      RoughStr  AO              AOStr
        { DEFAULT_MAT,      { DEFAULT_TEXTURE, DEFAULT_TEXTURE, 0.0f, DEFAULT_TEXTURE, 0.0f, DEFAULT_TEXTURE, 0.0f,   DEFAULT_TEXTURE, 0.0f } },
        { ROCK_0,           { ROCK_COLOR,      DEFAULT_TEXTURE, 0.0f, DEFAULT_TEXTURE, 0.0f, DEFAULT_TEXTURE, 0.0f,   DEFAULT_TEXTURE, 0.0f } },
        { ROCK_1,           { ROCK_COLOR,      ROCK_NORMAL,     1.0f, DEFAULT_TEXTURE, 0.0f, ROCK_ROUGH,      1.0f,   ROCK_AO,         1.0f } },
        { METAL_PLATE_0,    { METAL_COLOR,     DEFAULT_TEXTURE, 0.0f, DEFAULT_TEXTURE, 0.0f, DEFAULT_TEXTURE, 0.0f,   DEFAULT_TEXTURE, 0.0f } },
        { METAL_PLATE_1,    { METAL_COLOR,     METAL_NORMAL,    1.0f, METAL_METAL,     0.8f, METAL_ROUGH,     0.2f,   DEFAULT_TEXTURE, 0.0f } },
        { BRICKS_0,         { BRICKS_COLOR,    DEFAULT_TEXTURE, 0.0f, DEFAULT_TEXTURE, 0.0f, DEFAULT_TEXTURE, 0.0f,   DEFAULT_TEXTURE, 0.0f } },
        { BRICKS_1,         { BRICKS_COLOR,    BRICKS_NORMAL,   1.0f, DEFAULT_TEXTURE, 0.0f, BRICKS_ROUGH,    1.0f,   BRICKS_AO,       1.0f } },
    };
}



