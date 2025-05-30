#pragma once
#include <memory>
#include "DX12Common.h"

class SwapChainManager;
class DeviceManager;
class CommandQueueManager;
class DescriptorHeapManager;
class FenceManager;
class PipelineStateManager;

class DeviceContext
{
public:
	DeviceContext();
	~DeviceContext() = default;

	ID3D12Device* GetDevice() const;
	ID3D12GraphicsCommandList* GetCommandList() const;
	ID3D12CommandQueue* GetCommandQueue() const;

	DescriptorHeapManager* GetDescriptorHeapManager();
	FenceManager* GetFenceManager();

private:

	ComPtr<IDXGIFactory6> m_dxgiFactory;
	std::unique_ptr<DeviceManager> m_deviceManager;
	std::unique_ptr<SwapChainManager> m_swapChainManager;
	std::unique_ptr<CommandQueueManager> m_commandQueueManager;
	std::unique_ptr<DescriptorHeapManager> m_descriptorHeapManager;
	std::unique_ptr<PipelineStateManager> m_pipelineStateManager;
	std::unique_ptr<FenceManager> m_fenceManager;
};

