#pragma once
#include "AUIScreen.h"

class AGameObject;

class Inspector : public AUIScreen
{
public:
    Inspector();
    ~Inspector() = default;

    // Inherited via AUIScreen
    void DrawUI() override;

    void DrawTransformTab(AGameObject* object);
};

