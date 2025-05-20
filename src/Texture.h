#pragma once
#include <DirectXTex.h>

#include "stdafx.h"

class Texture
{
public:
	Texture(std::wstring texturePath);
	~Texture() = default;

	ComPtr<ID3D12Resource> GetTexture() const;
	ComPtr<ID3D12DescriptorHeap> GetSRVHeap() const;

private:
	ComPtr<ID3D12Resource> m_texture;
	ComPtr<ID3D12Resource> m_textureUploadHeap;
	ComPtr<ID3D12DescriptorHeap> m_srvHeap;

	D3D12_RESOURCE_DESC CreateResourceDescFromMetadata(const DirectX::TexMetadata& metadata);
};

