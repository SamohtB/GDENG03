#include "pch.h"
#include "BaseComponentSystem.h"
#include "PhysicsSystem.h"

std::unique_ptr<BaseComponentSystem> BaseComponentSystem::sharedInstance = nullptr;

BaseComponentSystem* BaseComponentSystem::GetInstance()
{
    return sharedInstance.get();
}

void BaseComponentSystem::Initialize()
{
    sharedInstance = std::make_unique<BaseComponentSystem>();
}

void BaseComponentSystem::Destroy()
{
    sharedInstance.reset();
}

PhysicsSystem* BaseComponentSystem::GetPhysicsSystem()
{
    return this->m_physicsSystem.get();
}

BaseComponentSystem::BaseComponentSystem()
{
    this->m_physicsSystem = std::make_unique<PhysicsSystem>();
}

