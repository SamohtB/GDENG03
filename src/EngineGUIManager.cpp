#include "pch.h"
#include "EngineGUIManager.h"
#include "GraphicsEngine.h"
#include "RenderSystem.h"
#include "RenderDevice.h"
#include "DeviceContext.h"
#include "DescriptorHeapManager.h"

#include "AUIScreen.h"

#include "Hierarchy.h"
#include "Toolbar.h"
#include "Inspector.h"
#include "MaterialEditor.h"
#include "DebugWindow.h"
#include "SaveScene.h"
#include "LoadScene.h"

#include "Debug.h"

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
	for (const auto& screen : m_uiList)
	{
		if (screen->IsVisible())
		{
			screen->DrawUI();
		}
	}
}

std::vector<AUIScreen*> EngineGUIManager::GetAllScreens()
{
	std::vector<AUIScreen*> allScreens;

	for (const auto& screen : m_uiList)
	{
		allScreens.push_back(screen.get());
	}

	return allScreens;
}

AUIScreen* EngineGUIManager::GetUI(const String& name)
{
	auto it = m_uiTable.find(name);

	if (it != m_uiTable.end())
	{
		return it->second.get();
	}
	else
	{
		Debug::LogWarning("EngineGUIManager::GetUI: UI with name '" + name + "' not found.");
		return nullptr;
	}
}

EngineGUIManager::EngineGUIManager(HWND hwnd)
{
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;          // Enable Docking

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

	setupStyle();

	PopulateGUI();
}

void EngineGUIManager::PopulateGUI()
{
	auto toolbar = std::make_shared<Toolbar>();
	this->m_uiTable[UINames::TOOLBAR] = toolbar;
	this->m_uiList.push_back(toolbar);

	auto hierarchy = std::make_shared<Hierarchy>();
	this->m_uiTable[UINames::HIERARCHY] = hierarchy;
	this->m_uiList.push_back(hierarchy);

	auto inspector = std::make_shared<Inspector>();
	this->m_uiTable[UINames::INSPECTOR] = inspector;
	this->m_uiList.push_back(inspector);

	auto materialEditor = std::make_shared<MaterialEditor>();
	this->m_uiTable[UINames::MATERIAL_EDITOR] = materialEditor;
	this->m_uiList.push_back(materialEditor);

	auto debugWindow = std::make_shared<DebugWindow>();
	this->m_uiTable[UINames::DEBUG] = debugWindow;
	this->m_uiList.push_back(debugWindow);

	auto saveScene = std::make_shared<SaveScene>();
	this->m_uiTable[UINames::SAVE_SCENE] = saveScene;
	this->m_uiList.push_back(saveScene);

	auto loadScene = std::make_shared<LoadScene>();
	this->m_uiTable[UINames::LOAD_SCENE] = loadScene;
	this->m_uiList.push_back(loadScene);
}

