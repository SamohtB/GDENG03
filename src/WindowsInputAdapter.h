#pragma once

enum class KeyCode;

class WindowsInputAdapter
{
public:
    WindowsInputAdapter(HWND hwnd);
    ~WindowsInputAdapter() = default;

    void processMessage(UINT msg, WPARAM wParam, LPARAM lParam);

private:
    void handleRawInput(HRAWINPUT hRawInput);
    KeyCode mapVirtualKeyToKeyCode(WPARAM vk);
    HWND m_hwnd;
};
