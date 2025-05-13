#include "RenderSystem.h"
#include "Helper.h"

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
		this->m_swapChainManager->GetSwapChain(), *m_descriptorHeap);

	/* Load Assets */
	/* Pipeline State Manager Temp creates default root signature and pipeline state */
	this->m_pipelineStateManager = std::make_unique<PipelineStateManager>(this->m_deviceManager->GetD3DDevice());
	this->m_commandQueueManager->CreateCommandLists(this->m_deviceManager->GetD3DDevice(), this->m_pipelineStateManager->GetPipelineState());
	this->m_vertexBuffer = std::make_unique<VertexBuffer>(this->m_deviceManager->GetD3DDevice(), static_cast<float>(width) / static_cast<float>(height));
	this->m_fenceManager = std::make_unique<FenceManager>(this->m_deviceManager->GetD3DDevice());

	// Wait for the command list to execute; we are reusing the same command 
	// list in our main loop but for now, we just want to wait for setup to 
	// complete before continuing.
	WaitForPreviousFrame();
}

RenderSystem::~RenderSystem()
{
	WaitForPreviousFrame();
	this->m_fenceManager->CloseEvent();
}

void RenderSystem::CreateFactory()
{
	UINT dxgiFactoryFlags = 0;
	ThrowIfFailed(CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&m_dxgiFactory)));
}

void RenderSystem::PopulateCommandList()
{
	ID3D12CommandAllocator* allocator = this->m_commandQueueManager->GetCommandAllocator().Get();
	ID3D12GraphicsCommandList* list = this->m_commandQueueManager->GetCommandList().Get();
	UINT currentFrameIndex = this->m_swapChainManager->GetCurrentFrameIndex();
	ID3D12Resource* renderTarget = this->m_renderTargetManager->GetRenderTarget(currentFrameIndex).Get();

	ThrowIfFailed(allocator->Reset());
	ThrowIfFailed(list->Reset(allocator, this->m_pipelineStateManager->GetPipelineState().Get()));

	// Set necessary state.
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

	/* Draw Calls */
	list->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	list->IASetVertexBuffers(0, 1, this->m_vertexBuffer->GetVertexBufferViewPointer());
	list->DrawInstanced(3, 1, 0, 0);

	/* Indicate that the back buffer will now be used to present */
	CD3DX12_RESOURCE_BARRIER barrierToPresent =
		CD3DX12_RESOURCE_BARRIER::Transition(
			renderTarget,
			D3D12_RESOURCE_STATE_RENDER_TARGET,
			D3D12_RESOURCE_STATE_PRESENT
		);

	list->ResourceBarrier(1, &barrierToPresent);
	ThrowIfFailed(list->Close());
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

void RenderSystem::WaitForPreviousFrame()
{
	// WAITING FOR THE FRAME TO COMPLETE BEFORE CONTINUING IS NOT BEST PRACTICE.
	// This is code implemented as such for simplicity. The D3D12HelloFrameBuffering
	// sample illustrates how to use fences for efficient resource usage and to
	// maximize GPU utilization.

	ID3D12Fence* _fence = this->m_fenceManager->GetFence().Get();
	HANDLE fenceEvent = this->m_fenceManager->GetFenceEvent();

	// Signal and increment the fence value.
	const UINT64 fence = this->m_fenceManager->GetFenceValue();
	ThrowIfFailed(this->m_commandQueueManager->GetCommandQueue()->Signal(_fence, fence));
	this->m_fenceManager->IncrementFence();

	// Wait until the previous frame is finished.
	if (_fence->GetCompletedValue() < fence)
	{
		ThrowIfFailed(_fence->SetEventOnCompletion(fence, fenceEvent));
		WaitForSingleObject(fenceEvent, INFINITE);
	}

	this->m_swapChainManager->SetFrameIndex();
}