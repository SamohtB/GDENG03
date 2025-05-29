#include "RenderSystem.h"
#include "Helper.h"
#include "TextureManager.h"
#include <iostream>

RenderSystem::RenderSystem(UINT width, UINT height, HWND hwnd) :
	m_viewport(0.0f, 0.0f, static_cast<float>(width), static_cast<float>(height)),
	m_scissorRect(0, 0, static_cast<LONG>(width), static_cast<LONG>(height))
{
	/* Load Pipeline */
	CreateFactory();
	this->m_deviceManager = std::make_unique<DeviceManager>(this->m_dxgiFactory.Get());
	this->m_commandQueueManager = std::make_unique<CommandQueueManager>(this->m_deviceManager->GetD3DDevice());
	this->m_swapChainManager = std::make_unique<SwapChainManager>(this->m_dxgiFactory.Get(),
		this->m_commandQueueManager->GetCommandQueue(), width, height, hwnd);
	this->m_descriptorHeap = std::make_unique<DescriptorHeapManager>(this->m_deviceManager->GetD3DDevice());
	this->m_renderTargetManager = std::make_unique<RenderTargetManager>(this->m_deviceManager->GetD3DDevice(), 
		this->m_swapChainManager->GetSwapChain(), *this->m_descriptorHeap);

	/* Load Assets */
	/* Pipeline State Manager Temp creates default root signature and pipeline state */
	this->m_pipelineStateManager = std::make_unique<PipelineStateManager>(this->m_deviceManager->GetD3DDevice());
	UINT currentFrameIndex = this->m_swapChainManager->GetCurrentFrameIndex();
	this->m_commandQueueManager->CreateCommandLists(this->m_deviceManager->GetD3DDevice(), currentFrameIndex);
	this->m_fenceManager = std::make_unique<FenceManager>(this->m_deviceManager->GetD3DDevice(),
		*this->m_swapChainManager);

	UINT frameIndex = this->m_swapChainManager->GetCurrentFrameIndex();
	this->m_fenceManager->IncrementFenceValueAtIndex(frameIndex);

	/* Initial Signal */
	ThrowIfFailed(
		this->m_commandQueueManager->GetCommandQueue()->Signal(
			this->m_fenceManager->GetFence(),
			this->m_fenceManager->GetFenceValue(frameIndex
			)));
}

RenderSystem::~RenderSystem()
{
	WaitForGPU();
	this->m_fenceManager->CloseEvent();
}

void RenderSystem::StartResourceUpload()
{
	UINT currentFrameIndex = this->m_swapChainManager->GetCurrentFrameIndex();
	this->m_fenceManager->WaitForFrameGPU(currentFrameIndex); // ensure safe reset
	this->m_commandQueueManager->ResetCommands(currentFrameIndex);
}

void RenderSystem::EndResourceUpload()
{
	ThrowIfFailed(this->m_commandQueueManager->GetCommandList()->Close());

	this->m_commandQueueManager->ExecuteCommandList();

	UINT frameIndex = this->m_swapChainManager->GetCurrentFrameIndex();
	this->m_fenceManager->IncrementFenceValueAtIndex(frameIndex);

	ThrowIfFailed(
		this->m_commandQueueManager->GetCommandQueue()->Signal(
			this->m_fenceManager->GetFence(), 
			this->m_fenceManager->GetFenceValue(frameIndex
			)));
}

