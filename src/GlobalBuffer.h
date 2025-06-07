#pragma once
#include "Dx12Commons.h"

class GlobalBuffer
{
public:

	struct alignas(256) GlobalConstantBuffer
	{
		float time;
	};


	GlobalBuffer(ID3D12Device* device);
	~GlobalBuffer() = default;

	void Update(float time, UINT currentFrame);
	D3D12_GPU_VIRTUAL_ADDRESS GetVirtualAddress(UINT currentFrameIndex);

	static constexpr UINT BUFFER_SIZE = (sizeof(GlobalConstantBuffer) + 255) & ~255;

private:
	ComPtr<ID3D12Resource> m_constantBuffer;
	GlobalConstantBuffer m_constantBufferData{};
	UINT8* m_cbvDataBegin;
};

