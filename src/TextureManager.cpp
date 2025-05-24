#include "TextureManager.h"
#include "Helper.h"
#include "GraphicsEngine.h"

TextureManager* TextureManager::sharedInstance = nullptr;

TextureManager* TextureManager::GetInstance()
{
	return sharedInstance;
}

void TextureManager::Initialize()
{
	try
	{
		sharedInstance = new TextureManager();
	}
	catch (...)
	{
		std::exception("Texture Manager not created sucessfully");
	}
}

void TextureManager::Destroy()
{
	delete sharedInstance;
}

TextureManager::TextureManager()
{
    InitializeSRV();

    LoadTexture(TextureType::CRATE, L"Assets/Textures/crate.png");
}

void TextureManager::LoadTexture(const TextureType& type, const std::wstring& filepath)
{
    TexturePtr tex = std::make_shared<Texture>();
    DirectX::TexMetadata metadata;
    DirectX::ScratchImage scratchImage;

    ThrowIfFailed(LoadFromWICFile(filepath.c_str(), DirectX::WIC_FLAGS::WIC_FLAGS_NONE, &metadata, scratchImage));

    auto device = GraphicsEngine::GetInstance()->GetRenderSystem()->GetD3DDevice();
    auto list = GraphicsEngine::GetInstance()->GetRenderSystem()->GetCommandList();
    auto desc = CreateResourceDescFromMetadata(metadata);

    CD3DX12_HEAP_PROPERTIES defaultHeapProps(D3D12_HEAP_TYPE_DEFAULT);
    CD3DX12_HEAP_PROPERTIES uploadHeapProps(D3D12_HEAP_TYPE_UPLOAD);

    ThrowIfFailed(device->CreateCommittedResource(
        &defaultHeapProps,
        D3D12_HEAP_FLAG_NONE,
        &desc,
        D3D12_RESOURCE_STATE_COPY_DEST,
        nullptr,
        IID_PPV_ARGS(&tex->resource)));


    UINT64 uploadBufferSize = GetRequiredIntermediateSize(tex->resource.Get(), 0, 1);
    CD3DX12_RESOURCE_DESC cdDesc = CD3DX12_RESOURCE_DESC::Buffer(uploadBufferSize);

    /* TO DO: Move uploads somewhere else to that it can be cleared after GPU upload */
    ThrowIfFailed(device->CreateCommittedResource(
        &uploadHeapProps,
        D3D12_HEAP_FLAG_NONE,
        &cdDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&tex->uploadHeap)));

    const DirectX::Image* img = scratchImage.GetImage(0, 0, 0);

    D3D12_SUBRESOURCE_DATA textureData = {};
    textureData.pData = img->pixels;
    textureData.RowPitch = img->rowPitch;
    textureData.SlicePitch = img->slicePitch;

    UpdateSubresources(list, tex->resource.Get(), tex->uploadHeap.Get(), 0, 0, 1, &textureData);

    CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
        tex->resource.Get(),
        D3D12_RESOURCE_STATE_COPY_DEST,
        D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE
    );

    list->ResourceBarrier(1, &barrier);

    UINT srvIndex = AllocateSRVSlot();
    m_srvMap[type] = srvIndex;

    // Describe and create a SRV for the texture.
    D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
    srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    srvDesc.Format = desc.Format;
    srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = desc.MipLevels;
    srvDesc.Texture2D.MostDetailedMip = 0;

    this->m_textureList.push_back(tex);

    CD3DX12_CPU_DESCRIPTOR_HANDLE handle(m_srvHeap->GetCPUDescriptorHandleForHeapStart(), srvIndex, m_srvDescriptorSize);
    device->CreateShaderResourceView(tex->resource.Get(), &srvDesc, handle);
}

void TextureManager::InitializeSRV()
{
	auto device = GraphicsEngine::GetInstance()->GetRenderSystem()->GetD3DDevice();

	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
	srvHeapDesc.NumDescriptors = MAX_SRV_COUNT;
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	ThrowIfFailed(device->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&m_srvHeap)));

	m_srvDescriptorSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
}

UINT TextureManager::AllocateSRVSlot()
{
	assert(m_currentSRVOffset < MAX_SRV_COUNT && "SRV heap overflow");
	return m_currentSRVOffset++;
}

CD3DX12_GPU_DESCRIPTOR_HANDLE TextureManager::GetSRVHandle(TextureType type)
{
    auto it = m_srvMap.find(type);
    assert(it != m_srvMap.end() && "Texture type not loaded");

    return CD3DX12_GPU_DESCRIPTOR_HANDLE(m_srvHeap->GetGPUDescriptorHandleForHeapStart(), it->second, m_srvDescriptorSize);
}

ID3D12DescriptorHeap* TextureManager::GetSRVHeap()
{
    return this->m_srvHeap.Get();
}

D3D12_RESOURCE_DESC TextureManager::CreateResourceDescFromMetadata(const DirectX::TexMetadata& metadata)
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
