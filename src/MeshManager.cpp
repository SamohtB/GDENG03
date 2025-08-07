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
	LoadCapsulePrimitive();
    //LoadMesh(MeshType::UTAH_TEAPOT, L"Assets/Meshes/teapot.obj");
    //LoadMesh(MeshType::STANFORD_BUNNY, L"Assets/Meshes/bunny.obj");
    //LoadMesh(MeshType::STANFORD_ARMADILLO, L"Assets/Meshes/armadillo.obj");
    //LoadMesh(MeshType::LUCY, L"Assets/Meshes/lucy.obj");
}

void MeshManager::LoadMesh(const String& meshName, const std::wstring& filePath)
{
    std::string inputFile(filePath.begin(), filePath.end());

    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    bool res = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, inputFile.c_str());

    if (!warn.empty())
        Debug::LogWarning(warn.c_str());

    if (!err.empty())
        Debug::LogError(err.c_str());

    if (!res)
    {
        Debug::LogError("Failed to load mesh: " + meshName);
        return;
    }

    if (shapes.size() > 1)
    {
		Debug::LogWarning("Mesh '" + meshName + "' has multiple shapes. Only the first shape will be used.");
    }

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    for (size_t s = 0; s < shapes.size(); s++)
    {
		size_t indexOffset = 0;

        for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++)
        {
			unsigned char numFaceVertices = shapes[s].mesh.num_face_vertices[f];

            std::vector<unsigned int> faceIndices;

            for (unsigned char v = 0; v < numFaceVertices; v++)
            {
                tinyobj::index_t index = shapes[s].mesh.indices[indexOffset + v];

                Vertex vertex = {};

                if (index.vertex_index >= 0)
                {
                    vertex.position.x = attrib.vertices[index.vertex_index * 3 + 0];
                    vertex.position.y = attrib.vertices[index.vertex_index * 3 + 1];
                    vertex.position.z = attrib.vertices[index.vertex_index * 3 + 2];
                }

                if (index.normal_index >= 0)
                {
                    vertex.normal.x = attrib.normals[index.normal_index * 3 + 0];
                    vertex.normal.y = attrib.normals[index.normal_index * 3 + 1];
                    vertex.normal.z = attrib.normals[index.normal_index * 3 + 2];
                }

                if (index.texcoord_index >= 0)
                {
                    vertex.texcoord.x = attrib.texcoords[index.texcoord_index * 2 + 0];
                    vertex.texcoord.y = 1.0f - attrib.texcoords[index.texcoord_index * 2 + 1];
                }

                vertices.push_back(vertex);
                faceIndices.push_back(static_cast<unsigned int>(vertices.size() - 1));
            }

            if (faceIndices.size() == 3)
            {
                indices.push_back(faceIndices[2]);
                indices.push_back(faceIndices[1]);
                indices.push_back(faceIndices[0]);
            }
            else
            {
                for (int i = 0; i < faceIndices.size(); ++i)
                    indices.push_back(faceIndices[i]);
            }

            indexOffset += numFaceVertices;
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

    MeshData meshData;
    meshData.m_vertexBuffer = std::make_unique<VertexBuffer>(vertices);
    meshData.m_indexBuffer = std::make_unique<IndexBuffer>(indices);
    meshData.m_indicesSize = static_cast<UINT>(indices.size());

    m_meshMap[meshName] = std::move(meshData);
    Debug::Log("MeshManager: Loaded " + meshName);
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
            indices.push_back(index2);
            indices.push_back(index1);

            indices.push_back(index1);
            indices.push_back(index2);
            indices.push_back(index3);
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

void MeshManager::LoadCapsulePrimitive()
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    const int ringsPerHemisphere = m_capsuleRings;
    const int ringsOnBody = m_capsuleRings;
    const int totalRings = ringsPerHemisphere * 2 + ringsOnBody;
    const int segments = m_capsuleSegments;
    const float radius = m_capsuleRadius;
    const float halfBodyHeight = (m_capsuleHeight * 0.5f) - radius;

    const int vertexPerRing = segments + 1;

    const float verticalStep = 1.0f / totalRings;
    const float horizontalStep = 1.0f / segments;

    // Precompute trig tables
    std::vector<float> sinTable(segments + 1);
    std::vector<float> cosTable(segments + 1);
    for (int i = 0; i <= segments; ++i)
    {
        float angle = (2.0f * M_PI * i) / segments;
        sinTable[i] = sinf(angle);
        cosTable[i] = cosf(angle);
    }

    // Generate vertices
    for (int ring = 0; ring <= totalRings; ++ring)
    {
        float t = static_cast<float>(ring) / totalRings;
        float y = 0.0f;
        float r = 0.0f;
        float yOffset = 0.0f;

        if (ring < ringsPerHemisphere) // top hemisphere
        {
            float v = static_cast<float>(ring) / ringsPerHemisphere;
            float theta = (v * 0.5f) * M_PI; 
            y = halfBodyHeight + radius * cosf(theta);
            r = radius * sinf(theta);
            yOffset = halfBodyHeight;
        }
        else if (ring >= ringsPerHemisphere + ringsOnBody) // bottom hemisphere
        {
            int relRing = ring - (ringsPerHemisphere + ringsOnBody);
            float v = static_cast<float>(relRing) / ringsPerHemisphere;
            float theta = (v * 0.5f + 0.5f) * M_PI; 
            y = -halfBodyHeight + radius * cosf(theta);
            r = radius * sinf(theta);
            yOffset = -halfBodyHeight;
        }
        else // cylinder body
        {
            int bodyRing = ring - ringsPerHemisphere;
            float v = static_cast<float>(bodyRing) / ringsOnBody;
            y = halfBodyHeight - v * (2.0f * halfBodyHeight);
            r = radius;
            yOffset = 0.0f;
        }

        for (int seg = 0; seg <= segments; ++seg)
        {
            float x = r * cosTable[seg];
            float z = r * sinTable[seg];

            Vector3 position = { x, y, z };
            Vector3 normal = Vector3(x, y - yOffset, z);
            normal.Normalize();

            Vector2 texcoord = { seg * horizontalStep, t };

            vertices.emplace_back(Vertex{ position, texcoord, normal, { 0, 0, 0 } });
        }
    }

    // Generate indices (triangle strips between each ring pair)
    for (int ring = 0; ring < totalRings; ++ring)
    {
        int ringStart = ring * vertexPerRing;
        int nextRingStart = (ring + 1) * vertexPerRing;

        for (int seg = 0; seg < segments; ++seg)
        {
            indices.push_back(ringStart + seg);
            indices.push_back(nextRingStart + seg);
            indices.push_back(nextRingStart + seg + 1);

            indices.push_back(ringStart + seg);
            indices.push_back(nextRingStart + seg + 1);
            indices.push_back(ringStart + seg + 1);
        }
    }

    // Calculate tangents
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

    // Build mesh
    MeshData capsuleData;
    capsuleData.m_vertexBuffer = std::make_unique<VertexBuffer>(vertices);
    capsuleData.m_indexBuffer = std::make_unique<IndexBuffer>(indices);
    capsuleData.m_indicesSize = static_cast<UINT>(indices.size());

    m_meshMap[MeshType::PRIMITIVE_CAPSULE] = std::move(capsuleData);
    Debug::Log("MeshManager: Loaded capsule primitive.");
}
