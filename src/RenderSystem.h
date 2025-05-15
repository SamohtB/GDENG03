#pragma once
#include <memory>
#include "stdafx.h"

#include "DeviceManager.h"
#include "CommandQueueManager.h"
#include "SwapChainManager.h"
#include "DescriptorHeapManager.h"
#include "RenderTargetManager.h"
#include "FenceManager.h"
#include "PipelineStateManager.h"

#include "VertexBuffer.h"

class RenderSystem
{
public:
	RenderSystem(UINT width, UINT height, HWND hwnd);	
	~RenderSystem();

	void StartFrame();
	void EndFrame();

	ID3D12GraphicsCommandList* GetCommandList();
	ComPtr<ID3D12Device> GetD3DDevice();

private:
	ComPtr<IDXGIFactory6> m_dxgiFactory;

	std::unique_ptr<DeviceManager> m_deviceManager;
	std::unique_ptr<CommandQueueManager> m_commandQueueManager;
	std::unique_ptr<SwapChainManager> m_swapChainManager;
	std::unique_ptr<DescriptorHeapManager> m_descriptorHeap;
	std::unique_ptr<RenderTargetManager> m_renderTargetManager;
	std::unique_ptr<FenceManager> m_fenceManager;
	std::unique_ptr<PipelineStateManager> m_pipelineStateManager;

	CD3DX12_VIEWPORT m_viewport;
	CD3DX12_RECT m_scissorRect;

	void CreateFactory();

	void ExecuteCommandList();
	void SwapBuffers();
	void WaitForPreviousFrame();
};

