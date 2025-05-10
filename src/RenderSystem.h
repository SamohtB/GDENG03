#pragma once
#include <memory>
#include "stdafx.h"

#include "DeviceManager.h"
#include "CommandQueueManager.h"
#include "SwapChainManager.h"
#include "DescriptorHeapManager.h"
#include "RenderTargetManager.h"
#include "FenceManager.h"

class RenderSystem
{
public:
	RenderSystem(UINT width, UINT height);	
	~RenderSystem() = default;

	void PopulateCommandList();
	void ExecuteCommandList();
	void SwapBuffers();
	void WaitForPreviousFrame();

	void Destroy();

private:
	ComPtr<IDXGIFactory6> m_dxgiFactory;

	std::unique_ptr<DeviceManager> m_deviceManager;
	std::unique_ptr<CommandQueueManager> m_commandQueueManager;
	std::unique_ptr<SwapChainManager> m_swapChainManager;
	std::unique_ptr<DescriptorHeapManager> m_descriptorHeap;
	std::unique_ptr<RenderTargetManager> m_renderTargetManager;
	std::unique_ptr<FenceManager> m_fenceManager;
	ComPtr<ID3D12PipelineState> m_pipelineState;

	void CreateFactory();
};

