#pragma once
#include <string>
#include "TextureNames.h"

struct MaterialNames
{
    inline static const std::string DEFAULT = "DEFAULT";
    inline static const std::string ROCK = "ROCK";
    inline static const std::string METAL = "METAL_PLATE";
    inline static const std::string BRICKS = "BRICKS";
};

enum MaterialMapType : int
{
    ALBEDO = 0,
    NORMAL,
    METAL,
    ROUGH,
    AO,
    HEIGHT
};

constexpr size_t MATERIAL_MAP_COUNT = 6;

enum MaterialFlags : uint32_t
{
    HasAlbedoMap = 1 << 0,
    HasNormalMap = 1 << 1,
    HasMetallicMap = 1 << 2,
    HasRoughnessMap = 1 << 3,
    HasAOMap = 1 << 4,
};