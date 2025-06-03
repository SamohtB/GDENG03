#pragma once
#include "TextureTypes.h"

struct Material
{
	TextureType albedo;
	TextureType normal;
	float normalValue;
	TextureType roughness;
	float roughnessValue;
	TextureType metallic;
	float metallicValue;

	Material() = default;
	Material(TextureType albedoTex, TextureType normalTex, float normVal,
		TextureType roughTex, float roughVal,
		TextureType metalTex, float metalVal)
		: albedo(albedoTex), normal(normalTex), normalValue(normVal),
		roughness(roughTex), roughnessValue(roughVal),
		metallic(metalTex), metallicValue(metalVal)
	{
	}
};

static Material rock_mat_0(ROCK_COLOR, UNSET, 0.f, UNSET, 0.f, UNSET, 0.f);
static Material rock_mat_1(ROCK_COLOR, ROCK_NORMAL, 1.0f, UNSET, 0.0f, UNSET, 0);
static Material rock_mat_2(ROCK_COLOR, ROCK_NORMAL, 1.0f, ROCK_ROUGH, 1.0f, UNSET, 0);
static Material metal_plate_mat(METAL_COLOR, METAL_NORMAL, 0.5f, METAL_ROUGH, 0.2f, METAL_METAL, 0.8f);
