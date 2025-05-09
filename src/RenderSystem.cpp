#include "RenderSystem.h"
#include "Helper.h"

#include "DeviceManager.h"
#include "CommandQueueManager.h"
#include "SwapChainManager.h"
#include "DescriptorHeapManager.h"
#include "RenderTargetManager.h"

RenderSystem::RenderSystem(UINT width, UINT height) : ABaseRenderer(width, height, L"DirectX Application")
{
	LoadPipeline();
	LoadAssets();
}

RenderSystem::~RenderSystem()
{
}

void RenderSystem::CreateFactory()
{
	UINT dxgiFactoryFlags = 0;
	ThrowIfFailed(CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&m_dxgiFactory)));
}

void RenderSystem::OnInit()
{
}

void RenderSystem::OnUpdate()
{
}

void RenderSystem::OnRender()
{
}

void RenderSystem::OnDestroy()
{
}

void RenderSystem::LoadPipeline()
{
	CreateFactory();
	this->m_deviceManager = std::make_unique<DeviceManager>(this->m_dxgiFactory);
	this->m_commandQueueManager = std::make_unique<CommandQueueManager>(this->m_deviceManager->GetD3DDevice());
	this->m_swapChainManager = std::make_unique<SwapChainManager>(this->m_dxgiFactory, this->m_commandQueueManager->GetCommandQueue(), this->m_width, this->m_height);
	this->m_descriptorHeap = std::make_unique<DescriptorHeapManager>(this->m_deviceManager->GetD3DDevice());
	this->m_RenderTargetManager = std::make_unique<RenderTargetManager>(this->m_deviceManager->GetD3DDevice(), this->m_swapChainManager->GetSwapChain(), *m_descriptorHeap);
}

void RenderSystem::LoadAssets()
{
}
