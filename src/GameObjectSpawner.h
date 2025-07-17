#pragma once

#include "Cube.h"
#include "Plane.h"
#include "Sphere.h"
#include "Cylinder.h"
#include "Camera.h"

#include "GameObjectManager.h"
#include "GraphicsEngine.h"
#include "LightManager.h"
#include "NameRegistry.h"

#include "PhysicsSystem.h"
#include "PhysicsComponent.h"

#include "Debug.h"

enum ObjectType
{
    PRIMITIVE_CUBE = 0,
    PRIMITIVE_PLANE,
    PRIMITIVE_SPHERE,
    PRIMITIVE_CYLINDER
};

class GameObjectSpawner
{
public:
    static void CreateLight()
    {
        std::string name = NameRegistry::GetInstance()->GenerateUniqueName("PointLight");
        GraphicsEngine::GetInstance()->GetLightManager()->CreateLight(
            name, Vector3(1.0f, 1.0f, 1.0f), 2.0f, 50.0f);
    }

    static void CreatePhysicsCube()
    {
        std::string name = NameRegistry::GetInstance()->GenerateUniqueName("PhysicsCube");
        auto obj = std::make_shared<Cube>(name);
		obj->SetPosition(0.0f, 10.0f, 0.0f);
		obj->SetMaterial(MaterialType::ROCK);

        if (obj)
        {
            auto physicsComponent = std::make_shared<PhysicsComponent>(name, obj);
            obj->AttachComponent(physicsComponent);
            PhysicsSystem::GetInstance()->RegisterComponent(physicsComponent);

            GameObjectManager::GetInstance()->AddGameObject(obj);
        }
        else
        {
            Debug::LogError("Physics Cube not Created! Error Occured GameObjectSpawner::CreatePhysicsCube ");
        }
	}

    static void CreateStaticPhysicsPlane()
    {
        std::string name = NameRegistry::GetInstance()->GenerateUniqueName("PhysicsPlane");
        auto obj = std::make_shared<Cube>(name);
		obj->SetScale(5.0f, 0.01f, 5.0f);

        if (obj)
        {
            auto physicsComponent = std::make_shared<PhysicsComponent>(name, obj);
            physicsComponent->SetBodyType(reactphysics3d::BodyType::STATIC);
            obj->AttachComponent(physicsComponent);
            PhysicsSystem::GetInstance()->RegisterComponent(physicsComponent);
            GameObjectManager::GetInstance()->AddGameObject(obj);
        }
        else
        {
            Debug::LogError("Static Physics Plane not Created! Error Occured GameObjectSpawner::CreateStaticPhysicsPlane ");
        }
	}

    static void CreatePrimitive(ObjectType type)
    {
        using GameObjectPtr = std::shared_ptr<AGameObject>;
        GameObjectPtr obj = nullptr;
        std::string name{};

        switch (type)
        {
        case PRIMITIVE_CUBE:
            name = NameRegistry::GetInstance()->GenerateUniqueName("Cube");
            obj = std::make_shared<Cube>(name);
            break;

        case PRIMITIVE_PLANE:
            name = NameRegistry::GetInstance()->GenerateUniqueName("Plane");
            obj = std::make_shared<Plane>(name);
            break;

        case PRIMITIVE_SPHERE:
            name = NameRegistry::GetInstance()->GenerateUniqueName("Sphere");
            obj = std::make_shared<Sphere>(name);
            break;

        case PRIMITIVE_CYLINDER:
            name = NameRegistry::GetInstance()->GenerateUniqueName("Cylinder");
            obj = std::make_shared<Cylinder>(name);
            break;

        default:
            return;
        }

        if (obj)
        {
            GameObjectManager::GetInstance()->AddGameObject(obj);
        }
        else
        {
            Debug::LogError("Primitive not Created! Error Occured GameObjectSpawner::CreatePrimitive ");
        }
    }

};