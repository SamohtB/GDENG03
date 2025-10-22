#pragma once

class UINames
{
public:
    inline static const std::string TOOLBAR = "TOOLBAR";
    inline static const std::string HIERARCHY = "HIERARCHY";
    inline static const std::string INSPECTOR = "INSPECTOR";
    inline static const std::string DEBUG = "DEBUG";
    inline static const std::string MATERIAL_EDITOR = "MATERIAL_EDITOR";
	inline static const std::string SAVE_SCENE = "SAVE_SCENE";
	inline static const std::string LOAD_SCENE = "LOAD_SCENE";
};

class AUIScreen;

class EngineGUIManager
{
public:
    using UIPtr = std::shared_ptr<AUIScreen>;
    using UIList = std::vector<UIPtr>;
    using UITable = std::unordered_map<String, UIPtr>;

    static EngineGUIManager* GetInstance();
    static void Initialize(HWND hwnd, UINT width, UINT height);
    static void Destroy();

    void DrawAllUI();
    std::vector<AUIScreen*> GetAllScreens();
    AUIScreen* GetUI(const String& name);

	ImFont* GetIconFont() const { return m_IconFont; }

    EngineGUIManager(HWND hwnd);
    ~EngineGUIManager();
    EngineGUIManager(EngineGUIManager const&) = delete;
    EngineGUIManager& operator=(EngineGUIManager const&) = delete;

    static void BeginToolbarRegion(const char* id, float height = 32.0f)
    {
        ImGui::BeginChild(id, ImVec2(0, height), false, ImGuiWindowFlags_NoScrollbar);
    }

    static void EndToolbarRegion()
    {
        ImGui::EndChild();
        ImGui::Separator();
    }

private:
    static std::unique_ptr<EngineGUIManager> sharedInstance;

    UIList m_uiList;
    UITable m_uiTable;

	ImFont* m_defaultFont = nullptr;
	ImFont* m_IconFont = nullptr;

    void PopulateGUI();
	void setupStyle();


};