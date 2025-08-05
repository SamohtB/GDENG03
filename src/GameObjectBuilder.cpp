#include "pch.h"
#include "GameObjectBuilder.h"
#include "GameEntity.h"
#include "MeshComponent.h"
#include "PhysicsComponent.h"
#include "PhysicsSystem.h"

GameObjectBuilder::GameObjectBuilder()
{
	this->m_entity = std::make_shared<GameEntity>("");
}

GameObjectBuilder& GameObjectBuilder::SetExisting(std::shared_ptr<GameEntity> gameObject)
{
	m_entity = gameObject;
	return *this;
}

GameObjectBuilder& GameObjectBuilder::SetName(const String& name)
{
	m_entity->SetName(name);
	return *this;
}

GameObjectBuilder& GameObjectBuilder::AddTransformComponent(String name)
{
	auto componentName = m_entity->GetName() + " Transform";
	auto transform = std::make_shared<TransformComponent>(componentName, m_entity);
	m_entity->AttachComponent(transform);
	m_entity->SetTransform(transform);
	return *this;
}

GameObjectBuilder& GameObjectBuilder::AddMeshComponent(String mesh, String material)
{
	auto name = m_entity->GetName() + " Mesh Component";
	auto meshComponent = std::make_shared<MeshComponent>(name, mesh, m_entity);
	meshComponent->SetMaterial(material);
	m_entity->AttachComponent(meshComponent);
	return *this;
}


GameObjectBuilder& GameObjectBuilder::AddPhysicsComponent(String meshType, bool isStatic)
{
	auto name = m_entity->GetName() + " Physics Component";
	auto physicsComponent = std::make_shared<PhysicsComponent>(name, meshType, m_entity);
	m_entity->AttachComponent(physicsComponent);
	PhysicsSystem::GetInstance()->RegisterComponent(physicsComponent);

	if (isStatic)
	{
		physicsComponent->SetBodyType(rp3d::BodyType::STATIC);
	}
	else
	{
		physicsComponent->SetBodyType(rp3d::BodyType::DYNAMIC);
	}

	return *this;
}

std::shared_ptr<GameEntity> GameObjectBuilder::Build()
{
	return m_entity;
}

