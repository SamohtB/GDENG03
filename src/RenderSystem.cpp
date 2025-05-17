#include "RenderSystem.h"
#include "Helper.h"
#include <iostream>

RenderSystem::RenderSystem(UINT width, UINT height, HWND hwnd) :
	m_viewport(0.0f, 0.0f, static_cast<float>(width), static_cast<float>(height)),
	m_scissorRect(0, 0, static_cast<LONG>(width), static_cast<LONG>(height))
{
	/* Load Pipeline */
	CreateFactory();
	this->m_deviceManager = std::make_unique<DeviceManager>(this->m_dxgiFactory);
	this->m_commandQueueManager = std::make_unique<CommandQueueManager>(this->m_deviceManager->GetD3DDevice());
	this->m_swapChainManager = std::make_unique<SwapChainManager>(this->m_dxgiFactory, 
		this->m_commandQueueManager->GetCommandQueue(), width, height, hwnd);
	this->m_descriptorHeap = std::make_unique<DescriptorHeapManager>(this->m_deviceManager->GetD3DDevice());
	this->m_renderTargetManager = std::make_unique<RenderTargetManager>(this->m_deviceManager->GetD3DDevice(), 
		this->m_swapChainManager->GetSwapChain(), *this->m_descriptorHeap);

	/* Load Assets */
	/* Pipeline State Manager Temp creates default root signature and pipeline state */
	this->m_pipelineStateManager = std::make_unique<PipelineStateManager>(this->m_deviceManager->GetD3DDevice());
	UINT currentFrameIndex = this->m_swapChainManager->GetCurrentFrameIndex();
	this->m_commandQueueManager->CreateCommandLists(this->m_deviceManager->GetD3DDevice(), 
		this->m_pipelineStateManager->GetPipelineState(), currentFrameIndex);
	this->m_fenceManager = std::make_unique<FenceManager>(this->m_deviceManager->GetD3DDevice(),
		*this->m_swapChainManager);

	// Wait for the command list to execute; we are reusing the same command 
	// list in our main loop but for now, we just want to wait for setup to 
	// complete before continuing.
	WaitForGPU();
}

RenderSystem::~RenderSystem()
{
	WaitForGPU();
	this->m_fenceManager->CloseEvent();
}

void RenderSystem::CreateFactory()
{
	UINT dxgiFactoryFlags = 0;
	ThrowIfFailed(CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&m_dxgiFactory)));
}

void RenderSystem::StartFrame()
{
	UINT currentFrameIndex = this->m_swapChainManager->GetCurrentFrameIndex();
	ID3D12CommandAllocator* allocator = this->m_commandQueueManager->GetCommandAllocator(currentFrameIndex).Get();
	ID3D12GraphicsCommandList* list = this->m_commandQueueManager->GetCommandList().Get();
	ID3D12Resource* renderTarget = this->m_renderTargetManager->GetRenderTarget(currentFrameIndex).Get();

	/* Reset Allocator and Command List */

	// Command list allocators can only be reset when the associated 
	// command lists have finished execution on the GPU; apps should use 
	// fences to determine GPU execution progress.
	ThrowIfFailed(allocator->Reset());
	// However, when ExecuteCommandList() is called on a particular command 
	// list, that command list can then be reset at any time and must be before 
	// re-recording.
	ThrowIfFailed(list->Reset(allocator, this->m_pipelineStateManager->GetPipelineState().Get()));

	list->SetGraphicsRootSignature(this->m_pipelineStateManager->GetRootSignature().Get());
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
		this->m_descriptorHeap->GetRTVHeapStart(),
		currentFrameIndex,
		this->m_descriptorHeap->GetRTVDescriptorSize()
	);

	list->OMSetRenderTargets(1, &rtvHandle, FALSE, nullptr);

	const float clearColor[] = { 0.0f, 0.2f, 0.4f, 1.0f };
	list->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
}

void RenderSystem::EndFrame()
{
	ID3D12GraphicsCommandList* list = this->m_commandQueueManager->GetCommandList().Get();
	UINT currentFrameIndex = this->m_swapChainManager->GetCurrentFrameIndex();
	ID3D12Resource* renderTarget = this->m_renderTargetManager->GetRenderTarget(currentFrameIndex).Get();

	/* Indicate that the back buffer will now be used to present */
	CD3DX12_RESOURCE_BARRIER barrierToPresent =
		CD3DX12_RESOURCE_BARRIER::Transition(
			renderTarget,
			D3D12_RESOURCE_STATE_RENDER_TARGET,
			D3D12_RESOURCE_STATE_PRESENT
		);

	list->ResourceBarrier(1, &barrierToPresent);
	ThrowIfFailed(list->Close());

	ExecuteCommandList();
	SwapBuffers();
	MoveToNextFrame();
}

ID3D12GraphicsCommandList* RenderSystem::GetCommandList()
{
	return this->m_commandQueueManager->GetCommandList().Get();
}

ComPtr<ID3D12Device> RenderSystem::GetD3DDevice()
{
	return this->m_deviceManager->GetD3DDevice();
}

void RenderSystem::ExecuteCommandList()
{
	ID3D12CommandList* ppCommandLists[] = { this->m_commandQueueManager->GetCommandList().Get() };
	this->m_commandQueueManager->GetCommandQueue()->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);
}

void RenderSystem::SwapBuffers()
{
	ThrowIfFailed(this->m_swapChainManager->GetSwapChain().Get()->Present(1, 0));
}

void RenderSystem::WaitForGPU()
{
	UINT currentFrameIndex = this->m_swapChainManager->GetCurrentFrameIndex();
	ID3D12Fence* fence = this->m_fenceManager->GetFence().Get();
	UINT64 fenceValue = this->m_fenceManager->GetFenceValue(currentFrameIndex);
	HANDLE fenceEvent = this->m_fenceManager->GetFenceEvent();

	ThrowIfFailed(this->m_commandQueueManager->GetCommandQueue()->Signal(
		fence, fenceValue));

	ThrowIfFailed(fence->SetEventOnCompletion(fenceValue, fenceEvent));
	WaitForSingleObjectEx(fenceEvent, INFINITE, FALSE);

	this->m_fenceManager->SetFenceValue(currentFrameIndex, fenceValue + 1);
}

void RenderSystem::MoveToNextFrame()
{
	ID3D12Fence* fence = this->m_fenceManager->GetFence().Get();
	UINT currentFrameIndex = this->m_swapChainManager->GetCurrentFrameIndex();
	UINT64 currentFenceValue = this->m_fenceManager->GetFenceValue(currentFrameIndex);

	ThrowIfFailed(this->m_commandQueueManager->GetCommandQueue()->Signal(fence, currentFenceValue));
	
	this->m_swapChainManager->UpdateFrameIndex();

	UINT nextFrameIndex = this->m_swapChainManager->GetCurrentFrameIndex();
	UINT64 nextFenceValue = this->m_fenceManager->GetFenceValue(nextFrameIndex); 
	HANDLE fenceEvent = this->m_fenceManager->GetFenceEvent();

	// If the next frame is not ready to be rendered yet, wait until it is ready.
	if (fence->GetCompletedValue() < nextFenceValue)
	{
		ThrowIfFailed(fence->SetEventOnCompletion(nextFenceValue, fenceEvent));
		WaitForSingleObjectEx(fenceEvent, INFINITE, FALSE);
	}

	// Set the fence value for the next frame.
	this->m_fenceManager->SetFenceValue(nextFrameIndex, currentFenceValue + 1);
}
