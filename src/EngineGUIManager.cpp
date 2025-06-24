#include "EngineGUIManager.h"
#include "GraphicsEngine.h"
#include "RenderSystem.h"
#include "RenderDevice.h"
#include "DeviceContext.h"
#include "DescriptorHeapManager.h"

std::unique_ptr<EngineGUIManager> EngineGUIManager::sharedInstance = nullptr;

EngineGUIManager* EngineGUIManager::GetInstance()
{
	return sharedInstance.get();
}

void EngineGUIManager::Initialize(HWND hwnd, UINT width, UINT height)
{
	try
	{
		sharedInstance = std::make_unique<EngineGUIManager>(hwnd);
	}
	catch (...)
	{
		Debug::Log("GUI Creation Failed!");
	}
}

void EngineGUIManager::Destroy()
{
	sharedInstance.reset();
	sharedInstance = nullptr;
}

void EngineGUIManager::DrawAllUI()
{
	//for (const auto& uiScreen : m_uiList)
	//{
	//	if (uiScreen)
	//	{
	//		//uiScreen->Draw();
	//	}
	//}
}

EngineGUIManager::EngineGUIManager(HWND hwnd)
{
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	auto renderSystem = GraphicsEngine::GetInstance()->GetRenderSystem();

	// Setup Platform/Renderer backends
	ImGui_ImplDX12_InitInfo init_info = {};
	init_info.Device = renderSystem->GetRenderDevice()->GetD3DDevice();
	init_info.CommandQueue = renderSystem->GetDeviceContext()->GetCommandQueue();
	init_info.NumFramesInFlight = FRAME_COUNT;
	init_info.RTVFormat = DXGI_FORMAT_R8G8B8A8_UNORM; // Or your render target format.

	auto heapManager = renderSystem->GetRenderDevice()->GetDescriptorHeapManager();

	init_info.SrvDescriptorHeap = heapManager->GetSRVHeap();
	init_info.UserData = heapManager;
	init_info.SrvDescriptorAllocFn = 
		+[](ImGui_ImplDX12_InitInfo* info, D3D12_CPU_DESCRIPTOR_HANDLE* out_cpu_handle, D3D12_GPU_DESCRIPTOR_HANDLE* out_gpu_handle)
		{ 
			auto* heapManager = static_cast<DescriptorHeapManager*>(info->UserData);
			UINT index = heapManager->AllocateSRVSlot();
			*out_cpu_handle = heapManager->GetSRVCPUHandleAt(index);
			*out_gpu_handle = heapManager->GetSRVGPUHandleAt(index);
		};

	init_info.SrvDescriptorFreeFn = 
		+[](ImGui_ImplDX12_InitInfo* info, D3D12_CPU_DESCRIPTOR_HANDLE cpu_handle, D3D12_GPU_DESCRIPTOR_HANDLE gpu_handle)
		{
			auto* heapManager = static_cast<DescriptorHeapManager*>(info->UserData);
			SIZE_T base = heapManager->GetSRVCPUHandleAt(0).ptr;
			UINT stride = heapManager->GetSRVDescriptorSize();
			UINT index = static_cast<UINT>((cpu_handle.ptr - base) / stride);
			heapManager->FreeSRVSlot(index);
		};

	ImGui_ImplDX12_Init(&init_info);
	ImGui_ImplWin32_Init(hwnd);
}

EngineGUIManager::~EngineGUIManager()
{
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}