#pragma once
#include <memory>
#include "Dx12Commons.h"
#include <directxtk12/SimpleMath.h>

#include "ShaderTypes.h"

class RenderDevice;
class DeviceContext;
class SwapChainManager;
class RenderTargetManager;

class ConstantBuffer;

class RenderSystem
{
public:
	RenderSystem(UINT width, UINT height, HWND hwnd);	
	~RenderSystem();

	void StartFrame();
	void EndFrame();

	void UpdateConstantBuffer(DirectX::SimpleMath::Vector3 values);
	void UpdateAndSetConstantBuffer(DirectX::SimpleMath::Vector3 values);

	ID3D12PipelineState* GetPipelineState(const ShaderType& type) const;

	ComPtr<ID3D12Device> GetD3DDevicePtr();
	RenderDevice* GetRenderDevice();
	DeviceContext* GetDeviceContext();

private:
	std::unique_ptr<RenderDevice> m_renderDevice;
	std::unique_ptr<DeviceContext> m_deviceContext;
	std::unique_ptr<SwapChainManager> m_swapChainManager;
	std::unique_ptr<RenderTargetManager> m_renderTargetManager;

	std::shared_ptr<ConstantBuffer> m_constantBuffer;

	CD3DX12_VIEWPORT m_viewport;
	CD3DX12_RECT m_scissorRect;
};