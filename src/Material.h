#pragma once
#include "ConstantBuffer.h"
#include "MaterialTypes.h"

class Material
{
public:
	Material(MaterialDescription desc, const MaterialConstantsData& data);
	~Material() = default;

	MaterialConstantsData GetMaterialData();

private:
	MaterialConstantsData m_data;
	MaterialDescription m_description;
};

