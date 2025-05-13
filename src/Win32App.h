#pragma once
#include <memory>

#include "ABaseWindow.h"

class ABaseWindow;

class Win32App
{
public:
    Win32App(ABaseWindow* window, std::wstring windowName);
    HWND GetHwnd();

    bool IsRun();
    void Broadcast();

protected:
    static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

private:
    ABaseWindow* m_window = nullptr;

    HWND m_hwnd;
    bool m_isRun = true;
};
