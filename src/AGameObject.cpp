#include "pch.h"
#include "AGameObject.h"
#include "PhysicsSystem.h"
#include "PhysicsComponent.h"
#include "Debug.h"

AGameObject::AGameObject(String name) : m_id(0), m_name(name), m_active(true)
{
	
}

AGameObject::~AGameObject()
{
	for (auto& component : m_componentList)
	{
		if (component->GetType() == AComponent::ComponentType::Physics)
		{
			if (auto phys = std::dynamic_pointer_cast<PhysicsComponent>(component))
			{
				PhysicsSystem::GetInstance()->UnregisterComponent(phys);
			}
		}

		component->DetachOwner();
	}
}

bool AGameObject::IsActive() const
{
	return m_active;
}

void AGameObject::SetActive(bool value)
{
	this->m_active = value;
}

int AGameObject::GetId() const
{
	return this->m_id;
}

void AGameObject::SetId(const UINT id)
{
	this->m_id = id;
}

String AGameObject::GetName() const
{
	return this->m_name;
}

void AGameObject::SetName(String name)
{
	this->m_name = name;
}

void AGameObject::AttachChild(std::shared_ptr<AGameObject> child)
{
	if (child->GetParent() != nullptr)
	{
		Debug::Log("Child already has a parent. Detaching from current parent.");
		child->GetParent()->DetachChild(child);
	}

	this->m_children.push_back(child);
	child->m_parent = shared_from_this();
	Debug::Log("Attached child: " + child->GetName() + " to parent: " + this->GetName());
}

void AGameObject::DetachChild(std::shared_ptr<AGameObject> child)
{
	auto it = std::find(m_children.begin(), m_children.end(), child);
	if (it == m_children.end()) return;
	child->m_parent.reset();
	m_children.erase(it);
	Debug::Log("Detached child: " + child->GetName() + " from parent: " + this->GetName());
}

AGameObject::ObjectList AGameObject::GetChildren() const
{
	return this->m_children;
}

AGameObject* AGameObject::GetParent() const
{
	return this->m_parent.lock().get();
}

void AGameObject::AttachComponent(std::shared_ptr<AComponent> component)
{
	this->m_componentList.push_back(component);
	component->AttachOwner(shared_from_this());
}

void AGameObject::DetachComponent(std::shared_ptr<AComponent> component)
{
	auto it = std::find(m_componentList.begin(), m_componentList.end(), component);
	if (it == m_componentList.end()) return;

	if (component->GetType() == AComponent::ComponentType::Physics)
	{
		if (auto physicsComponent = std::dynamic_pointer_cast<PhysicsComponent>(component))
		{
			PhysicsSystem::GetInstance()->UnregisterComponent(physicsComponent);
		}
	}

	component->DetachOwner();
	m_componentList.erase(it);

	Debug::Log("Detached " + component->GetName());
}

AComponent* AGameObject::FindComponentByName(String name)
{
	for (const auto& comp : m_componentList)
	{
		if (comp->GetName() == name)
		{
			return comp.get();
		}
	}

	return nullptr;
}

AComponent* AGameObject::FindComponentOfType(AComponent::ComponentType type)
{
	for (const auto& comp : m_componentList)
	{
		if (comp->GetType() == type)
		{
			return comp.get();
		}		
	}

	return nullptr;
}

AGameObject::ComponentList AGameObject::GetComponentsOfType(AComponent::ComponentType type)
{
	ComponentList foundList;

	for (const auto& comp : m_componentList)
	{
		if (comp->GetType() == type)
		{
			foundList.push_back(comp);
		}		
	}

	return foundList;
}

AGameObject::ComponentList AGameObject::GetComponentsOfTypeRecursive(AComponent::ComponentType type)
{
	ComponentList foundList;

	for (const auto& comp : m_componentList)
	{
		if (comp->GetType() == type)
			foundList.push_back(comp);
	}

	for (const auto& child : m_children)
	{
		ComponentList childList = child->GetComponentsOfTypeRecursive(type);
		foundList.insert(foundList.end(), childList.begin(), childList.end());
	}

	return foundList;
}

AGameObject::ComponentList AGameObject::GetAllComponents()
{
	return this->m_componentList;
}

std::shared_ptr<TransformComponent> AGameObject::Transform() const
{
	return m_transform;
}

void AGameObject::SetTransform(const std::shared_ptr<TransformComponent>& transform)
{
	m_transform = transform;
}