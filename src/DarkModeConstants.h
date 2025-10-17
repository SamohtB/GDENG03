#pragma once
#include "imgui.h"

namespace DarkModeGUIConstants
{
    // Text
    constexpr ImVec4 TEXT = ImVec4(0.7686f, 0.7686f, 0.7686f, 1.0f);

    // Backgrounds
    constexpr ImVec4 WINDOW_BG = ImVec4(0.235f, 0.235f, 0.235f, 1.0f);
    constexpr ImVec4 CHILD_BG = ImVec4(0.196f, 0.196f, 0.196f, 1.0f);
    constexpr ImVec4 POPUP_BG = ImVec4(0.235f, 0.235f, 0.235f, 1.0f);

    // Borders
    constexpr ImVec4 BORDER = ImVec4(0.f, 0.f, 0.f, 0.f);
	constexpr ImVec4 BORDER_SHADOW = ImVec4(0.275f, 0.376f, 0.486f, 1.0f);

    // Scrollbar
    constexpr ImVec4 SCROLLBAR_BG = ImVec4(0.165f, 0.165f, 0.165f, 1.0f);
    constexpr ImVec4 SCROLLBAR_GRAB = ImVec4(0.345f, 0.345f, 0.345f, 1.0f);
    constexpr ImVec4 SCROLLBAR_GRAB_HOVERED = ImVec4(0.404f, 0.404f, 0.404f, 1.0f);
    constexpr ImVec4 SCROLLBAR_GRAB_ACTIVE = ImVec4(0.275f, 0.376f, 0.486f, 0.698f);

    // Checkmark
    constexpr ImVec4 CHECKMARK = ImVec4(0.8431f, 0.8431f, 0.8431f, 1.0f);

    // Slider
    constexpr ImVec4 SLIDER_GRAB = ImVec4(0.6000f, 0.6000f, 0.6000f, 1.0f);
	constexpr ImVec4 SLIDER_GRAB_HOVERED = ImVec4(0.9176f, 0.9176f, 0.9176f, 1.0f);
    constexpr ImVec4 SLIDER_GRAB_ACTIVE = ImVec4(0.9176f, 0.9176f, 0.9176f, 1.0f);
 
    // Frames
    constexpr ImVec4 FRAME_BG = ImVec4(0.165f, 0.165f, 0.165f, 1.0f);
    constexpr ImVec4 FRAME_BG_HOVERED = ImVec4(0.404f, 0.404f, 0.404f, 1.0f);
    constexpr ImVec4 FRAME_BG_ACTIVE = ImVec4(0.275f, 0.376f, 0.486f, 0.698f);

    // Titles
    constexpr ImVec4 TITLE_BG = ImVec4(0.063f, 0.063f, 0.063f, 1.0f);
    constexpr ImVec4 TITLE_BG_ACTIVE = ImVec4(0.063f, 0.063f, 0.063f, 1.0f);
    constexpr ImVec4 TITLE_BG_COLLAPSED = ImVec4(0.063f, 0.063f, 0.063f, 1.0f);

    // Menus
    constexpr ImVec4 MENU_BAR_BG = ImVec4(1.f, 1.f, 1.f, 1.0f);

    // Buttons
    constexpr ImVec4 BUTTON = ImVec4(0.345f, 0.345f, 0.345f, 1.0f);
    constexpr ImVec4 BUTTON_HOVERED = ImVec4(0.504f, 0.504f, 0.504f, 0.698f);
    constexpr ImVec4 BUTTON_ACTIVE = ImVec4(0.275f, 0.376f, 0.486f, 0.698f);

    // Headers
    constexpr ImVec4 HEADER = ImVec4(0.196f, 0.196f, 0.196f, 1.0f);
    constexpr ImVec4 HEADER_HOVERED = ImVec4(0.118f, 0.118f, 0.118f, 1.0f);
    constexpr ImVec4 HEADER_ACTIVE = ImVec4(0.275f, 0.376f, 0.486f, 0.698f);

    // Tabs
    constexpr ImVec4 TAB = ImVec4(0.063f, 0.063f, 0.063f, 1.0f);
    constexpr ImVec4 TAB_UNFOCUSED = ImVec4(0.063f, 0.063f, 0.063f, 1.0f);
    constexpr ImVec4 TAB_UNFOCUSED_ACTIVE = ImVec4(0.235f, 0.235f, 0.235f, 1.0f);
    constexpr ImVec4 TAB_ACTIVE = ImVec4(0.235f, 0.235f, 0.235f, 1.0f);
    constexpr ImVec4 TAB_HOVERED = ImVec4(0.118f, 0.118f, 0.118f, 1.0f);

    // Selection
    constexpr ImVec4 TEXT_SELECTED_BG = ImVec4(0.7686f, 0.7686f, 0.7686f, 1.0f);
}
