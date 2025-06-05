#pragma once
#include <memory>
#include <windows.h>

#include "RenderSystem.h"
#include "BatchUploader.h"

class GraphicsEngine
{
public:
	static GraphicsEngine* GetInstance();
	static void Initialize(UINT width, UINT height, HWND hwnd);
	static void Destroy();

	RenderSystem* GetRenderSystem();
	BatchUploader* GetBatchUploader();

private:
	GraphicsEngine(UINT width, UINT height, HWND hwnd);
	~GraphicsEngine() = default;
	GraphicsEngine(GraphicsEngine const&) {}
	GraphicsEngine& operator=(GraphicsEngine const&) {}

	static GraphicsEngine* sharedInstance;

	std::unique_ptr<RenderSystem> m_renderSystem = nullptr;
	std::shared_ptr<BatchUploader> m_batchUploader = nullptr;

	friend RenderDevice;
};

