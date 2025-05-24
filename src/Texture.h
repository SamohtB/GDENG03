#pragma once
#include "stdafx.h"
#include <DirectXTex.h>

class TextureManager;

class Texture
{
public:
	Texture(std::wstring texturePath);
	~Texture() = default;

	ID3D12Resource* GetTexture() const;

private:
	ComPtr<ID3D12Resource> m_texture;
	ComPtr<ID3D12Resource> m_textureUploadHeap;

	D3D12_RESOURCE_DESC CreateResourceDescFromMetadata(const DirectX::TexMetadata& metadata);
	friend TextureManager;
};

