#include "Sphere.h"

Sphere::Sphere(String name, Vector3 color) : AMeshObject(name, color)
{
    std::vector<POS_COL> vertices;
    std::vector<unsigned int> indices;

    const uint32_t verticalSegments = TESSELLATION;
    const uint32_t horizontalSegments = TESSELLATION * 2;

    // Create rings of vertices at progressively higher latitudes.
    for (uint32_t i = 0; i <= verticalSegments; i++)
    {
        const float v = 1 - float(i) / float(verticalSegments);

        const float latitude = (float(i) * M_PI / float(verticalSegments)) - M_PI / 2.0f;
        float dy, dxz;

        DirectX::XMScalarSinCos(&dy, &dxz, latitude);

        // Create a single ring of vertices at this latitude.
        for (uint32_t j = 0; j <= horizontalSegments; j++)
        {
            const float u = float(j) / float(horizontalSegments);

            const float longitude = float(j) * (M_PI * 2.0f) / float(horizontalSegments);
            float dx, dz;

            DirectX::XMScalarSinCos(&dx, &dz, longitude);

            dx *= dxz;
            dz *= dxz;

            const Vector3 normal = Vector3(dx, dy, dz);
            //const Vector2 textureCoordinate = Vector2(u, v);

            Vector3 position = XMVectorScale(normal, RADIUS);
            POS_COL vertexData = POS_COL(position, color);
            vertices.push_back(vertexData);
        }
    }

    // Fill the index buffer with triangles joining each pair of latitude rings.
    const uint32_t stride = horizontalSegments + 1;

    for (uint32_t i = 0; i < verticalSegments; i++)
    {
        for (uint32_t j = 0; j <= horizontalSegments; j++)
        {
            const uint32_t nextI = i + 1;
            const uint32_t nextJ = (j + 1) % stride;

            indices.push_back(i * stride + j);
            indices.push_back(nextI * stride + j);
            indices.push_back(i * stride + nextJ);

            indices.push_back(i * stride + nextJ);
            indices.push_back(nextI * stride + j);
            indices.push_back(nextI * stride + nextJ);
        }
    }

    SetGeometry(vertices, indices);
	this->SetTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
}

void Sphere::OnUpdate(float deltaTime)
{
	
}
