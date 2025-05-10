#pragma once
#include <Windows.h>
#include <string>

class Window
{
public:
    Window(UINT width, UINT height, std::wstring name);
    virtual ~Window();

    virtual void OnCreate() = 0;
    virtual void OnUpdate() = 0;
    virtual void OnRender() = 0;
    virtual void OnDestroy() = 0;

    unsigned int GetWidth() const;
    unsigned int GetHeight() const;

    bool IsRun();

    static HWND GetHWND();

protected:
    static LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

    static HWND m_hwnd;
    bool m_isRun = false;

    /* viewport dimensions */
    unsigned int m_width = 0;
    unsigned int m_height = 0;
    float m_aspectRatio = 0;

private:
    bool Broadcast();

    bool isInit = false;
};

