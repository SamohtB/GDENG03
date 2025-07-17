#include "pch.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"
#include "MeshManager.h"
#include "MeshTypes.h"
#include "Debug.h"

MeshManager::MeshManager(std::shared_ptr<BatchUploader> uploader)
{
}

void MeshManager::LoadInitialMeshes()
{
	LoadCubePrimitive();
	LoadPlanePrimitive();
	LoadSpherePrimitive();
	LoadCylinderPrimitive();
}

void MeshManager::LoadMesh(const String& meshName, const std::wstring& filePath)
{
}

MeshData* MeshManager::GetMeshData(const String& meshName)
{
	auto it = m_meshMap.find(meshName);

	if (it != m_meshMap.end())
	{
		return &it->second;
	}

	Debug::LogError("MeshManager: Mesh '" + meshName + "' not found.");
	return nullptr;
}

std::vector<const char*> MeshManager::GetAllMeshNames() const
{
	std::vector<const char*> names;
	names.reserve(m_meshMap.size());

	for (const auto& pair : m_meshMap)
	{
		names.push_back(pair.first.c_str());
	}

	return names;
}

void MeshManager::LoadCubePrimitive()
{
    float halfSize = m_cubeSize * 0.5f;

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    vertices.reserve(24);
    indices.reserve(36);

    const Vector3 faceNormals[6] = 
    {
        {  0,  0,  1 }, {  0,  0, -1 },
        {  0,  1,  0 }, {  0, -1,  0 },
        { -1,  0,  0 }, {  1,  0,  0 }
    };

    const Vector3 faceTangents[6] = {
        { 1,  0,  0 }, { -1, 0,  0 },
        { 1,  0,  0 }, { 1,  0,  0 },
        { 0,  0, -1 }, { 0,  0,  1 }
    };

    const Vector2 texCoords[4] = 
    {
        { 0, 1 }, { 1, 1 }, { 0, 0 }, { 1, 0 }
    };

    const Vector3 positions[24] =
    {
        {-halfSize, -halfSize,  halfSize}, { halfSize, -halfSize,  halfSize},
        {-halfSize,  halfSize,  halfSize}, { halfSize,  halfSize,  halfSize},

        { halfSize, -halfSize, -halfSize}, {-halfSize, -halfSize, -halfSize},
        { halfSize,  halfSize, -halfSize}, {-halfSize,  halfSize, -halfSize},

        {-halfSize,  halfSize,  halfSize}, { halfSize,  halfSize,  halfSize},
        {-halfSize,  halfSize, -halfSize}, { halfSize,  halfSize, -halfSize},

        {-halfSize, -halfSize, -halfSize}, { halfSize, -halfSize, -halfSize},
        {-halfSize, -halfSize,  halfSize}, { halfSize, -halfSize,  halfSize},

        {-halfSize, -halfSize, -halfSize}, {-halfSize, -halfSize,  halfSize},
        {-halfSize,  halfSize, -halfSize}, {-halfSize,  halfSize,  halfSize},

        { halfSize, -halfSize,  halfSize}, { halfSize, -halfSize, -halfSize},
        { halfSize,  halfSize,  halfSize}, { halfSize,  halfSize, -halfSize}
    };

    for (int face = 0; face < 6; ++face)
    {
        for (int i = 0; i < 4; ++i)
        {
            Vertex v;
            v.position = positions[face * 4 + i];
            v.normal = faceNormals[face];
            v.tangent = faceTangents[face];
            v.texcoord = texCoords[i];
            vertices.push_back(v);
        }

        int base = face * 4;
        indices.push_back(base + 0);
        indices.push_back(base + 2);
        indices.push_back(base + 1);

        indices.push_back(base + 2);
        indices.push_back(base + 3);
        indices.push_back(base + 1);
    }

    // Calculate tangents
    std::vector<Vector3> positionsOnly(vertices.size());
    std::vector<Vector3> normals(vertices.size());
    std::vector<Vector2> texcoords(vertices.size());
    std::vector<Vector3> tangents(vertices.size());

    for (size_t i = 0; i < vertices.size(); ++i)
    {
        positionsOnly[i] = vertices[i].position;
        normals[i] = vertices[i].normal;
        texcoords[i] = vertices[i].texcoord;
    }

    GeoMath::CalculateTangentFrame(indices, positionsOnly.data(), normals.data(), texcoords.data(), vertices.size(), tangents.data());

    for (size_t i = 0; i < vertices.size(); ++i)
        vertices[i].tangent = tangents[i];

    MeshData cubeData;
	cubeData.m_vertexBuffer = std::make_unique<VertexBuffer>(vertices);
	cubeData.m_indexBuffer = std::make_unique<IndexBuffer>(indices);
    cubeData.m_indicesSize = static_cast<UINT>(indices.size());

    m_meshMap[MeshType::PRIMITIVE_CUBE] = std::move(cubeData);
	Debug::Log("MeshManager: Loaded cube primitive.");
}