void RenderSystem::StartFrame()
{
	UINT currentFrameIndex = this->m_swapChainManager->GetCurrentFrameIndex();
	this->m_fenceManager->WaitForFrameGPU(currentFrameIndex);
	this->m_commandQueueManager->ResetCommands(currentFrameIndex);
	
	ID3D12GraphicsCommandList* list = this->m_commandQueueManager->GetCommandList();
	ID3D12Resource* renderTarget = this->m_renderTargetManager->GetRenderTarget(currentFrameIndex);

	/* pipeline state and root signature can moved to game objects to have their own PSOs and Roots */
	list->SetGraphicsRootSignature(this->m_pipelineStateManager->GetRootSignature());
	list->SetPipelineState(m_pipelineStateManager->GetPipelineState(InputLayoutType::Pos_Tex_Color, L"Default"));

	auto srvHeap = TextureManager::GetInstance()->GetSRVHeap();

	/* Set Heaps SRV/CBV/UAV */
	ID3D12DescriptorHeap* ppHeaps[] = { srvHeap };
	list->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	list->RSSetViewports(1, &m_viewport);
	list->RSSetScissorRects(1, &m_scissorRect);

	/* Indicate that the back buffer will be used as a render target */
	CD3DX12_RESOURCE_BARRIER barrierToRenderTarget =
		CD3DX12_RESOURCE_BARRIER::Transition(
			renderTarget,
			D3D12_RESOURCE_STATE_PRESENT,
			D3D12_RESOURCE_STATE_RENDER_TARGET
		);

	list->ResourceBarrier(1, &barrierToRenderTarget);

	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(
		this->m_descriptorHeap->GetRTVHeap()->GetCPUDescriptorHandleForHeapStart(),
		currentFrameIndex,
		this->m_descriptorHeap->GetRTVDescriptorSize()
	);

	const float clearColor[] = { 0.0f, 0.2f, 0.4f, 1.0f };
	list->OMSetRenderTargets(1, &rtvHandle, FALSE, nullptr);
	list->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
}

void RenderSystem::EndFrame()
{
	ID3D12GraphicsCommandList* list = this->m_commandQueueManager->GetCommandList();
	UINT currentFrameIndex = this->m_swapChainManager->GetCurrentFrameIndex();
	ID3D12Resource* renderTarget = this->m_renderTargetManager->GetRenderTarget(currentFrameIndex);

	/* Indicate that the back buffer will now be used to present */
	CD3DX12_RESOURCE_BARRIER barrierToPresent =
		CD3DX12_RESOURCE_BARRIER::Transition(
			renderTarget,
			D3D12_RESOURCE_STATE_RENDER_TARGET,
			D3D12_RESOURCE_STATE_PRESENT
		);

	list->ResourceBarrier(1, &barrierToPresent);
	ThrowIfFailed(list->Close());

	this->m_commandQueueManager->ExecuteCommandList();
	SwapBuffers();
	MoveToNextFrame();
}

ID3D12GraphicsCommandList* RenderSystem::GetCommandList() const
{
	return this->m_commandQueueManager->GetCommandList();
}

ID3D12CommandQueue* RenderSystem::GetCommandQueue() const
{
	return this->m_commandQueueManager->GetCommandQueue();
}

ID3D12Device* RenderSystem::GetD3DDevice() const 
{
	return this->m_deviceManager->GetD3DDevice();
}

DescriptorHeapManager* RenderSystem::GetDescriptorHeapManager() const
{
	return m_descriptorHeap.get();
}

void RenderSystem::CreateFactory()
{
	UINT dxgiFactoryFlags = 0;
	ThrowIfFailed(CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&m_dxgiFactory)));
}

void RenderSystem::SwapBuffers()
{
	ThrowIfFailed(this->m_swapChainManager->GetSwapChain()->Present(1, 0));
}

void RenderSystem::MoveToNextFrame()
{
	UINT currentFrameIndex = this->m_swapChainManager->GetCurrentFrameIndex();
	UINT64 signaledFenceValue = this->m_fenceManager->IncrementFenceValueAtIndex(currentFrameIndex);

	ThrowIfFailed(
		this->m_commandQueueManager->GetCommandQueue()->Signal(
			this->m_fenceManager->GetFence(), signaledFenceValue
		));

	this->m_swapChainManager->UpdateFrameIndex();

	UINT nextFrameIndex = this->m_swapChainManager->GetCurrentFrameIndex();
	this->m_fenceManager->WaitForFrameGPU(nextFrameIndex);
}

void RenderSystem::WaitForGPU()
{
	UINT currentFrameIndex = this->m_swapChainManager->GetCurrentFrameIndex();
	ID3D12Fence* fence = this->m_fenceManager->GetFence();
	HANDLE fenceEvent = this->m_fenceManager->GetFenceEvent();

	UINT64 fenceValue = this->m_fenceManager->IncrementFenceValueAtIndex(currentFrameIndex);
	ThrowIfFailed(this->m_commandQueueManager->GetCommandQueue()->Signal(fence, fenceValue));

	ThrowIfFailed(fence->SetEventOnCompletion(fenceValue, fenceEvent));
	WaitForSingleObjectEx(fenceEvent, INFINITE, FALSE);
}