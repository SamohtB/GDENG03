#include "pch.h"
#include "CustomMesh.h"

CustomMesh::CustomMesh(String name, String mesh) : AMeshObject(name, mesh)
{
	SetTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	SetMaterial(MaterialType::DEFAULT);
}

void CustomMesh::Update(float deltaTime)
{

}