void MeshManager::LoadPlanePrimitive()
{
    float halfSize = m_planeSize * 0.5f;

    std::vector<Vertex> vertices(4);
    std::vector<unsigned int> indices = { 0, 2, 1, 2, 3, 1 };

    const Vector3 positions[4] = {
           { -halfSize, 0.0f, -halfSize },  // 0
           { -halfSize, 0.0f,  halfSize },  // 1
           {  halfSize, 0.0f, -halfSize },  // 2
           {  halfSize, 0.0f,  halfSize }   // 3
    };

    const Vector2 texcoords[4] = {
        { 0.0f, 1.0f },
        { 0.0f, 0.0f },
        { 1.0f, 1.0f },
        { 1.0f, 0.0f }
    };

    const Vector3 normal = { 0.0f, 1.0f, 0.0f };

    for (int i = 0; i < 4; ++i) {
        vertices[i].position = positions[i];
        vertices[i].texcoord = texcoords[i];
        vertices[i].normal = normal;
        vertices[i].tangent = Vector3(0, 0, 0);
    }

    std::vector<Vector3> tangents(4);
    GeoMath::CalculateTangentFrame(
        indices,
        reinterpret_cast<const Vector3*>(&vertices[0].position),
        reinterpret_cast<const Vector3*>(&vertices[0].normal),
        reinterpret_cast<const Vector2*>(&vertices[0].texcoord),
        4,
        tangents.data()
    );

    for (int i = 0; i < 4; ++i)
        vertices[i].tangent = tangents[i];

    MeshData planeData;
    planeData.m_vertexBuffer = std::make_unique<VertexBuffer>(vertices);
    planeData.m_indexBuffer = std::make_unique<IndexBuffer>(indices);
    planeData.m_indicesSize = static_cast<UINT>(indices.size());

    m_meshMap[MeshType::PRIMITIVE_PLANE] = std::move(planeData);
    Debug::Log("MeshManager: Loaded plane primitive.");
}

void MeshManager::LoadSpherePrimitive()
{
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;

    const uint32_t verticalSegments = m_sphereTessellation;
    const uint32_t horizontalSegments = m_sphereTessellation * 2;

    vertices.reserve((verticalSegments + 1) * horizontalSegments);
    indices.reserve(verticalSegments * horizontalSegments * 6);

    for (uint32_t i = 0; i <= verticalSegments; ++i)
    {
        float v = 1.0f - float(i) / float(verticalSegments);
        float latitude = float(i) * DirectX::XM_PI / float(verticalSegments) - DirectX::XM_PIDIV2;

        float dy, dxz;
        DirectX::XMScalarSinCos(&dy, &dxz, latitude);

        for (uint32_t j = 0; j < horizontalSegments; ++j)
        {
            float u = float(j) / float(horizontalSegments);
            float longitude = float(j) * DirectX::XM_2PI / float(horizontalSegments);

            float dx, dz;
            DirectX::XMScalarSinCos(&dx, &dz, longitude);

            dx *= dxz;
            dz *= dxz;

            Vector3 normal(dx, dy, dz);
            Vector2 texcoord(u, v);
            Vector3 position = normal * m_sphereRadius;

            vertices.emplace_back(Vertex{ position, texcoord, normal });
        }
    }

    const uint32_t stride = horizontalSegments;

    for (uint32_t i = 0; i < verticalSegments; ++i)
    {
        for (uint32_t j = 0; j < horizontalSegments; ++j)
        {
            uint32_t nextI = i + 1;
            uint32_t nextJ = (j + 1) % horizontalSegments;

            uint32_t index0 = i * stride + j;
            uint32_t index1 = i * stride + nextJ;
            uint32_t index2 = nextI * stride + j;
            uint32_t index3 = nextI * stride + nextJ;

            indices.push_back(index0);
            indices.push_back(index1);
            indices.push_back(index2);

            indices.push_back(index1);
            indices.push_back(index3);
            indices.push_back(index2);
        }
    }

    std::vector<Vector3> positions(vertices.size());
    std::vector<Vector3> normals(vertices.size());
    std::vector<Vector2> texcoords(vertices.size());
    std::vector<Vector3> tangents(vertices.size());

    for (size_t i = 0; i < vertices.size(); ++i)
    {
        positions[i] = vertices[i].position;
        normals[i] = vertices[i].normal;
        texcoords[i] = vertices[i].texcoord;
    }

    GeoMath::CalculateTangentFrame(indices, positions.data(), normals.data(), texcoords.data(), vertices.size(), tangents.data());

    for (size_t i = 0; i < vertices.size(); ++i)
        vertices[i].tangent = tangents[i];

	MeshData sphereData;
	sphereData.m_vertexBuffer = std::make_unique<VertexBuffer>(vertices);
    sphereData.m_indexBuffer = std::make_unique<IndexBuffer>(indices);
    sphereData.m_indicesSize = static_cast<UINT>(indices.size());

    m_meshMap[MeshType::PRIMITIVE_SPHERE] = std::move(sphereData);
	Debug::Log("MeshManager: Loaded sphere primitive.");
}

