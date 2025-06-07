#include "RenderSystem.h"

#include "RenderDevice.h"
#include "DeviceContext.h"
#include "SwapChainManager.h"
#include "RenderTargetManager.h"
#include "MaterialBuffer.h"

#include "Debug.h"

#include "TextureManager.h"
#include "MaterialManager.h"

/* Sub Classes */
#include "FenceManager.h"
#include "PipelineStateManager.h"
#include "DescriptorHeapManager.h"

RenderSystem::RenderSystem(UINT width, UINT height, HWND hwnd) :
	m_viewport(0.0f, 0.0f, static_cast<float>(width), static_cast<float>(height)),
	m_scissorRect(0, 0, static_cast<LONG>(width), static_cast<LONG>(height))
{
	this->m_renderDevice = std::make_unique<RenderDevice>();
	auto d3dDevice = this->m_renderDevice->GetD3DDevice();

	this->m_deviceContext = std::make_unique<DeviceContext>(d3dDevice);
	this->m_swapChainManager = std::make_unique<SwapChainManager>(this->m_renderDevice->GetFactory(),
		this->m_deviceContext->GetCommandQueue(), width, height, hwnd);

	auto heapManager = this->m_renderDevice->GetDescriptorHeapManager();

	this->m_renderTargetManager = std::make_unique<RenderTargetManager>(d3dDevice, this->m_swapChainManager->GetSwapChain(),
		heapManager->GetRTVHeap()->GetCPUDescriptorHandleForHeapStart(), heapManager->GetRTVDescriptorSize());

	this->m_globalBuffer = std::make_unique<GlobalBuffer>(d3dDevice);

	/* Initial Signal */
	this->m_renderDevice->GetFenceManager()->SignalCurrentFrameGPU(this->m_deviceContext->GetCommandQueue(), 0);
}

RenderSystem::~RenderSystem()
{
	auto commandQueue = this->m_deviceContext->GetCommandQueue();
	auto fenceManager = this->m_renderDevice->GetFenceManager();

	for (UINT frameIndex = 0; frameIndex < FRAME_COUNT; ++frameIndex)
	{
		fenceManager->SignalCurrentFrameGPU(commandQueue, frameIndex);
		fenceManager->WaitForFrameGPU(frameIndex);
	}

	fenceManager->ShutDown();
}

void RenderSystem::InitResourceManagers(std::shared_ptr<BatchUploader> uploader)
{
	try
	{
		this->m_textureManager = std::make_unique<TextureManager>(m_renderDevice->GetDescriptorHeapManagerPtr(), uploader);
	}
	catch (...)
	{
		Debug::LogError("Texture Manager initialization failed!");
		return;
	}

	try
	{
		this->m_materialManager = std::make_unique<MaterialManager>(m_renderDevice->GetDescriptorHeapManagerPtr(), uploader);
	}
	catch (...)
	{
		Debug::LogError("Material Manager initialization failed!");
		return;
	}
}

void RenderSystem::LoadInitialResources()
{
	this->m_textureManager->LoadInitialTextures();
	this->m_materialManager->LoadInitialMaterials();
}

void RenderSystem::StartFrame()
{
	UINT currentFrameIndex = this->m_swapChainManager->GetCurrentFrameIndex();
	
	this->m_renderDevice->GetFenceManager()->WaitForFrameGPU(currentFrameIndex);
	this->m_deviceContext->ResetCommands(currentFrameIndex);

	/* Set ENV Can Be Moved */
	{
		this->m_deviceContext->SetRootSignature(this->m_renderDevice->GetPSOManager()->GetRootSignature());

		/* Set Heaps SRV/CBV/UAV */
		auto heaps = this->m_renderDevice->GetDescriptorHeapManager()->GetActiveHeaps();
		this->m_deviceContext->SetDescriptorHeaps(heaps);

		this->m_deviceContext->SetTexture(
			this->m_renderDevice->GetDescriptorHeapManager()->GetShaderVisibleGPUHandleAt(0));

		this->m_deviceContext->SetViewport(&m_viewport);
		this->m_deviceContext->SetScissorRect(&m_scissorRect);
	}
	
	auto renderTarget = this->m_renderTargetManager->GetRenderTarget(currentFrameIndex);
	this->m_deviceContext->TransitionToRenderTarget(renderTarget);

	auto rtvHandle = this->m_renderDevice->GetDescriptorHeapManager()->GetRTVCPUHandleAt(currentFrameIndex);
	this->m_deviceContext->ClearRenderTargetColor(rtvHandle, 0.0f, 0.2f, 0.4f, 1.0f);
}

void RenderSystem::EndFrame()
{
	UINT currentFrameIndex = this->m_swapChainManager->GetCurrentFrameIndex();
	auto renderTarget = this->m_renderTargetManager->GetRenderTarget(currentFrameIndex);
	this->m_deviceContext->TransitionToPresent(renderTarget);
	this->m_deviceContext->ExecuteCommandList();

	this->m_swapChainManager->PresentFrame();

	this->m_renderDevice->GetFenceManager()->SignalCurrentFrameGPU(this->m_deviceContext->GetCommandQueue(), currentFrameIndex);
	this->m_swapChainManager->UpdateFrameIndex();
}

void RenderSystem::UpdateGlobalBuffer(float time)
{
	auto index = this->m_swapChainManager->GetCurrentFrameIndex();
	this->m_globalBuffer->Update(time, index);
}

void RenderSystem::SetMaterialConstantBuffer(MaterialType type)
{
	auto index = this->m_swapChainManager->GetCurrentFrameIndex();
	auto handle = m_materialManager->GetMaterialHandle(type, index);
	this->m_deviceContext->SetMaterialBuffer(handle);
}

D3D12_GPU_VIRTUAL_ADDRESS RenderSystem::GetGlobalBufferAddress()
{
	auto index = this->m_swapChainManager->GetCurrentFrameIndex();
	return this->m_globalBuffer->GetVirtualAddress(index);
}

ID3D12PipelineState* RenderSystem::GetPipelineState(const ShaderType& type) const
{
	auto psoManager = this->m_renderDevice->GetPSOManager();

	switch (type)
	{
	case TEXTURED: return psoManager->GetPipelineState(InputLayoutType::Pos_Tex_Col, L"Textured");
	case ANIMATED: return psoManager->GetPipelineState(InputLayoutType::Pos_Pos_Col_Col, L"Animated");
	case PBS: return psoManager->GetPipelineState(InputLayoutType::Pos_Tex_Nor_Tan_Bit, L"PBS");
	default: return psoManager->GetPipelineState(InputLayoutType::Pos_Color, L"Default");
	}
	
}

ComPtr<ID3D12Device> RenderSystem::GetD3DDevicePtr()
{
	return this->m_renderDevice->GetD3DDevicePtr();
}

RenderDevice* RenderSystem::GetRenderDevice()
{
	return this->m_renderDevice.get();
}

DeviceContext* RenderSystem::GetDeviceContext()
{
	return this->m_deviceContext.get();
}
