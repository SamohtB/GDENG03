#pragma once
#include "Dx12Commons.h"

class ConstantBuffer
{
public:

	struct alignas(256) SceneConstantBuffer 
	{
		float time = 0;
	};

	ConstantBuffer(ID3D12Device* device);
	~ConstantBuffer() = default;

	void Update(float time, UINT currentFrameIndex);
	D3D12_GPU_VIRTUAL_ADDRESS GetVirtualAddress(UINT currentFrameIndex);

	static constexpr UINT BUFFER_SIZE = (sizeof(SceneConstantBuffer) + 255) & ~255;

private:
	ComPtr<ID3D12Resource> m_constantBuffer;
	SceneConstantBuffer m_constantBufferData {};
	UINT8* m_pCbvDataBegin;
};

