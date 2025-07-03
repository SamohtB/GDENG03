#include "Material.h"

Material::Material(String name, const std::vector<D3D12_GPU_VIRTUAL_ADDRESS>& perFrameAddress, MaterialConstants constants)
	: m_name(name), m_gpuAddresses(perFrameAddress), m_data(constants), m_version(0)
{
}

D3D12_GPU_VIRTUAL_ADDRESS Material::GetCBAddress(UINT currentFrameIndex)
{
	return this->m_gpuAddresses[currentFrameIndex];
}
