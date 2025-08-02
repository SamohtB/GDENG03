#include "pch.h"
#include "MaterialEditor.h"
#include "GameObjectManager.h"
#include "GraphicsEngine.h"
#include "RenderSystem.h"
#include "RenderDevice.h"
#include "PipelineStateManager.h"
#include "TextureManager.h"
#include "MaterialManager.h"
#include "AGameObject.h"
#include "MaterialTypes.h"

MaterialEditor::MaterialEditor() : AUIScreen("Material Editor")
{
    this->m_visible = false;
    this->m_currentMaterial = MaterialType::BLANK_1;
}

void MaterialEditor::DrawUI()
{
    ImGui::Begin("Material Editor", &this->m_visible);

	// === Get Material To Edit ===
    auto materialOptions = GraphicsEngine::GetInstance()->GetMaterialManager()->GetAllMaterialNames();

    int currentIndex = 0;
    for (int i = 0; i < materialOptions.size(); ++i)
    {
        if (this->m_currentMaterial == materialOptions[i])
        {
            currentIndex = i;
            break;
        }
    }

    if (ImGui::Combo("Material", &currentIndex, materialOptions.data(), static_cast<int>(materialOptions.size())))
    {
        this->m_currentMaterial = materialOptions[currentIndex];
    }

    MaterialDescription matDesc = GraphicsEngine::GetInstance()->GetMaterialManager()->GetMaterialDescription(this->m_currentMaterial);
    bool propertyChanged = false;

    ImGui::Separator();

    // === Set Shader ===
    auto shaderOptions = GraphicsEngine::GetInstance()->GetRenderSystem()->GetRenderDevice()->GetPSOManager()->GetAllShaderNames();

    int shaderIndex = 0;
    for (int i = 0; i < shaderOptions.size(); ++i)
    {
        if (matDesc.shader == shaderOptions[i])
        {
            shaderIndex = i;
            break;
        }
    }

    if (ImGui::Combo("Shader", &shaderIndex, shaderOptions.data(), static_cast<int>(shaderOptions.size())))
    {
        matDesc.shader = shaderOptions[shaderIndex];
        propertyChanged = true;
    }

	// === Draw Material Properties ===
    // 
    // === Albedo & RGB ===
    ImGui::SeparatorText("Main Maps");
    if (DrawTextureField("Albedo", matDesc.albedoTex)) { propertyChanged = true; }
    ImGui::SameLine();
    if (ImGui::ColorEdit4("Color", &matDesc.albedoColor.x)) { propertyChanged = true; }

	// === Metallic, Roughness, Normal, Occlusion ===
    if (DrawTextureField("Metallic", matDesc.metalTex)) { propertyChanged = true; }
    if (matDesc.metalTex != TextureType::DEFAULT)
    {
        ImGui::SameLine();
        if (ImGui::SliderFloat("##MetalStrength", &matDesc.metalStrength, 0.0f, 1.0f)) { propertyChanged = true; }
    }

    if (DrawTextureField("Smoothness", matDesc.roughTex)) { propertyChanged = true; }
    if (matDesc.roughTex != TextureType::DEFAULT)
    {
        ImGui::SameLine();
        if (ImGui::SliderFloat("##SmoothStrength", &matDesc.roughStrength, 0.0f, 1.0f)) { propertyChanged = true; }
    }

    if (DrawTextureField("Normal", matDesc.normalTex)) { propertyChanged = true; }
    if (matDesc.normalTex != TextureType::DEFAULT)
    {
        ImGui::SameLine();
        if (ImGui::SliderFloat("##NormalStrength", &matDesc.normalStrength, 0.0f, 2.0f)) { propertyChanged = true; }
    }

    if (DrawTextureField("Occlusion", matDesc.aoTex)) { propertyChanged = true; }
    if (matDesc.aoTex != TextureType::DEFAULT)
    {
        ImGui::SameLine();
        if (ImGui::SliderFloat("##AOStrength", &matDesc.aoStrength, 0.0f, 1.0f)) { propertyChanged = true; }
    }

	ImGui::Spacing();

    ImGui::Text("Tiling");
    ImGui::SameLine();
    if (ImGui::DragFloat2("##Tiling", &matDesc.tiling.x, 0.01f, 0.01f, 100.0f)) { propertyChanged = true; }

    ImGui::Text("Offset");
    ImGui::SameLine();
    if (ImGui::DragFloat2("##Offset", &matDesc.offset.x, 0.01f, -10.0f, 10.0f)) { propertyChanged = true; }

    if (propertyChanged)
    {
        GraphicsEngine::GetInstance()->GetMaterialManager()->UpdateMaterialDescription(this->m_currentMaterial, matDesc);
    }

	ImGui::End();
}

void MaterialEditor::SetCurrentMaterial(const std::string& materialName)
{
    m_currentMaterial = materialName;
}

bool MaterialEditor::DrawTextureField(const char* label, std::string& textureName)
{
    bool changed = false;
    ImGui::Text("%s: %s", label, textureName.c_str());

    ImTextureID texID = GraphicsEngine::GetInstance()->GetTextureManager()->GetThumbnail(textureName);

    if (ImGui::ImageButton(label, (void*)texID, ImVec2(16, 16)))
    {
        ImGui::OpenPopup(label);
    }

    if (ImGui::BeginPopup(label))
    {
        auto textureList = GraphicsEngine::GetInstance()->GetTextureManager()->GetAllTextureNames();

        for (const char* texName : textureList)
        {
            if (ImGui::Selectable(texName))
            {
                textureName = texName;
                changed = true;
                ImGui::CloseCurrentPopup();
                break;
            }
        }

        ImGui::EndPopup();
    }
    return changed;
}