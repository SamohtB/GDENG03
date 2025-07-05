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

private:
	void DrawObjectInfo(AGameObject* object);
    void DrawTransformTab(AGameObject* object);
    void DrawMaterialTab(AGameObject* object);

};

