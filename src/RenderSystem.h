#pragma once
#include <memory>
#include "DX12Common.h"

#include "DeviceContext.h"
#include "RenderTargetManager.h"
#include "VertexBuffer.h"
#include "ConstantBuffer.h"

class RenderSystem
{
public:
	RenderSystem(UINT width, UINT height, HWND hwnd);	
	~RenderSystem();

	void StartResourceUpload();
	void EndResourceUpload();

	void StartFrame();
	void EndFrame();

	void UpdateConstantBuffer(float time);

private:

	std::unique_ptr<DeviceContext> m_deviceContext;
	std::unique_ptr<RenderTargetManager> m_renderTargetManager;
	std::shared_ptr<ConstantBuffer> m_constantBuffer;

	CD3DX12_VIEWPORT m_viewport;
	CD3DX12_RECT m_scissorRect;

	UINT m_frameIndex = 0;

	void SwapBuffers();
	void MoveToNextFrame();
	void WaitForGPU();
};

