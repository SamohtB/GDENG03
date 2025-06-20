#include "Cube.h"
#include "DeviceContext.h"
#include "RenderSystem.h"
#include "RenderDevice.h"
#include "GraphicsEngine.h"
#include "GameObjectManager.h"
#include "Random.h"

Cube::Cube(String name, Vector3 color) : AMeshObject(name, color)
{
    /* Rainbow Shader Cube (1 & 3) */
    //std::vector<POS_COL> vertices = {
    //    {{-1.0f, -1.0f,  1.0f}, {1.0f, 0.0f, 0.0f}}, // 0
    //    {{ 1.0f, -1.0f,  1.0f}, {1.0f, 1.0f, 0.0f}}, // 1
    //    {{-1.0f,  1.0f,  1.0f}, {0.0f, 1.0f, 0.0f}}, // 2
    //    {{ 1.0f,  1.0f,  1.0f}, {0.0f, 0.0f, 1.0f}}, // 3

    //    {{-1.0f, -1.0f, -1.0f}, {1.0f, 0.0f, 0.0f}}, // 4
    //    {{ 1.0f, -1.0f, -1.0f}, {1.0f, 1.0f, 0.0f}}, // 5
    //    {{-1.0f,  1.0f, -1.0f}, {0.0f, 1.0f, 0.0f}}, // 6
    //    {{ 1.0f,  1.0f, -1.0f}, {0.0f, 0.0f, 1.0f}}, // 7
    //};

    /* Single Color (Default = white) (2 & 6) */
    //std::vector<POS_COL> vertices = {
    //    {{-0.5f, -0.5f,  0.5f}, color }, // 0
    //    {{ 0.5f, -0.5f,  0.5f}, color }, // 1
    //    {{-0.5f,  0.5f,  0.5f}, color }, // 2
    //    {{ 0.5f,  0.5f,  0.5f}, color }, // 3
    // 
    //    {{-0.5f, -0.5f, -0.5f}, color }, // 4
    //    {{ 0.5f, -0.5f, -0.5f}, color }, // 5
    //    {{-0.5f,  0.5f, -0.5f}, color }, // 6
    //    {{ 0.5f,  0.5f, -0.5f}, color }, // 7
    //};

    /* Card Blue and White Colors (7) */
    std::vector<POS_COL> vertices = {
        {{-0.5f, -0.5f,  0.5f}, {1.0f, 1.0f, 1.0f}}, // 0
        {{ 0.5f, -0.5f,  0.5f}, {1.0f, 1.0f, 1.0f}}, // 1
        {{-0.5f,  0.5f,  0.5f}, {1.0f, 1.0f, 1.0f}}, // 2
        {{ 0.5f,  0.5f,  0.5f}, {1.0f, 1.0f, 1.0f}}, // 3

        {{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}}, // 4
        {{ 0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}}, // 5
        {{-0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}}, // 6
        {{ 0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}}, // 7
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
    const float twoPi = static_cast<float>(M_PI) * 2.0f;
    const float fourPi = static_cast<float>(M_PI) * 4.0f;
    m_ticks += deltaTime * 2.0f;

    /* Rotate On All Axis Behaviour (2 & 4) */
    //auto rotateSpeed = deltaTime * 50.0f;
    //this->Rotate(rotateSpeed, rotateSpeed, rotateSpeed);
    /* End of 2 & 4 */

    /* Move Along X and Y axis + Lerp Scale (3) */
	//auto t = std::sin(m_ticks * 2.0f) * 2.0f;
    //auto tP = std::sin(m_ticks) * 0.5f + 0.5f;

    //if (m_ticks < twoPi)
    //{
    //   this->SetPosition(Vector3(t, 0.0f, 0.0f));
    //}
    //else if (m_ticks < fourPi)
    //{
    //   this->SetPosition(Vector3(0.0f, t, 0.0f));
    //}
    //else
    //{
    //   m_ticks = 0.0f;
    //}

    //float lerpT = std::clamp(m_ticks / fourPi, 0.0f, 1.0f);
    //float scale = std::lerp(1.0f, 0.25f, lerpT);
    //this->SetScale(scale, scale, scale);
    /* End of 3 */


    /* Warping Animation (5) behaviour */
    //float lerpT = std::clamp(m_ticks / fourPi, 0.0f, 1.0f);
	//float scaleXZ = std::lerp(1.0f, 5.00f, lerpT);
    //float scaleY = std::lerp(1.0f, 0.00f, lerpT);
	//float scaleRot = std::lerp(0.0f, 15.00f, lerpT);

	//this->SetScale(scaleXZ, scaleY, scaleXZ);
	//this->SetRotation(scaleRot, 0.0f, 0.0f);

    //if (m_ticks > fourPi)
    //{
    //  m_ticks = 0.0f;
    //}
    /* End of 5 */

}

void Cube::SetRandomRotation()
{
    float pitch = Random::Range(0.0f, DirectX::XM_2PI); 
    float yaw = Random::Range(0.0f, DirectX::XM_2PI);
    float roll = Random::Range(0.0f, DirectX::XM_2PI);

    this->SetRotation(pitch, yaw, roll);
}
