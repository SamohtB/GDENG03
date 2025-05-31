//#include "TextureManager.h"
//#include "GraphicsEngine.h"
//#include "Helper.h"
//
//#include <directxtk12/ResourceUploadBatch.h>
//#include <directxtk12/WICTextureLoader.h>
//#include <directxtk12/DirectXHelpers.h>
//
//TextureManager::TextureManager()
//{
//    InitializeSRV();
//
//    LoadTexture(TextureType::CRATE, L"Assets/Textures/crate.png");
//}
//
//void TextureManager::LoadTexture(const TextureType& type, const std::wstring& filepath)
//{
//    auto device = GraphicsEngine::GetInstance()->GetRenderSystem()->GetD3DDevice();
//    auto commandQueue = GraphicsEngine::GetInstance()->GetRenderSystem()->GetCommandQueue();
//
//    DirectX::ResourceUploadBatch uploadBatch(device);
//    uploadBatch.Begin();
//
//    TexturePtr tex = std::make_shared<Texture>();
//
//    ThrowIfFailed(CreateWICTextureFromFile(
//        device,
//        uploadBatch,
//        filepath.c_str(),
//        tex->resource.ReleaseAndGetAddressOf(),
//        true // generate mipmaps
//    ));
//
//    auto finished = uploadBatch.End(commandQueue);
//    finished.wait(); // Ensure GPU upload is complete before proceeding
//
//    UINT srvIndex = AllocateSRVSlot();
//    m_srvMap[type] = srvIndex;
//    m_textureList.push_back(tex);
//
//    CD3DX12_CPU_DESCRIPTOR_HANDLE handle(
//        m_srvHeap->GetCPUDescriptorHandleForHeapStart(),
//        srvIndex,
//        m_srvDescriptorSize
//    );
//
//    DirectX::CreateShaderResourceView(device, tex->resource.Get(), handle);
//}