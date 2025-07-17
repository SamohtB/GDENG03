#include "pch.h"
#include "AComponent.h"

AComponent::AComponent(String name, ComponentType type, std::weak_ptr<AGameObject> owner) : m_name(name), m_component_type(type), m_owner(owner)
{
}

void AComponent::AttachOwner(std::weak_ptr<AGameObject> owner)
{
    this->m_owner = owner;
}

void AComponent::DetachOwner()
{
    m_owner.reset();
}

AGameObject* AComponent::GetOwner()
{
    auto shared = m_owner.lock();
    if (!shared) {
        throw std::runtime_error("AComponent: Owner no longer exists.");
    }
    return shared.get();
}

AComponent::ComponentType AComponent::GetType()
{
    return this->m_component_type;
}

String AComponent::GetName()
{
    return this->m_name;
}