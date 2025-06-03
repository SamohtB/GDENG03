#pragma once
#include "TextureTypes.h"

struct Material
{
	TextureType albedo;
	TextureType normal;
	float normalValue;
	TextureType roughness;
	float roughnessValue;

	Material() = default;
	Material(TextureType albedoTex, TextureType normalTex, float normVal,
		TextureType roughTex, float roughVal)
		: albedo(albedoTex), normal(normalTex), normalValue(normVal),
		roughness(roughTex), roughnessValue(roughVal)
	{
	}
};

static Material rock_mat_0(ROCK_COLOR, UNSET, 0.f, UNSET, 0.f);
static Material rock_mat_1(ROCK_COLOR, ROCK_NORMAL, 0.5f, UNSET, 0.0f);
static Material rock_mat_2(ROCK_COLOR, ROCK_NORMAL, 0.5f, ROCK_ROUGH, 0.8f);
static Material metal_plate_mat_0(METAL_COLOR, UNSET, 0.f, UNSET, 0.f);
static Material metal_plate_mat_1(METAL_COLOR, METAL_NORMAL, 0.9f, METAL_ROUGH, 0.2f);
