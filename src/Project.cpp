#include "pch.h"
#include "Project.h"

ProjectWindow::ProjectWindow() : AUIScreen("Project")
{
}

void ProjectWindow::DrawUI()
{
	//ImGui::ShowMetricsWindow();

	//ImGui::Begin("Project", &m_visible);

 //   if (ImGui::Button("+", ImVec2(28.0f, 0.0f)))
 //   {
 //   }

 //   ImGui::SameLine();

 //   {
 //       ImGui::Dummy(ImVec2(500.0f, 0.0f)); // Spacer

 //       ImGui::SameLine();

 //       const float searchWidth = 500.0f;

 //       ImGui::SetNextItemWidth(searchWidth);
 //       static char searchBuffer[128] = "";
 //       ImGui::InputText("##Search", searchBuffer, IM_ARRAYSIZE(searchBuffer));

 //       ImGui::SameLine();

 //       for (int i = 0; i < 5; i++)
 //       {
	//		if (ImGui::Button(("##Toggle" + std::to_string(i)).c_str(), ImVec2(24.0f, 24.0f)))
 //           {
	//			// Toggle stuff
 //           }

 //           if(i != 4)
	//		    ImGui::SameLine();
 //       }

	//	ImGui::Separator();

 //       ImGui::BeginTable("ViewSpaces", 2);

 //       ImGui::TableNextRow();
 //       float size = ImGui::GetWindowWidth() * 0.2f;
 //       ImGui::TableSetColumnIndex(0);
 //       ImGui::BeginChild("Project View", ImVec2(size, 200.0f), ImGuiChildFlags_AutoResizeX);
 //       ImGui::EndChild();

	//	ImGui::TableNextColumn();
 //       size = ImGui::GetWindowWidth() * 0.8f;
 //       ImGui::TableSetColumnIndex(1);
 //       ImGui::BeginChild("Asset View", ImVec2(size, 200.f), ImGuiChildFlags_AutoResizeX);
 //       ImGui::EndChild();

 //       ImGui::EndTable();
 //   }

	//ImGui::End();
}