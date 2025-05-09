#pragma once
#include <Windows.h>
#include <string>

class ABaseRenderer
{
public:
    ABaseRenderer(UINT width, UINT height, std::wstring name);
    virtual ~ABaseRenderer();

    virtual void OnInit() = 0;
    virtual void OnUpdate() = 0;
    virtual void OnRender() = 0;
    virtual void OnDestroy() = 0;

    unsigned int GetWidth() const;
    unsigned int GetHeight() const;

protected:
    void SetCustomWindowText(LPCWSTR text);

    /* viewport dimensions */
    unsigned int m_width = 0;
    unsigned int m_height = 0;
    float m_aspectRatio = 0;

private:
    std::wstring m_title{};
};

