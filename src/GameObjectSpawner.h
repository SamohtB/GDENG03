#pragma once
#include "Camera.h"

#include "GameObjectManager.h"
#include "GraphicsEngine.h"
#include "LightManager.h"
#include "NameRegistry.h"
#include "MeshTypes.h"

#include "PhysicsSystem.h"
#include "PhysicsComponent.h"

#include "MaterialManager.h"
#include "Debug.h"

enum ObjectType
{
    PRIMITIVE_CUBE = 0,
    PRIMITIVE_PLANE,
    PRIMITIVE_SPHERE,
    PRIMITIVE_CYLINDER,
    BUNNY,
    ARMADILLO,
    TEAPOT
};

class GameObjectSpawner
{
public:
    static void CreateLight()
    {
        
    }
};