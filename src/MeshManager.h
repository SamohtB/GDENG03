#pragma once
#include "BatchUploader.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

struct MeshData
{
    std::unique_ptr<VertexBuffer> m_vertexBuffer;
    std::unique_ptr<IndexBuffer> m_indexBuffer;
	UINT m_indicesSize;
};

class MeshManager
{
public:
    MeshManager(std::shared_ptr<BatchUploader> uploader);
    ~MeshManager() = default;

    using MeshMap = std::unordered_map<String, MeshData>;

    void LoadInitialMeshes();
    void LoadMesh(const String& meshName, const std::wstring& filePath);

    MeshData* GetMeshData(const String& meshName);

    std::vector<const char*> GetAllMeshNames() const;

private:
    void LoadCubePrimitive();
	void LoadPlanePrimitive();
	void LoadSpherePrimitive();
	void LoadCylinderPrimitive();
	void LoadCapsulePrimitive();

	/* Cube Properties */ 
	float m_cubeSize = 1.0f;

    /* Plane Properties */
	float m_planeSize = 10.0f;

    /* Sphere Properties */
    int m_sphereTessellation = 20;
    float m_sphereRadius = 0.5f;

	/* Cylinder Properties */ 
    int m_cylinderSegments = 20;
    float m_cylinderRadius = 0.5f;
    float m_cylinderHeight = 2.0f;

    /* Capsule Properties */
    float m_capsuleRadius = 0.5f;           
    float m_capsuleHeight = 2.0f;           
    int   m_capsuleSegments = 24;          
    int   m_capsuleRings = 12;

    std::shared_ptr<BatchUploader> m_uploader;
    MeshMap m_meshMap;
};

