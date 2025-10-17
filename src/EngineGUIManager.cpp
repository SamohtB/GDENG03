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
#include "DarkModeConstants.h"
#include "IconsMaterialDesign.h"

#include <filesystem>
#include <fstream>

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


	String filePath = "Assets/Fonts/Inter_18pt-Regular.ttf";

	io.Fonts->AddFontDefault();

	/* Fonts Loading */
	this->m_defaultFont = io.Fonts->AddFontFromFileTTF(filePath.c_str(), 16.0f, NULL, io.Fonts->GetGlyphRangesDefault());
	io.FontDefault = this->m_defaultFont;

	static const ImWchar icons_ranges[] = { ICON_MIN_MD, ICON_MAX_16_MD, 0 };
	ImFontConfig icons_config;
	icons_config.MergeMode = true;
	icons_config.PixelSnapH = true;
	icons_config.GlyphMinAdvanceX = 16.0f;
	io.Fonts->AddFontFromFileTTF(FONT_ICON_FILE_NAME_MD, 16.0f, &icons_config, icons_ranges);


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
	style.WindowPadding = ImVec2(8.0f, 8.0f);
	style.WindowRounding = 0.0f;
	style.WindowBorderSize = 1.0f;
	style.WindowMinSize = ImVec2(32.0f, 32.0f);
	style.WindowTitleAlign = ImVec2(0.0f, 0.5f);
	style.WindowMenuButtonPosition = ImGuiDir_Left;
	style.ChildRounding = 4.0f;
	style.ChildBorderSize = 1.0f;
	style.PopupRounding = 4.0f;
	style.PopupBorderSize = 1.0f;
	style.FramePadding = ImVec2(6.0f, 4.0f);
	style.FrameRounding = 3.0f;
	style.FrameBorderSize = 1.0f;
	style.ItemSpacing = ImVec2(8.0f, 6.0f);
	style.ItemInnerSpacing = ImVec2(4.0f, 4.0f);
	style.CellPadding = ImVec2(4.0f, 2.0f);
	style.IndentSpacing = 20.0f;
	style.ColumnsMinSpacing = 6.0f;
	style.ScrollbarSize = 14.0f;
	style.ScrollbarRounding = 6.0f;
	style.GrabMinSize = 10.0f;
	style.GrabRounding = 3.0f;
	style.TabRounding = 3.0f;
	style.TabBorderSize = 1.0f;
	//style.TabMinWidthForCloseButton = 0.0f;
	style.ColorButtonPosition = ImGuiDir_Left;


	style.Colors[ImGuiCol_Text] = DarkModeGUIConstants::TEXT;
	style.Colors[ImGuiCol_WindowBg] = DarkModeGUIConstants::WINDOW_BG;
	style.Colors[ImGuiCol_ChildBg] = DarkModeGUIConstants::CHILD_BG;
	style.Colors[ImGuiCol_PopupBg] = DarkModeGUIConstants::POPUP_BG;
	style.Colors[ImGuiCol_Border] = DarkModeGUIConstants::BORDER;
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(1, 1, 1, 1);
	style.Colors[ImGuiCol_FrameBg] = DarkModeGUIConstants::FRAME_BG;
	style.Colors[ImGuiCol_FrameBgHovered] = DarkModeGUIConstants::FRAME_BG_HOVERED;
	style.Colors[ImGuiCol_FrameBgActive] = DarkModeGUIConstants::FRAME_BG_ACTIVE;
	style.Colors[ImGuiCol_TitleBg] = DarkModeGUIConstants::TITLE_BG;
	style.Colors[ImGuiCol_TitleBgActive] = DarkModeGUIConstants::TITLE_BG_ACTIVE;
	style.Colors[ImGuiCol_TitleBgCollapsed] = DarkModeGUIConstants::TITLE_BG_COLLAPSED;
	style.Colors[ImGuiCol_MenuBarBg] = DarkModeGUIConstants::MENU_BAR_BG;
	style.Colors[ImGuiCol_Button] = DarkModeGUIConstants::BUTTON;
	style.Colors[ImGuiCol_ButtonHovered] = DarkModeGUIConstants::BUTTON_HOVERED;
	style.Colors[ImGuiCol_ButtonActive] = DarkModeGUIConstants::BUTTON_ACTIVE;
	style.Colors[ImGuiCol_Header] = DarkModeGUIConstants::HEADER;
	style.Colors[ImGuiCol_HeaderHovered] = DarkModeGUIConstants::HEADER_HOVERED;
	style.Colors[ImGuiCol_HeaderActive] = DarkModeGUIConstants::HEADER_ACTIVE;
	style.Colors[ImGuiCol_Tab] = DarkModeGUIConstants::TAB;
	style.Colors[ImGuiCol_TabUnfocused] = DarkModeGUIConstants::TAB_UNFOCUSED;
	style.Colors[ImGuiCol_TabUnfocusedActive] = DarkModeGUIConstants::TAB_UNFOCUSED_ACTIVE;
	style.Colors[ImGuiCol_TabActive] = DarkModeGUIConstants::TAB_ACTIVE;
	style.Colors[ImGuiCol_TabHovered] = DarkModeGUIConstants::TAB_HOVERED;
	style.Colors[ImGuiCol_TextSelectedBg] = DarkModeGUIConstants::TEXT_SELECTED_BG;
}


EngineGUIManager::~EngineGUIManager()
{
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}