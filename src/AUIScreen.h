#pragma once
#include "pch.h"

class AUIScreen
{
public:
    using String = std::string;

    AUIScreen(String name);
    ~AUIScreen() = default;

    String GetName();
    virtual void DrawUI() = 0;

private:
    String name;
};

