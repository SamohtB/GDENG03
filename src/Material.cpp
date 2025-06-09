#include "Material.h"

Material::Material(MaterialDescription desc, const MaterialConstantsData& data) : m_description(desc), m_data(data)
{

}

MaterialConstantsData Material::GetMaterialData()
{
	return this->m_data;
}
