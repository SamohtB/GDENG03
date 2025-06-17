#include "Cube.h"
#include "DeviceContext.h"
#include "RenderSystem.h"
#include "RenderDevice.h"
#include "GraphicsEngine.h"
#include "GameObjectManager.h"
#include "Random.h"

Cube::Cube(String name, Vector3 color) : AMeshObject(name, color)
{
    std::vector<POS_COL> vertices = {

        {{-1.0f, -1.0f,  1.0f}, color}, // 0
        {{ 1.0f, -1.0f,  1.0f}, color}, // 1
        {{-1.0f,  1.0f,  1.0f}, color}, // 2
        {{ 1.0f,  1.0f,  1.0f}, color}, // 3

        {{-1.0f, -1.0f, -1.0f}, color}, // 4
        {{ 1.0f, -1.0f, -1.0f}, color}, // 5
        {{-1.0f,  1.0f, -1.0f}, color}, // 6
        {{ 1.0f,  1.0f, -1.0f}, color}, // 7
    };

    std::vector<unsigned int> indices =
    {
        2, 6, 7,
        2, 3, 7,

        0, 4, 5,
        0, 1, 5,

        0, 2, 6,
        0, 4, 6,

        1, 3, 7,
        1, 5, 7,

        0, 2, 3,
        0, 1, 3,

        4, 6, 7,
        4, 5, 7
    };

	this->SetGeometry(vertices, indices);
    this->SetTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
}

void Cube::OnUpdate(float deltaTime)
{
    m_ticks += deltaTime;

    float t = 1.5f * sin(1.5f * m_ticks);
}

void Cube::SetRandomRotation()
{
    float pitch = Random::Range(0.0f, DirectX::XM_2PI); 
    float yaw = Random::Range(0.0f, DirectX::XM_2PI);
    float roll = Random::Range(0.0f, DirectX::XM_2PI);

    this->SetRotation(pitch, yaw, roll);
}
