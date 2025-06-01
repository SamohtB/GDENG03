#include "GraphicsEngine.h"

#include "RenderDevice.h"
#include "Debug.h"

GraphicsEngine* GraphicsEngine::sharedInstance = nullptr;

GraphicsEngine::GraphicsEngine(UINT width, UINT height, HWND hwnd)
{
	try
	{
		this->m_renderSystem = std::make_unique<RenderSystem>(width, height, hwnd);
	}
	catch (...)
	{
		Debug::LogError("Render System initialization failed!");
		return;
	}

	auto dvcPtr = this->m_renderSystem->GetD3DDevicePtr();
	auto descHeap = this->m_renderSystem->GetRenderDevice()->GetDescriptorHeapManagerPtr();

	try
	{
		this->m_batchUploader = std::make_shared<BatchUploader>(dvcPtr);
	}
	catch (...)
	{
		Debug::LogError("Batch Uploader initialization failed!");
		return;
	}

	try 
	{
		this->m_textureManager = std::make_unique<TextureManager>(descHeap, this->m_batchUploader);
	}
	catch (...)
	{
		Debug::LogError("Texture Manager initialization failed!");
		return;
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
		Debug::LogError("Graphics Engine initialization failed!");
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

BatchUploader* GraphicsEngine::GetBatchUploader()
{
	return this->m_batchUploader.get();
}

TextureManager* GraphicsEngine::GetTextureManager()
{
	return this->m_textureManager.get();
}

