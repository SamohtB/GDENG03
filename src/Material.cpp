#include "Material.h"

Material::Material(const std::vector<ComPtr<ID3D12Resource>>& buffers, MaterialDescription desc)
{
	for (int i = 0; i < FRAME_COUNT; i++)
	{
		this->m_materialBuffers[i] = std::make_unique<MaterialBuffer>(buffers[i]);
	}

	this->SetMaterialDescription(desc);
}

void Material::SetMaterialDescription(MaterialDescription desc)
{
	for (int i = 0; i < FRAME_COUNT; i++)
	{
		this->m_materialBuffers[i]->Update(desc);
	}
}
