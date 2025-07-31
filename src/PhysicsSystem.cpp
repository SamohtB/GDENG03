#include "pch.h"
#include "PhysicsSystem.h"
#include "PhysicsComponent.h"
#include "EngineTime.h"
#include "Debug.h"

std::unique_ptr<PhysicsSystem> PhysicsSystem::sharedInstance = nullptr;

PhysicsSystem* PhysicsSystem::GetInstance()
{
    return sharedInstance.get();
}

void PhysicsSystem::Initialize()
{
    try
    {
        sharedInstance = std::make_unique<PhysicsSystem>();
    }

    catch (...)
    {
        Debug::LogError("Physics System initialization failed!");
		return;
    }
    
}

void PhysicsSystem::Destroy()
{
    sharedInstance.reset();
}

PhysicsSystem::PhysicsSystem()
{
    this->m_physicsCommon = std::make_unique<reactphysics3d::PhysicsCommon>();

    reactphysics3d::PhysicsWorld::WorldSettings settings;
    settings.defaultVelocitySolverNbIterations = 50;
    settings.gravity = reactphysics3d::Vector3(0, -9.81f, 0);

    this->m_physicsWorld = this->m_physicsCommon->createPhysicsWorld(settings);

	Debug::Log("PhysicsSystem initialized with gravity: " + std::to_string(settings.gravity.y));
}

PhysicsSystem::~PhysicsSystem()
{
    if (m_physicsWorld) {
        m_physicsCommon->destroyPhysicsWorld(m_physicsWorld);
        m_physicsWorld = nullptr;
    }
}

void PhysicsSystem::RegisterComponent(PhysicsPtr physicsComponent)
{
    this->m_componentList.push_back(physicsComponent);
    this->m_componentTable[physicsComponent->GetName()] = physicsComponent;
}

void PhysicsSystem::UnregisterComponent(PhysicsPtr physicsComponent)
{
    if (!physicsComponent) return;

    const String& name = physicsComponent->GetName();

    auto it = m_componentTable.find(name);
    if (it != m_componentTable.end())
    {
        m_componentTable.erase(it);
    }

    m_componentList.erase(
        std::remove(m_componentList.begin(), m_componentList.end(), physicsComponent),
        m_componentList.end()
    );
}

void PhysicsSystem::UnregisterComponentByName(String name)
{
    auto it = m_componentTable.find(name);
    if (it == m_componentTable.end()) return;

    PhysicsPtr physicsComponent = it->second;

    m_componentList.erase(
        std::remove(m_componentList.begin(), m_componentList.end(), physicsComponent),
        m_componentList.end()
    );

    m_componentTable.erase(it);
}

PhysicsComponent* PhysicsSystem::FindComponentByName(String name)
{
    auto it = m_componentTable.find(name);
    if (it != m_componentTable.end())
    {
        return it->second.get();
    }

    return nullptr;
}

PhysicsSystem::PhysicsComponentList PhysicsSystem::GetAllComponents()
{
    return this->m_componentList;
}

void PhysicsSystem::UpdateAllComponents(float deltaTime)
{

    if (deltaTime > 0.0f)
    {
        this->m_physicsWorld->update(deltaTime);

        for (PhysicsPtr physics : this->m_componentList)
        {
            physics->Perform(deltaTime);
        }
    }

}

reactphysics3d::PhysicsWorld* PhysicsSystem::GetPhysicsWorld()
{
    return this->m_physicsWorld;
}

reactphysics3d::PhysicsCommon* PhysicsSystem::GetPhysicsCommon()
{
    return this->m_physicsCommon.get();
}
