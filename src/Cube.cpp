#include "pch.h"
#include "Cube.h"
#include "MeshTypes.h"
#include "Random.h"

Cube::Cube(String name) : AMeshObject(name, MeshType::PRIMITIVE_CUBE)
{
    SetTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    SetMaterial(MaterialType::DEFAULT);
}

void Cube::Update(float deltaTime)
{

}