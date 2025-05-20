#pragma once
#include <d3d12.h>
#include <vector>

enum class InputLayoutType : int
{
    Pos_Color = 0,
    Pos_Tex_Color = 1,
};

constexpr int InputLayoutType_Count = 2;

class InputLayouts
{
public:
    static const std::vector<D3D12_INPUT_ELEMENT_DESC>& Get(InputLayoutType type)
    {
        static const std::vector<D3D12_INPUT_ELEMENT_DESC> layouts[] = {
            // Pos_Color
            {
                { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,   D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
                { "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
            },
            // Pos_Tex_Color
            {
                { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,   D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
                { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
                { "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
            }
        };

        return layouts[static_cast<int>(type)];
    }
};