#pragma once
#include <memory>
#include "Dx12Commons.h"
#include <directxtk12/SimpleMath.h>

#include "ShaderTypes.h"
#include "MaterialTypes.h"

#include "ConstantBuffer.h"

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

	void UpdateFrameConstants(const FrameConstantsData& data);
	D3D12_GPU_VIRTUAL_ADDRESS GetFrameConstantsAddress();

	void UpdateMaterialConstants(MaterialType type);
	D3D12_GPU_VIRTUAL_ADDRESS GetMaterialConstantsAddress(MaterialType type);

	ID3D12PipelineState* GetPipelineState(const ShaderType& type) const;
	UINT GetCurrentFrameIndex();

	TextureManager* GetTextureManager();

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

	std::unique_ptr<FrameConstantsBuffer> m_frameConstantsBuffer;

	CD3DX12_VIEWPORT m_viewport;
	CD3DX12_RECT m_scissorRect;

	const int MAX_OBJECT_COUNT = 1000;
};