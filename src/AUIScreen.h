#pragma once
#include "pch.h"

class AUIScreen
{
public:
    using String = std::string;

    AUIScreen(String name);
    ~AUIScreen() = default;

    virtual void DrawUI() = 0;

    String GetName();
    bool IsVisible() const;
    void SetVisible(bool visible);

protected:
    ImVec4 m_bodyColor = ImVec4(0.65f, 0.91f, 0.64f, 1.0f);    // Soft Nature Green
    ImVec4 m_headerColor = ImVec4(0.32f, 0.45f, 0.32f, 1.0f);  // Darker green hue
    ImVec4 m_textColor = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);   // Med Gray

private:
    String m_name;
    bool m_visible;
};

