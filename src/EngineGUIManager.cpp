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
#include "Project.h"

#include "Debug.h"
#include "DarkModeConstants.h"
#include "IconsMaterialDesign.h"
#include "EditorTheme.h"

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
	icons_config.GlyphOffset = ImVec2(1.0f, 4.0f);

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

	auto projectWinow = std::make_shared<ProjectWindow>();
	this->m_uiTable[UINames::PROJECT_WINDOW] = projectWinow;
	this->m_uiList.push_back(projectWinow);

	auto saveScene = std::make_shared<SaveScene>();
	this->m_uiTable[UINames::SAVE_SCENE] = saveScene;
	this->m_uiList.push_back(saveScene);

	auto loadScene = std::make_shared<LoadScene>();
	this->m_uiTable[UINames::LOAD_SCENE] = loadScene;
	this->m_uiList.push_back(loadScene);


}

void EngineGUIManager::setupStyle()
{
	ImGuiStyle& style = ImGui::GetStyle();

	style.Alpha = 1.0f;
	style.DisabledAlpha = 0.6000000238418579f;
	style.WindowPadding = ImVec2(15.0f, 10.10000038146973f);
	style.WindowRounding = 0.0f;
	style.WindowBorderSize = 1.0f;
	style.WindowMinSize = ImVec2(32.0f, 32.0f);
	style.WindowTitleAlign = ImVec2(0.0f, 0.5f);
	style.WindowMenuButtonPosition = ImGuiDir_Left;
	style.ChildRounding = 5.0f;
	style.ChildBorderSize = 0.0f;
	style.PopupRounding = 5.0f;
	style.PopupBorderSize = 1.0f;
	style.FramePadding = ImVec2(1.0f, 1.0f);
	style.FrameRounding = 0.0f;
	style.FrameBorderSize = 0.0f;
	style.ItemSpacing = ImVec2(4.0f, 4.0f);
	style.ItemInnerSpacing = ImVec2(4.0f, 4.0f);
	style.CellPadding = ImVec2(4.0f, 2.0f);
	style.IndentSpacing = 21.0f;
	style.ColumnsMinSpacing = 6.0f;
	style.ScrollbarSize = 12.0f;
	style.ScrollbarRounding = 9.0f;
	style.GrabMinSize = 10.0f;
	style.GrabRounding = 10.0f;
	style.TabRounding = 0.0f;
	style.TabBorderSize = 0.0f;
	//style.TabMinWidthForCloseButton = 0.0f;
	style.ColorButtonPosition = ImGuiDir_Left;
	style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
	style.SelectableTextAlign = ImVec2(0.0f, 0.0f);

	style.Colors[ImGuiCol_Text] = DarkTheme.TEXT;
	style.Colors[ImGuiCol_TextDisabled] = DarkTheme.TEXT_DISABLED;

	style.Colors[ImGuiCol_WindowBg] = DarkTheme.WINDOW_BG;
	style.Colors[ImGuiCol_ChildBg] = DarkTheme.CHILD_BG;
	style.Colors[ImGuiCol_PopupBg] = DarkTheme.POPUP_BG;

	style.Colors[ImGuiCol_Border] = DarkTheme.BORDER;
	style.Colors[ImGuiCol_BorderShadow] = DarkTheme.BORDER_SHADOW;

	style.Colors[ImGuiCol_FrameBg] = DarkTheme.FRAME_BG;
	style.Colors[ImGuiCol_FrameBgHovered] = DarkTheme.FRAME_BG_HOVERED;
	style.Colors[ImGuiCol_FrameBgActive] = DarkTheme.FRAME_BG_ACTIVE;

	style.Colors[ImGuiCol_TitleBg] = DarkTheme.TITLE_BG;
	style.Colors[ImGuiCol_TitleBgActive] = DarkTheme.TITLE_BG_ACTIVE;
	style.Colors[ImGuiCol_TitleBgCollapsed] = DarkTheme.TITLE_BG_COLLAPSED;

	style.Colors[ImGuiCol_MenuBarBg] = DarkTheme.MENU_BAR_BG;

	style.Colors[ImGuiCol_ScrollbarBg] = DarkTheme.SCROLLBAR_BG;
	style.Colors[ImGuiCol_ScrollbarGrab] = DarkTheme.SCROLLBAR_GRAB;
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = DarkTheme.SCROLLBAR_GRAB_HOVERED;
	style.Colors[ImGuiCol_ScrollbarGrabActive] = DarkTheme.SCROLLBAR_GRAB_ACTIVE;

	style.Colors[ImGuiCol_CheckMark] = DarkTheme.CHECKMARK;

	style.Colors[ImGuiCol_SliderGrab] = DarkTheme.SLIDER_GRAB;
	style.Colors[ImGuiCol_SliderGrabActive] = DarkTheme.SLIDER_GRAB_ACTIVE;

	style.Colors[ImGuiCol_Button] = DarkTheme.BUTTON;
	style.Colors[ImGuiCol_ButtonHovered] = DarkTheme.BUTTON_HOVERED;
	style.Colors[ImGuiCol_ButtonActive] = DarkTheme.BUTTON_ACTIVE;

	style.Colors[ImGuiCol_Header] = DarkTheme.HEADER;
	style.Colors[ImGuiCol_HeaderHovered] = DarkTheme.HEADER_HOVERED;
	style.Colors[ImGuiCol_HeaderActive] = DarkTheme.HEADER_ACTIVE;

	style.Colors[ImGuiCol_Separator] = DarkTheme.SEPARATOR;
	style.Colors[ImGuiCol_SeparatorHovered] = DarkTheme.SEPARATOR_HOVERED;
	style.Colors[ImGuiCol_SeparatorActive] = DarkTheme.SEPARATOR_ACTIVE;

	style.Colors[ImGuiCol_ResizeGrip] = DarkTheme.RESIZE_GRIP;
	style.Colors[ImGuiCol_ResizeGripHovered] = DarkTheme.RESIZE_GRIP_HOVERED;
	style.Colors[ImGuiCol_ResizeGripActive] = DarkTheme.RESIZE_GRIP_ACTIVE;

	style.Colors[ImGuiCol_Tab] = DarkTheme.TAB;
	style.Colors[ImGuiCol_TabHovered] = DarkTheme.TAB_HOVERED;
	style.Colors[ImGuiCol_TabActive] = DarkTheme.TAB_ACTIVE;
	style.Colors[ImGuiCol_TabUnfocused] = DarkTheme.TAB_UNFOCUSED;
	style.Colors[ImGuiCol_TabUnfocusedActive] = DarkTheme.TAB_UNFOCUSED_ACTIVE;

	style.Colors[ImGuiCol_PlotLines] = DarkTheme.PLOT_LINES;
	style.Colors[ImGuiCol_PlotLinesHovered] = DarkTheme.PLOT_LINES_HOVERED;
	style.Colors[ImGuiCol_PlotHistogram] = DarkTheme.PLOT_HISTOGRAM;
	style.Colors[ImGuiCol_PlotHistogramHovered] = DarkTheme.PLOT_HISTOGRAM_HOVERED;

	style.Colors[ImGuiCol_TableHeaderBg] = DarkTheme.TABLE_HEADER_BG;
	style.Colors[ImGuiCol_TableBorderStrong] = DarkTheme.TABLE_BORDER_STRONG;
	style.Colors[ImGuiCol_TableBorderLight] = DarkTheme.TABLE_BORDER_LIGHT;
	style.Colors[ImGuiCol_TableRowBg] = DarkTheme.TABLE_ROW_BG;
	style.Colors[ImGuiCol_TableRowBgAlt] = DarkTheme.TABLE_ROW_BG_ALT;

	style.Colors[ImGuiCol_TextSelectedBg] = DarkTheme.TEXT_SELECTED_BG;
	style.Colors[ImGuiCol_DragDropTarget] = DarkTheme.DRAG_DROP_TARGET;

	style.Colors[ImGuiCol_NavHighlight] = DarkTheme.NAV_HIGHLIGHT;
	style.Colors[ImGuiCol_NavWindowingHighlight] = DarkTheme.NAV_WINDOWING_HIGHLIGHT;
	style.Colors[ImGuiCol_NavWindowingDimBg] = DarkTheme.NAV_WINDOWING_DIM_BG;
	style.Colors[ImGuiCol_ModalWindowDimBg] = DarkTheme.MODAL_WINDOW_DIM_BG;
}


EngineGUIManager::~EngineGUIManager()
{
	ImGui::DestroyPlatformWindows();
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}