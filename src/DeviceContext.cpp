#include "DeviceContext.h"
#include "SwapChainManager.h"
#include "DeviceManager.h"
#include "CommandQueueManager.h"
#include "DescriptorHeapManager.h"
#include "FenceManager.h"
#include "PipelineStateManager.h"

#include "DxException.h"

DeviceContext::DeviceContext()
{
    THROW_IF_FAILED(CreateDXGIFactory2(0, IID_PPV_ARGS(&m_dxgiFactory)), "Factory Creation Failed!");

    this->m_deviceManager = std::make_unique<DeviceManager>(this->m_dxgiFactory.Get());
    this->m_swapChainManager = std::make_unique<SwapChainManager>();
    this->m_commandQueueManager = std::make_unique<CommandQueueManager>(this->m_deviceManager->GetD3DDevice());
    this->m_descriptorHeapManager = std::make_unique<DescriptorHeapManager>(this->m_deviceManager->GetD3DDevice());
    this->m_pipelineStateManager = std::make_unique<PipelineStateManager>(this->m_deviceManager->GetD3DDevice());
    this->m_fenceManager = std::make_unique<FenceManager>(this->m_deviceManager->GetD3DDevice());
}

ID3D12Device* DeviceContext::GetDevice() const
{
    return this->m_deviceManager->GetD3DDevice();
}

ID3D12GraphicsCommandList* DeviceContext::GetCommandList() const
{
    return this->m_commandQueueManager->GetCommandList();
}

ID3D12CommandQueue* DeviceContext::GetCommandQueue() const
{
    return this->m_commandQueueManager->GetCommandQueue();
}

DescriptorHeapManager* DeviceContext::GetDescriptorHeapManager()
{
    return this->m_descriptorHeapManager.get();
}

FenceManager* DeviceContext::GetFenceManager()
{
    return this->m_fenceManager.get();
}
