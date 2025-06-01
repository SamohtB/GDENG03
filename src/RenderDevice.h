#pragma once
#include <memory>
#include "Dx12Commons.h"

#include "DeviceManager.h"
#include "DescriptorHeapManager.h"
#include "PipelineStateManager.h"
#include "FenceManager.h"

class RenderDevice
{
public:
	RenderDevice();
	~RenderDevice() = default;

	ID3D12Device* GetD3DDevice() const;
	ComPtr<ID3D12Device> GetD3DDevicePtr() const;
	IDXGIFactory6* GetFactory() const;

	FenceManager* GetFenceManager() const;
	PipelineStateManager* GetPSOManager() const;
	DescriptorHeapManager* GetDescriptorHeapManager() const;

private:
	ComPtr<IDXGIFactory6> m_dxgiFactory;
	std::unique_ptr<DeviceManager> m_deviceManager;
	std::unique_ptr<FenceManager> m_fenceManager;
	std::unique_ptr<DescriptorHeapManager> m_descriptorHeap;
	std::unique_ptr<PipelineStateManager> m_pipelineStateManager;
};



