#pragma once
#include <memory>
#include "stdafx.h"
#include "ABaseRenderer.h"

class DeviceManager;
class CommandQueueManager;
class SwapChainManager;
class DescriptorHeapManager;
class RenderTargetManager;

class RenderSystem : public ABaseRenderer
{
public:
	RenderSystem(UINT width, UINT height);	
	~RenderSystem();

	// Inherited via ABaseRenderer
	void OnInit() override;
	void OnUpdate() override;
	void OnRender() override;
	void OnDestroy() override;

private:
	ComPtr<IDXGIFactory6> m_dxgiFactory;

	std::unique_ptr<DeviceManager> m_deviceManager;
	std::unique_ptr<CommandQueueManager> m_commandQueueManager;
	std::unique_ptr<SwapChainManager> m_swapChainManager;
	std::unique_ptr<DescriptorHeapManager> m_descriptorHeap;
	std::unique_ptr<RenderTargetManager> m_RenderTargetManager;

	void LoadPipeline();
	void LoadAssets();
	void CreateFactory();
};

