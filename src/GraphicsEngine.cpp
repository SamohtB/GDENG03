#include "GraphicsEngine.h"
#include "Helper.h"

GraphicsEngine* GraphicsEngine::sharedInstance = nullptr;

GraphicsEngine::GraphicsEngine() {}

GraphicsEngine* GraphicsEngine::GetInstance()
{
	return sharedInstance;
}

void GraphicsEngine::Initialize(UINT width, UINT height)
{
	try
	{
		sharedInstance = new GraphicsEngine();
	}
	catch (...)
	{
		std::runtime_error("Graphics Engine not created");
	}

	try
	{
		sharedInstance->m_renderSystem = std::make_unique<RenderSystem>(width, height);
	}
	catch (...)
	{
		throw std::exception("RenderSystem not created successfully");
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

void GraphicsEngine::RenderFrame()
{
	this->m_renderSystem->PopulateCommandList();
	this->m_renderSystem->ExecuteCommandList();
	this->m_renderSystem->SwapBuffers();
	this->m_renderSystem->WaitForPreviousFrame();
}

void GraphicsEngine::CloseRenderSystem()
{
	this->m_renderSystem->Destroy();
}

