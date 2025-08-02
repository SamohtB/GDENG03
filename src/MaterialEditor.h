#pragma once
#include "AUIScreen.h"

class AGameObject;

class MaterialEditor : public AUIScreen
{
public:
    MaterialEditor();
    ~MaterialEditor() = default;

    void DrawUI() override;
    void SetCurrentMaterial(const std::string& materialName);

private:
    bool DrawTextureField(const char* label, std::string& textureName);

	String m_currentMaterial;
};

