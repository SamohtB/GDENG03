#pragma once
#include <string>
#include <memory>
#include <vector>
#include <array>
#include "Dx12Commons.h"
#include "MaterialBuffer.h"
#include "MaterialTypes.h"

class Material
{
public:
	Material(const std::vector<ComPtr<ID3D12Resource>>& buffers, MaterialDescription desc);
	~Material() = default;

	void SetMaterialDescription(MaterialDescription desc);

private:
	std::array<std::unique_ptr<MaterialBuffer>, FRAME_COUNT> m_materialBuffers;
};

