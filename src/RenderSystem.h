#pragma once
#include "pch.h"

#include "ShaderTypes.h"
#include "MaterialTypes.h"

#include "ConstantBuffer.h"
#include "DepthBuffer.h"

#include "RenderDevice.h"
#include "DeviceContext.h"
#include "SwapChainManager.h"
#include "RenderTargetManager.h"

class RenderSystem
{
public:
	RenderSystem(UINT width, UINT height, HWND hwnd);	
	~RenderSystem() = default;

	void BeginFrame();
	void EndFrame();

	void UpdateFrameConstants(const FrameConstantsData& data);
	D3D12_GPU_VIRTUAL_ADDRESS GetFrameConstantsAddress();

	ID3D12PipelineState* GetPipelineState(const ShaderType& type) const;
	UINT GetCurrentFrameIndex();

	ComPtr<ID3D12Device> GetD3DDevicePtr();
	RenderDevice* GetRenderDevice();
	DeviceContext* GetDeviceContext();

private:
	std::unique_ptr<DepthBuffer> m_depthBuffer;
	std::unique_ptr<RenderDevice> m_renderDevice; // wraps D3D12 Device Creation
	std::unique_ptr<DeviceContext> m_deviceContext; // wraps commandlists and command queue
	std::unique_ptr<SwapChainManager> m_swapChainManager;
	std::unique_ptr<RenderTargetManager> m_renderTargetManager;

	std::unique_ptr<FrameConstantsBuffer> m_frameConstantsBuffer;

	CD3DX12_VIEWPORT m_viewport;
	CD3DX12_RECT m_scissorRect;
};