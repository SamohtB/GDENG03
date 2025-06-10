#pragma once
#include "Dx12Commons.h"

class DepthBuffer
{
public:
	DepthBuffer(ID3D12Device* device, D3D12_CPU_DESCRIPTOR_HANDLE handle, UINT width, UINT height);
	~DepthBuffer() = default;

	ID3D12Resource* GetDepthStencilBuffer() const;

private:
	ComPtr<ID3D12Resource> m_depthStencilBuffer;
};

