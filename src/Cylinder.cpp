#include "pch.h"
#include "Cylinder.h"
#include "MeshTypes.h"
#include "Random.h"

Cylinder::Cylinder(String name) : AMeshObject(name, MeshType::PRIMITIVE_CYLINDER)
{
    SetTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    SetMaterial(MaterialType::DEFAULT);
}


void Cylinder::Update(float deltaTime)
{
}
