#include "pch.h"
#include "DebugWindow.h"

DebugWindow::DebugWindow() : AUIScreen("Debug")
{
	this->m_logLimit = 100; // Default log limit
}

void DebugWindow::DrawUI()
{
    ImGui::Begin("Debug Log");

    if (ImGui::Button("Clear"))
    {
        ClearLog();
    }

    ImGui::Separator();

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