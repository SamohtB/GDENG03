#pragma once
#include "TextureTypes.h"
#include <iostream>

enum MaterialType : int
{
	DEFAULT_MAT = 0,
	ROCK_0 = 1,
	ROCK_1,
	ROCK_2,
	METAL_PLATE_0,
	METAL_PLATE_1,
	METAL_PLATE_2,
	BRICKS_0,
	BRICKS_1,
	BRICKS_2
};

struct MaterialDescription
{
	TextureType albedo;
	TextureType normal;
	float normalValue;
	TextureType roughness;
	float roughnessValue;

	MaterialDescription() = default;
	MaterialDescription(TextureType albedoTex, TextureType normalTex, float normVal,
		TextureType roughTex, float roughVal)
		: albedo(albedoTex), normal(normalTex), normalValue(normVal),
		roughness(roughTex), roughnessValue(roughVal)
	{

	}
};
