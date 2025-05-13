#pragma once
#include <Windows.h>
#include <string>

class ABaseWindow
{
public:
    ABaseWindow(UINT width, UINT height);
    virtual ~ABaseWindow() = default;

    virtual void OnCreate(HWND hwnd) = 0;
    virtual void OnUpdate() = 0;
    virtual void OnRender() = 0;
    virtual void OnDestroy() = 0;
    /* add on input later */

    unsigned int GetWidth() const;
    unsigned int GetHeight() const;

protected:
    /* viewport dimensions */
    unsigned int m_width = 0;
    unsigned int m_height = 0;
    float m_aspectRatio = 0;
};

