#include "pch.h"
#include "DebugWindow.h"
#include "EngineGUIManager.h"
#include "IconsMaterialDesign.h"

DebugWindow::DebugWindow() : AUIScreen("Debug")
{
	this->m_logLimit = 100; // Default log limit
}

void DebugWindow::DrawUI()
{
	ImGui::PushFont(EngineGUIManager::GetInstance()->GetIconFont());
    ImGui::Begin(ICON_MD_TEXT_SNIPPET "Console");
	ImGui::PopFont();

	EngineGUIManager::BeginToolbarRegion("DebugToolbar", 28.0f);

    // --- Define sizes ---
    float buttonWidth = 80.0f;
    float buttonSpacing = ImGui::GetStyle().ItemSpacing.x;

    // --- Left group: Clear → Editor ---
    if (ImGui::Button("Clear", ImVec2(buttonWidth, 0))) { ClearLog(); }
    ImGui::SameLine();
    if (ImGui::Button("Collapse", ImVec2(buttonWidth, 0))) { /* Collapse logic */ }
    ImGui::SameLine();
    if (ImGui::Button("Error Pause", ImVec2(buttonWidth, 0))) { /* Pause logic */ }
    ImGui::SameLine();
    if (ImGui::Button("Editor", ImVec2(buttonWidth, 0))) { /* Editor logic */ }

    // --- Right group: Search + 3 filter buttons ---
    float searchWidth = 200.0f;

    // total width of search + 3 buttons + spacing
    float rightGroupWidth = searchWidth + (buttonWidth * 3) + (buttonSpacing * 3);

    // align to right edge of window content area
    float rightEdgeX = ImGui::GetContentRegionAvail().x - rightGroupWidth;
    if (rightEdgeX > 0.0f)
        ImGui::SameLine(rightEdgeX);

    // render right-aligned widgets
    static char searchBuffer[128] = "";
    ImGui::SetNextItemWidth(searchWidth);
    ImGui::InputText("##Search", searchBuffer, IM_ARRAYSIZE(searchBuffer));

    ImGui::SameLine();
    if (ImGui::Button(ICON_MD_INFO, ImVec2(buttonWidth, 0))) { /* Info filter */ }
    ImGui::SameLine();
    if (ImGui::Button(ICON_MD_WARNING, ImVec2(buttonWidth, 0))) { /* Warning filter */ }
    ImGui::SameLine();
    if (ImGui::Button(ICON_MD_ERROR, ImVec2(buttonWidth, 0))) { /* Error filter */ }


	EngineGUIManager::EndToolbarRegion();

    ImGui::BeginChild("LogScrollRegion", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);

    for (const auto& message : m_logMessages)
    {
        ImVec4 color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);

        if (message.find("[Error]") != std::string::npos)
            color = ImVec4(1.0f, 0.4f, 0.4f, 1.0f); // Red
        else if (message.find("[Warning]") != std::string::npos)
            color = ImVec4(1.0f, 1.0f, 0.4f, 1.0f); // Yellow
        else if (message.find("[Exception]") != std::string::npos)
            color = ImVec4(1.0f, 0.6f, 0.6f, 1.0f); // Light Red

        ImGui::PushStyleColor(ImGuiCol_Text, color);
        ImGui::TextUnformatted(message.c_str());
        ImGui::PopStyleColor();
    }

    ImGui::EndChild();

    if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY() - 20.0f)
    {
        ImGui::SetScrollHereY(1.0f);
    }

    ImGui::End();
}

void DebugWindow::ClearLog()
{
    m_logMessages.clear();
}

void DebugWindow::LogMessage(const String& message)
{
	m_logMessages.push_back(message);

	if (m_logMessages.size() > m_logLimit) 
	{
		m_logMessages.erase(m_logMessages.begin());
	}
}