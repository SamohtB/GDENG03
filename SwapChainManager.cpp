#include "SwapChainManager.h"
#include "App.h"
#include "Helper.h"

//SwapChainManager::SwapChainManager(ComPtr<ID3D12CommandQueue> commandQueue, UINT width, UINT height, UINT frameCount)
//    : m_commandQueue(commandQueue)
//{
//    UINT dxgiFactoryFlags = 0;
//    ComPtr<IDXGIFactory4> factory;
//    ThrowIfFailed(CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&factory)));
//
//    DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
//    swapChainDesc.BufferCount = frameCount; 
//    swapChainDesc.Width = width;
//    swapChainDesc.Height = height;
//    swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
//    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
//    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
//    swapChainDesc.SampleDesc.Count = 1;
//
//    ComPtr<IDXGISwapChain1> swapChain;
//
//    ThrowIfFailed(factory->CreateSwapChainForHwnd(
//        commandQueue.Get(),
//        App::GetHWND(),
//        &swapChainDesc,
//        nullptr,
//        nullptr,
//        &swapChain
//    ));
//}
//
//SwapChainManager::~SwapChainManager()
//{
//}
