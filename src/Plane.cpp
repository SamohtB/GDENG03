#include "Plane.h"

Plane::Plane(String name, Vector3 color) : AMeshObject(name, color)
{
    std::vector<POS_COL> vertices = {
        {{-5.0f,  0.0f, -5.0f}, color},
        {{-5.0f,  0.0f,  5.0f}, color},
        {{ 5.0f,  0.0f,  5.0f}, color},
        {{ 5.0f,  0.0f, -5.0f}, color}
    };

    std::vector<unsigned int> indices =
    {
        0, 1, 2,
        0, 2, 3,

        2, 1, 0,
        3, 2, 0
    };

	this->SetGeometry(vertices, indices);
	this->SetTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
}

void Plane::OnUpdate(float deltaTime)
{
}
