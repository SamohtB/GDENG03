#include "pch.h"
#include "Plane.h"
#include "MeshTypes.h"

Plane::Plane(String name) : AMeshObject(name, MeshType::PRIMITIVE_PLANE)
{
    SetTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    SetMaterial(MaterialType::DEFAULT);
}

void Plane::Update(float deltaTime)
{
}