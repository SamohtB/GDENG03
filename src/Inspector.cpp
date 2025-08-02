#include "pch.h"
#include "Inspector.h"
#include "GameObjectManager.h"
#include "AGameObject.h"
#include "MaterialTypes.h"
#include "GraphicsEngine.h"
#include "MaterialManager.h"
#include "TextureManager.h"
#include "RenderSystem.h"
#include "RenderDevice.h"
#include "PipelineStateManager.h"

Inspector::Inspector() : AUIScreen("Inspector") {}

void Inspector::DrawUI()
{
    ImGui::Begin("Inspector", &this->m_visible);

    AGameObject* object = GameObjectManager::GetInstance()->GetSelectedObject();
    if (object != nullptr)
    {
        ImGui::Text("Name: %s", object->GetName().c_str());
        ImGui::Spacing();
        
        auto components = object->GetAllComponents();

        for (const auto& component : components)
        {
            component->DrawUI();
        }
    }
    else
    {
        ImGui::Text("No object selected.");
    }

    ImGui::End();
}

void Inspector::DrawObjectInfo(AGameObject* object)
{
    
}

void Inspector::DrawTransformTab(AGameObject* object)
{
    
}

void Inspector::DrawMaterialTab(AGameObject* object)
{
 //   auto meshObject = dynamic_cast<AMeshObject*>(object);
 //   if (!meshObject) return;

	//auto materialOptions = GraphicsEngine::GetInstance()->GetMaterialManager()->GetAllMaterialNames();

 //   std::string currentMat = meshObject->GetMaterial();
 //   int currentIndex = 0;
 //   for (int i = 0; i < materialOptions.size(); ++i)
 //   {
 //       if (currentMat == materialOptions[i])
 //       {
 //           currentIndex = i;
 //           break;
 //       }
 //   }

 //   if (ImGui::Combo("Material", &currentIndex, materialOptions.data(), static_cast<int>(materialOptions.size())))
 //   {
 //       meshObject->SetMaterial(materialOptions[currentIndex]);
 //       return;
 //   }

 //   MaterialDescription matDesc = GraphicsEngine::GetInstance()->GetMaterialManager()->GetMaterialDescription(currentMat);
 //   bool propertyChanged = false;

 //   ImGui::Separator();

	//auto shaderOptions = GraphicsEngine::GetInstance()->GetRenderSystem()->GetRenderDevice()->GetPSOManager()->GetAllShaderNames();

 //   int shaderIndex = 0;
 //   for (int i = 0; i < shaderOptions.size(); ++i)
 //   {
 //       if (matDesc.shader == shaderOptions[i])
 //       {
 //           shaderIndex = i;
 //           break;
 //       }
 //   }

 //   if (ImGui::Combo("Shader", &shaderIndex, shaderOptions.data(), static_cast<int>(shaderOptions.size())))
 //   {
 //       matDesc.shader = shaderOptions[shaderIndex];
 //       propertyChanged = true;
 //   }

 //   ImGui::SeparatorText("Main Maps");
 //   if (DrawTextureField("Albedo", matDesc.albedoTex)) { propertyChanged = true; }
 //   ImGui::SameLine();
 //   if (ImGui::ColorEdit4("Color", &matDesc.albedoColor.x)) { propertyChanged = true; }

 //   if (DrawTextureField("Metallic", matDesc.metalTex)) { propertyChanged = true; }

 //   if (matDesc.metalTex != TextureType::DEFAULT)
 //   {
 //       ImGui::SameLine();
 //       if (ImGui::SliderFloat("##MetalStrength", &matDesc.metalStrength, 0.0f, 1.0f)) { propertyChanged = true; }
 //   }

 //   if (DrawTextureField("Smoothness", matDesc.roughTex)) { propertyChanged = true; }
 //   if (matDesc.roughTex != TextureType::DEFAULT)
 //   {
 //       ImGui::SameLine();
 //       if (ImGui::SliderFloat("##SmoothStrength", &matDesc.roughStrength, 0.0f, 1.0f)) { propertyChanged = true; }
 //   }

 //   if (DrawTextureField("Normal", matDesc.normalTex)) { propertyChanged = true; }
 //   if (matDesc.normalTex != TextureType::DEFAULT)
 //   {
 //       ImGui::SameLine();
 //       if (ImGui::SliderFloat("##NormalStrength", &matDesc.normalStrength, 0.0f, 2.0f)) { propertyChanged = true; }
 //   }

 //   if (DrawTextureField("Occlusion", matDesc.aoTex)) { propertyChanged = true; }
 //   if (matDesc.aoTex != TextureType::DEFAULT)
 //   {
 //       ImGui::SameLine();
 //       if (ImGui::SliderFloat("##AOStrength", &matDesc.aoStrength, 0.0f, 1.0f)) { propertyChanged = true; }
 //   }

 //   ImGui::Text("Tiling");
 //   ImGui::SameLine();
 //   if (ImGui::DragFloat2("##Tiling", &matDesc.tiling.x, 0.01f, 0.01f, 100.0f)) { propertyChanged = true; }

 //   ImGui::Text("Offset");
 //   ImGui::SameLine();
 //   if (ImGui::DragFloat2("##Offset", &matDesc.offset.x, 0.01f, -10.0f, 10.0f)) { propertyChanged = true; }

 //   if (propertyChanged)
 //   {
 //       GraphicsEngine::GetInstance()->GetMaterialManager()->UpdateMaterialDescription(currentMat, matDesc);
 //   }
}

bool Inspector::DrawTextureField(const char* label, std::string& textureName)
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