void MeshManager::LoadCylinderPrimitive()
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    const float halfHeight = m_cylinderHeight * 0.5f;
    const float angleStep = 2.0f * M_PI / m_cylinderSegments;

    vertices.reserve(m_cylinderSegments * 4 + 2);
    indices.reserve(m_cylinderSegments * 12);

    for (int i = 0; i <= m_cylinderSegments; ++i)
    {
        const float theta = i * angleStep;
        const float cosTheta = cosf(theta);
        const float sinTheta = sinf(theta);
        const float x = m_cylinderRadius * cosTheta;
        const float z = m_cylinderRadius * sinTheta;
        const float u = static_cast<float>(i) / m_cylinderSegments;

        Vector3 normal(x, 0.0f, z);
        normal.Normalize();

        vertices.emplace_back(Vertex{ { x, +halfHeight, z }, { u, 0.0f }, normal, { 0, 0, 0 } });
        vertices.emplace_back(Vertex{ { x, -halfHeight, z }, { u, 1.0f }, normal, { 0, 0, 0 } });

        if (i < m_cylinderSegments)
        {
            const int base = i * 2;
            indices.push_back(base);
            indices.push_back(base + 1);
            indices.push_back(base + 2);

            indices.push_back(base + 2);
            indices.push_back(base + 1);
            indices.push_back(base + 3);
        }
    }

    const int topCenterIndex = static_cast<int>(vertices.size());
    vertices.emplace_back(Vertex{ { 0, +halfHeight, 0 }, { 0.5f, 0.5f }, { 0, 1, 0 }, { 0, 0, 0 } });

    for (int i = 0; i <= m_cylinderSegments; ++i)
    {
        const float theta = i * angleStep;
        const float cosTheta = cosf(theta);
        const float sinTheta = sinf(theta);
        const float x = m_cylinderRadius * cosTheta;
        const float z = m_cylinderRadius * sinTheta;

        const float u = 0.5f + 0.5f * cosTheta;
        const float v = 0.5f - 0.5f * sinTheta;

        vertices.emplace_back(Vertex{ { x, +halfHeight, z }, { u, v }, { 0, 1, 0 }, { 0, 0, 0 } });

        if (i < m_cylinderSegments)
        {
            indices.push_back(topCenterIndex);
            indices.push_back(topCenterIndex + i + 1);
            indices.push_back(topCenterIndex + i + 2);
        }
    }

    const int bottomCenterIndex = static_cast<int>(vertices.size());
    vertices.emplace_back(Vertex{ { 0, -halfHeight, 0 }, { 0.5f, 0.5f }, { 0, -1, 0 }, { 0, 0, 0 } });

    for (int i = 0; i <= m_cylinderSegments; ++i)
    {
        const float theta = i * angleStep;
        const float cosTheta = cosf(theta);
        const float sinTheta = sinf(theta);
        const float x = m_cylinderRadius * cosTheta;
        const float z = m_cylinderRadius * sinTheta;

        const float u = 0.5f + 0.5f * cosTheta;
        const float v = 0.5f + 0.5f * sinTheta;

        vertices.emplace_back(Vertex{ { x, -halfHeight, z }, { u, v }, { 0, -1, 0 }, { 0, 0, 0 } });

        if (i < m_cylinderSegments)
        {
            indices.push_back(bottomCenterIndex);
            indices.push_back(bottomCenterIndex + i + 2);
            indices.push_back(bottomCenterIndex + i + 1);
        }
    }

    std::vector<Vector3> positions(vertices.size());
    std::vector<Vector3> normals(vertices.size());
    std::vector<Vector2> texcoords(vertices.size());
    std::vector<Vector3> tangents(vertices.size());

    for (size_t i = 0; i < vertices.size(); ++i)
    {
        positions[i] = vertices[i].position;
        normals[i] = vertices[i].normal;
        texcoords[i] = vertices[i].texcoord;
    }

    GeoMath::CalculateTangentFrame(
        indices,
        positions.data(),
        normals.data(),
        texcoords.data(),
        vertices.size(),
        tangents.data()
    );

    for (size_t i = 0; i < vertices.size(); ++i)
        vertices[i].tangent = tangents[i];

	MeshData cylinderData;
	cylinderData.m_vertexBuffer = std::make_unique<VertexBuffer>(vertices);
    cylinderData.m_indexBuffer = std::make_unique<IndexBuffer>(indices);
    cylinderData.m_indicesSize = static_cast<UINT>(indices.size());

	m_meshMap[MeshType::PRIMITIVE_CYLINDER] = std::move(cylinderData);
	Debug::Log("MeshManager: Loaded cylinder primitive.");
}
