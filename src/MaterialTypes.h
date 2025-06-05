#pragma once
#include "TextureTypes.h"
#include <iostream>

enum MaterialType : int
{
	DEFAULT_MAT = 0,
	ROCK = 1,
	METAL_PLATE = 2
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

	void Print() const
	{
		std::cout << "Material:\n";
		std::cout << "  Albedo: " << static_cast<int>(albedo) << "\n";
		std::cout << "  Normal: " << static_cast<int>(normal) << " (Strength: " << normalValue << ")\n";
		std::cout << "  Roughness: " << static_cast<int>(roughness) << " (Strength: " << roughnessValue << ")\n";
	}
};

static MaterialDescription default_mat(DEFAULT_TEXTURE, DEFAULT_TEXTURE, 0.0f, DEFAULT_TEXTURE, 0.0f);
static MaterialDescription rock_mat_0(ROCK_COLOR, DEFAULT_TEXTURE, 0.f, DEFAULT_TEXTURE, 0.f);
static MaterialDescription rock_mat_1(ROCK_COLOR, ROCK_NORMAL, 0.5f, DEFAULT_TEXTURE, 0.0f);
static MaterialDescription rock_mat_2(ROCK_COLOR, ROCK_NORMAL, 0.5f, ROCK_ROUGH, 0.8f);
static MaterialDescription metal_plate_mat_0(METAL_COLOR, DEFAULT_TEXTURE, 0.f, DEFAULT_TEXTURE, 0.f);
static MaterialDescription metal_plate_mat_1(METAL_COLOR, METAL_NORMAL, 0.9f, DEFAULT_TEXTURE, 0.f);
static MaterialDescription metal_plate_mat_2(METAL_COLOR, METAL_NORMAL, 0.9f, METAL_ROUGH, 0.2f);
