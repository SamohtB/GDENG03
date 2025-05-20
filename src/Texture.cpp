#include "Texture.h"
#include "Helper.h"
#include "GraphicsEngine.h"

Texture::Texture(std::wstring texturePath)
{
    DirectX::TexMetadata metadata;
    DirectX::ScratchImage scratchImage;

    ThrowIfFailed(LoadFromWICFile(texturePath.c_str(), DirectX::WIC_FLAGS::WIC_FLAGS_NONE, &metadata, scratchImage));

    auto device = GraphicsEngine::GetInstance()->GetRenderSystem()->GetD3DDevice();
    auto list = GraphicsEngine::GetInstance()->GetRenderSystem()->GetCommandList();
    auto desc = CreateResourceDescFromMetadata(metadata);

    /* Create Resource */
    ThrowIfFailed(device->CreateCommittedResource(
        &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
        D3D12_HEAP_FLAG_NONE,
        &desc,
        D3D12_RESOURCE_STATE_COPY_DEST,
        nullptr,
        IID_PPV_ARGS(&m_texture)));

    const UINT64 uploadBufferSize = GetRequiredIntermediateSize(m_texture.Get(), 0, 1);

    // Create the GPU upload buffer.
    ThrowIfFailed(device->CreateCommittedResource(
        &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
        D3D12_HEAP_FLAG_NONE,
        &CD3DX12_RESOURCE_DESC::Buffer(uploadBufferSize),
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&m_textureUploadHeap)));

    const DirectX::Image* img = scratchImage.GetImage(0, 0, 0);

    D3D12_SUBRESOURCE_DATA textureData = {};
    textureData.pData = img->pixels;
    textureData.RowPitch = img->rowPitch;
    textureData.SlicePitch = img->slicePitch;

    UpdateSubresources(list, m_texture.Get(), m_textureUploadHeap.Get(), 0, 0, 1, &textureData);
    list->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_texture.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE));

    // Describe and create a SRV for the texture.
    D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
    srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    srvDesc.Format = desc.Format;
    srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = desc.MipLevels;

    device->CreateShaderResourceView(m_texture.Get(), &srvDesc, m_srvHeap->GetCPUDescriptorHandleForHeapStart());
}

ComPtr<ID3D12Resource> Texture::GetTexture() const
{
    return this->m_texture;
}

ComPtr<ID3D12DescriptorHeap> Texture::GetSRVHeap() const
{
    return this->m_srvHeap;
}

D3D12_RESOURCE_DESC Texture::CreateResourceDescFromMetadata(const DirectX::TexMetadata& metadata)
{
    D3D12_RESOURCE_DESC desc = {};
    desc.MipLevels = static_cast<UINT16>(metadata.mipLevels);
    desc.Format = metadata.format;
    desc.Width = static_cast<UINT>(metadata.width);
    desc.Height = static_cast<UINT>(metadata.height);
    desc.Flags = D3D12_RESOURCE_FLAG_NONE;
    desc.DepthOrArraySize = static_cast<UINT16>(
        metadata.dimension == DirectX::TEX_DIMENSION_TEXTURE3D ? metadata.depth : metadata.arraySize);
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;
    desc.Dimension = static_cast<D3D12_RESOURCE_DIMENSION>(metadata.dimension);
    return desc;
}
