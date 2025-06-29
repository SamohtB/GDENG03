#pragma once
#include "pch.h"
#include "InputListener.h"

class ImGuiInputListener : public InputListener
{
public:
    virtual void OnKeyPressed(int key) override
    {
        ImGuiIO& io = ImGui::GetIO();
        io.AddKeyEvent(KeyCodeToImGuiKey(key), true);
        UpdateModifiers(io);
    }

    virtual void OnKeyReleased(int key) override
    {
        ImGuiIO& io = ImGui::GetIO();
        io.AddKeyEvent(KeyCodeToImGuiKey(key), false);
        UpdateModifiers(io);
    }

    virtual void OnKeyHeld(int key) override
    {
        // Optional: ImGui doesn't use "held" state directly, but you could process repeat logic here.
    }

    virtual void OnMouseMove(const Vector2& mousePos) override
    {
        ImGuiIO& io = ImGui::GetIO();
        io.MousePos = ImVec2(mousePos.x, mousePos.y);
    }

    virtual void OnMouseWheel(const float& delta) override
    {
        ImGuiIO& io = ImGui::GetIO();
        io.MouseWheel += delta;
    }

    virtual void OnLeftMousePressed(const Vector2& mousePos) override
    {
        ImGuiIO& io = ImGui::GetIO();
        io.MouseDown[0] = true;
        io.MousePos = ImVec2(mousePos.x, mousePos.y);
    }

    virtual void OnLeftMouseReleased(const Vector2& mousePos) override
    {
        ImGuiIO& io = ImGui::GetIO();
        io.MouseDown[0] = false;
        io.MousePos = ImVec2(mousePos.x, mousePos.y);
    }

    virtual void OnRightMousePressed(const Vector2& mousePos) override
    {
        ImGuiIO& io = ImGui::GetIO();
        io.MouseDown[1] = true;
        io.MousePos = ImVec2(mousePos.x, mousePos.y);
    }

    virtual void OnRightMouseReleased(const Vector2& mousePos) override
    {
        ImGuiIO& io = ImGui::GetIO();
        io.MouseDown[1] = false;
        io.MousePos = ImVec2(mousePos.x, mousePos.y);
    }

private:
    void UpdateModifiers(ImGuiIO& io)
    {
        // Update commonly used modifiers if you define them
        io.AddKeyEvent(ImGuiMod_Ctrl, (GetKeyState(VK_CONTROL) & 0x8000) != 0);
        io.AddKeyEvent(ImGuiMod_Shift, (GetKeyState(VK_SHIFT) & 0x8000) != 0);
        io.AddKeyEvent(ImGuiMod_Alt, (GetKeyState(VK_MENU) & 0x8000) != 0);
        io.AddKeyEvent(ImGuiMod_Super, (GetKeyState(VK_LWIN) & 0x8000) != 0 ||
            (GetKeyState(VK_RWIN) & 0x8000) != 0);
    }

    ImGuiKey KeyCodeToImGuiKey(int vk)
    {
        // Alphanumeric keys
        if (vk >= 'A' && vk <= 'Z')
            return (ImGuiKey)(ImGuiKey_A + (vk - 'A'));
        if (vk >= '0' && vk <= '9')
            return (ImGuiKey)(ImGuiKey_0 + (vk - '0'));

        // Function keys
        if (vk >= VK_F1 && vk <= VK_F12)
            return (ImGuiKey)(ImGuiKey_F1 + (vk - VK_F1));

        switch (vk)
        {
        case VK_ESCAPE:        return ImGuiKey_Escape;
        case VK_RETURN:        return ImGuiKey_Enter;
        case VK_TAB:           return ImGuiKey_Tab;
        case VK_BACK:          return ImGuiKey_Backspace;
        case VK_SPACE:         return ImGuiKey_Space;
        case VK_DELETE:        return ImGuiKey_Delete;
        case VK_INSERT:        return ImGuiKey_Insert;
        case VK_HOME:          return ImGuiKey_Home;
        case VK_END:           return ImGuiKey_End;
        case VK_PRIOR:         return ImGuiKey_PageUp;
        case VK_NEXT:          return ImGuiKey_PageDown;
        case VK_LEFT:          return ImGuiKey_LeftArrow;
        case VK_RIGHT:         return ImGuiKey_RightArrow;
        case VK_UP:            return ImGuiKey_UpArrow;
        case VK_DOWN:          return ImGuiKey_DownArrow;
        case VK_SHIFT:         return ImGuiKey_LeftShift;   // Depends on scancode
        case VK_CONTROL:       return ImGuiKey_LeftCtrl;
        case VK_MENU:          return ImGuiKey_LeftAlt;
        case VK_LSHIFT:        return ImGuiKey_LeftShift;
        case VK_RSHIFT:        return ImGuiKey_RightShift;
        case VK_LCONTROL:      return ImGuiKey_LeftCtrl;
        case VK_RCONTROL:      return ImGuiKey_RightCtrl;
        case VK_LMENU:         return ImGuiKey_LeftAlt;
        case VK_RMENU:         return ImGuiKey_RightAlt;
        default:               return ImGuiKey_None;
        }
    }
};
