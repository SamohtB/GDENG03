#include "TextureManager.h"
#include "GraphicsEngine.h"
#include "Helper.h"

#include <directxtk12/ResourceUploadBatch.h>
#include <directxtk12/WICTextureLoader.h>
#include <directxtk12/DirectXHelpers.h>

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
    auto device = GraphicsEngine::GetInstance()->GetRenderSystem()->GetD3DDevice();
    auto commandQueue = GraphicsEngine::GetInstance()->GetRenderSystem()->GetCommandQueue();

    DirectX::ResourceUploadBatch uploadBatch(device);
    uploadBatch.Begin();

    TexturePtr tex = std::make_shared<Texture>();

    ThrowIfFailed(CreateWICTextureFromFile(
        device,
        uploadBatch,
        filepath.c_str(),
        tex->resource.ReleaseAndGetAddressOf(),
        true // generate mipmaps
    ));

    auto finished = uploadBatch.End(commandQueue);
    finished.wait(); // Ensure GPU upload is complete before proceeding

    UINT srvIndex = AllocateSRVSlot();
    m_srvMap[type] = srvIndex;
    m_textureList.push_back(tex);

    CD3DX12_CPU_DESCRIPTOR_HANDLE handle(
        m_srvHeap->GetCPUDescriptorHandleForHeapStart(),
        srvIndex,
        m_srvDescriptorSize
    );

    DirectX::CreateShaderResourceView(device, tex->resource.Get(), handle);
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