#pragma once
#include <memory>
#include "stdafx.h"

#include "RenderSystem.h"

class GraphicsEngine
{
public:
	static GraphicsEngine* GetInstance();
	static void Initialize(UINT width, UINT height);
	static void Destroy();

	RenderSystem* GetRenderSystem();
	void RenderFrame();
	void CloseRenderSystem();

private:
	GraphicsEngine();
	~GraphicsEngine() = default;
	GraphicsEngine(GraphicsEngine const&) {}
	GraphicsEngine& operator=(GraphicsEngine const&) {}

	static GraphicsEngine* sharedInstance;

	std::unique_ptr<RenderSystem> m_renderSystem = nullptr;
};