void EngineGUIManager::setupStyle()
{
	// Tokyo Night Storm style from ImThemes
	ImGuiStyle& style = ImGui::GetStyle();

	style.Alpha = 1.0f;
	style.DisabledAlpha = 0.6000000238418579f;
	style.WindowPadding = ImVec2(15.0f, 10.10000038146973f);
	style.WindowRounding = 10.0f;
	style.WindowBorderSize = 1.0f;
	style.WindowMinSize = ImVec2(32.0f, 32.0f);
	style.WindowTitleAlign = ImVec2(0.0f, 0.5f);
	style.WindowMenuButtonPosition = ImGuiDir_Left;
	style.ChildRounding = 5.0f;
	style.ChildBorderSize = 1.0f;
	style.PopupRounding = 5.0f;
	style.PopupBorderSize = 1.0f;
	style.FramePadding = ImVec2(4.0f, 3.0f);
	style.FrameRounding = 4.0f;
	style.FrameBorderSize = 0.0f;
	style.ItemSpacing = ImVec2(8.0f, 4.0f);
	style.ItemInnerSpacing = ImVec2(4.0f, 4.0f);
	style.CellPadding = ImVec2(4.0f, 2.0f);
	style.IndentSpacing = 21.0f;
	style.ColumnsMinSpacing = 6.0f;
	style.ScrollbarSize = 12.0f;
	style.ScrollbarRounding = 9.0f;
	style.GrabMinSize = 10.0f;
	style.GrabRounding = 10.0f;
	style.TabRounding = 4.0f;
	style.TabBorderSize = 0.0f;
	//style.TabMinWidthForCloseButton = 0.0f;
	style.ColorButtonPosition = ImGuiDir_Left;
	style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
	style.SelectableTextAlign = ImVec2(0.0f, 0.0f);

	style.Colors[ImGuiCol_Text] = ImVec4(0.7686f, 0.7686f, 0.7686f, 1.0f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.2549019753932953f, 0.2823529541492462f, 0.407843142747879f, 1.0f);

	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.235f, 0.235f, 0.235f, 1.0f);
	style.Colors[ImGuiCol_ChildBg] = ImVec4(0.196f, 0.196f, 0.196f, 1.0f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(0.235f, 0.235f, 0.235f, 1.0f);

	style.Colors[ImGuiCol_Border] = ImVec4(0, 0, 0, 0);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.2543022036552429f, 0.2832040190696716f, 0.407843142747879f, 0.501960813999176f);

	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.165f, 0.165f, 0.165f, 1.0f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.404f, 0.404f, 0.404f, 1.0f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.275f, 0.376f, 0.486f, 0.6980392336845398f);

	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.063, 0.063, 0.063, 1.0f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.063, 0.063, 0.063, 1.0f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.063, 0.063, 0.063, 1.0f);

	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(1.f, 1.f, 1.f, 1.0f);

	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.01960784383118153f, 0.01960784383118153f, 0.01960784383118153f, 0.5299999713897705f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.3098039329051971f, 0.3098039329051971f, 0.3098039329051971f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.407843142747879f, 0.407843142747879f, 0.407843142747879f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.5098039507865906f, 0.5098039507865906f, 0.5098039507865906f, 1.0f);

	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.7333333492279053f, 0.6039215922355652f, 0.9686274528503418f, 1.0f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.7333333492279053f, 0.6039215922355652f, 0.9686274528503418f, 1.0f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.7942906618118286f, 0.6929580569267273f, 0.9785407781600952f, 1.0f);

	style.Colors[ImGuiCol_Button] = ImVec4(0.345, 0.345, 0.345, 1.0f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.404f, 0.404f, 0.404f, 0.6980392336845398f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.275f, 0.376f, 0.486f, 0.6980392336845398f);

	style.Colors[ImGuiCol_Header] = ImVec4(0.196f, 0.196f, 0.196f, 1.0f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.118f, 0.118, 0.118f, 1.0f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.275f, 0.376f, 0.486f, 0.6980392336845398f);

	style.Colors[ImGuiCol_Separator] = ImVec4(0.4274509847164154f, 0.4274509847164154f, 0.4980392158031464f, 0.5f);
	style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.09803921729326248f, 0.4000000059604645f, 0.7490196228027344f, 0.7799999713897705f);
	style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.09803921729326248f, 0.4000000059604645f, 0.7490196228027344f, 1.0f);

	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.2588235437870026f, 0.5882353186607361f, 0.9764705896377563f, 0.2000000029802322f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.2588235437870026f, 0.5882353186607361f, 0.9764705896377563f, 0.6700000166893005f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.2588235437870026f, 0.5882353186607361f, 0.9764705896377563f, 0.949999988079071f);

	style.Colors[ImGuiCol_Tab] = ImVec4(0.063, 0.063, 0.063, 1.0f);
	style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.235f, 0.235f, 0.235f, 1.0f);
	style.Colors[ImGuiCol_TabActive] = ImVec4(0.235f, 0.235f, 0.235f, 1.0f);

	style.Colors[ImGuiCol_TabHovered] = ImVec4(0.118f, 0.118, 0.118f, 1.0f);
	style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.063, 0.063, 0.063, 1.0f);

	style.Colors[ImGuiCol_PlotLines] = ImVec4(1.0f, 0.6196078658103943f, 0.3921568691730499f, 1.0f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.9686274528503418f, 0.4627451002597809f, 0.5568627715110779f, 1.0f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.8784313797950745f, 0.686274528503418f, 0.407843142747879f, 1.0f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.0f, 0.6196078658103943f, 0.3921568691730499f, 1.0f);

	style.Colors[ImGuiCol_TableHeaderBg] = ImVec4(1, 0, 0, 0);
	style.Colors[ImGuiCol_TableBorderStrong] = ImVec4(0.3098039329051971f, 0.3098039329051971f, 0.3490196168422699f, 1.0f);
	style.Colors[ImGuiCol_TableBorderLight] = ImVec4(0.2274509817361832f, 0.2274509817361832f, 0.2470588237047195f, 1.0f);
	style.Colors[ImGuiCol_TableRowBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
	style.Colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.0f, 1.0f, 1.0f, 0.05999999865889549f);

	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.2588235437870026f, 0.5882353186607361f, 0.9764705896377563f, 0.3499999940395355f);
	style.Colors[ImGuiCol_DragDropTarget] = ImVec4(1.0f, 1.0f, 0.0f, 0.8999999761581421f);

	style.Colors[ImGuiCol_NavHighlight] = ImVec4(0, 0, 1, 1.0f);
	style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.0f, 1.0f, 1.0f, 0.699999988079071f);
	style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.800000011920929f, 0.800000011920929f, 0.800000011920929f, 0.2000000029802322f);
	style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.800000011920929f, 0.800000011920929f, 0.800000011920929f, 0.3499999940395355f);
}


EngineGUIManager::~EngineGUIManager()
{
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}