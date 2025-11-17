#pragma once
#include "InputListener.h"

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
	inline static const std::string PROJECT_WINDOW = "PROJECT_WINDOW";
};

class AUIScreen;

class EngineGUIManager : public InputListener
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

	void OnLeftMousePressed(const Vector2& mousePos) override;
	void OnLeftMouseReleased(const Vector2& mousePos) override;

	ImFont* GetIconFont() const { return m_IconFont; }

	void DetectAndRecordLayoutChanges();

    EngineGUIManager(HWND hwnd);
    ~EngineGUIManager();
    EngineGUIManager(EngineGUIManager const&) = delete;
    EngineGUIManager& operator=(EngineGUIManager const&) = delete;

private:
    static std::unique_ptr<EngineGUIManager> sharedInstance;

    UIList m_uiList;
    UITable m_uiTable;

	ImFont* m_defaultFont = nullptr;
	ImFont* m_IconFont = nullptr;

	std::string m_lastLayoutSnapshot;

	bool m_isFirstFrame = true;
	bool m_isLeftMouseDown = false;
    bool m_isRecording = false;

    void PopulateGUI();
    std::string GetIniDump();
	void setupStyle();
};