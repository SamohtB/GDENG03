#pragma once
#include "AUIScreen.h"

class AGameObject;

class Inspector : public AUIScreen
{
public:
    Inspector();
    ~Inspector() = default;

    void DrawUI() override;

};