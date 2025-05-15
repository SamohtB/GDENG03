#include "GraphicsEngine.h"

GraphicsEngine* GraphicsEngine::sharedInstance = nullptr;

GraphicsEngine::GraphicsEngine(UINT width, UINT height, HWND hwnd)
{
	try
	{
		this->m_renderSystem = std::make_unique<RenderSystem>(width, height, hwnd);
	}
	catch (...)
	{
		throw std::exception("RenderSystem not created successfully");
	}
}

GraphicsEngine* GraphicsEngine::GetInstance()
{
	return sharedInstance;
}

void GraphicsEngine::Initialize(UINT width, UINT height, HWND hwnd)
{
	try
	{
		sharedInstance = new GraphicsEngine(width, height, hwnd);
	}
	catch (...)
	{
		std::exception("Graphics Engine not created sucessfully");
	}
}

void GraphicsEngine::Destroy()
{
	delete sharedInstance;
}

RenderSystem* GraphicsEngine::GetRenderSystem()
{
	return this->m_renderSystem.get();
}

