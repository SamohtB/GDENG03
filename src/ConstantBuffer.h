#pragma once
#include "Dx12Commons.h"
#include <directxtk12/SimpleMath.h>

class ConstantBuffer
{
public:

	struct alignas(256) SceneConstantBuffer 
	{
		float normal = 0;
		float roughness = 0;
		float metalness = 0;
	};

	ConstantBuffer(ID3D12Device* device);
	~ConstantBuffer() = default;

	void Update(DirectX::SimpleMath::Vector3 matValues, UINT currentFrameIndex);
	D3D12_GPU_VIRTUAL_ADDRESS GetVirtualAddress(UINT currentFrameIndex);

	static constexpr UINT BUFFER_SIZE = (sizeof(SceneConstantBuffer) + 255) & ~255;

private:
	ComPtr<ID3D12Resource> m_constantBuffer;
	SceneConstantBuffer m_constantBufferData {};
	UINT8* m_pCbvDataBegin;
};

