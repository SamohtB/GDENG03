#include "pch.h"
#include "Sphere.h"
#include "MeshTypes.h"

Sphere::Sphere(String name) : AMeshObject(name, MeshType::PRIMITIVE_SPHERE)
{
    SetTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    SetMaterial(MaterialType::DEFAULT);
}

void Sphere::Update(float deltaTime)
{

}