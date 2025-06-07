#pragma once
#include <memory>
#include "Dx12Commons.h"
#include <directxtk12/SimpleMath.h>

#include "ShaderTypes.h"
#include "MaterialTypes.h"

#include "GlobalBuffer.h"

class RenderDevice;
class DeviceContext;
class SwapChainManager;
class RenderTargetManager;
class TextureManager;
class MaterialManager;
class BatchUploader;

class RenderSystem
{
public:
	RenderSystem(UINT width, UINT height, HWND hwnd);	
	~RenderSystem();
	
	void InitResourceManagers(std::shared_ptr<BatchUploader> uploader);
	void LoadInitialResources();

	void StartFrame();
	void EndFrame();

	void UpdateGlobalBuffer(float time);
	void SetMaterialConstantBuffer(MaterialType type);
	D3D12_GPU_VIRTUAL_ADDRESS GetGlobalBufferAddress();

	ID3D12PipelineState* GetPipelineState(const ShaderType& type) const;

	ComPtr<ID3D12Device> GetD3DDevicePtr();
	RenderDevice* GetRenderDevice();
	DeviceContext* GetDeviceContext();

private:
	std::unique_ptr<RenderDevice> m_renderDevice;
	std::unique_ptr<DeviceContext> m_deviceContext;
	std::unique_ptr<SwapChainManager> m_swapChainManager;
	std::unique_ptr<RenderTargetManager> m_renderTargetManager;
	std::unique_ptr<TextureManager> m_textureManager;
	std::unique_ptr<MaterialManager> m_materialManager;
	std::unique_ptr<GlobalBuffer> m_globalBuffer;

	CD3DX12_VIEWPORT m_viewport;
	CD3DX12_RECT m_scissorRect;
};