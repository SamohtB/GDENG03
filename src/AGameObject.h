#pragma once
#include "Math.h"
#include "AComponent.h"
#include "TransformComponent.h"

class DeviceContext;

class AGameObject : public std::enable_shared_from_this<AGameObject>
{
public:
    using ComponentPtr = std::shared_ptr<AComponent>;
    using ComponentList = std::vector<ComponentPtr>;

    AGameObject(String name);
    virtual ~AGameObject();

    virtual void Update(float deltaTime) = 0;

    bool IsActive() const;
    void SetActive(bool value);

    int GetId() const;
    void SetId(const UINT id);

    String GetName() const;
    void SetName(String name);

    // --- NEWLY ADDED METHODS ---
    String GetObjectType() const;
    void SetObjectType(const String& type);
    // --- END NEWLY ADDED METHODS ---

    void AttachComponent(std::shared_ptr<AComponent> component);
    void DetachComponent(std::shared_ptr<AComponent> component);

    AComponent* FindComponentByName(String name);
    AComponent* FindComponentOfType(AComponent::ComponentType type);
    ComponentList GetComponentsOfType(AComponent::ComponentType type);
    ComponentList GetComponentsOfTypeRecursive(AComponent::ComponentType type);
    ComponentList GetAllComponents();

    std::shared_ptr<TransformComponent> Transform() const;
    void SetTransform(const std::shared_ptr<TransformComponent>& transform);

protected:
    unsigned int m_id;
    String m_name;
    String m_objectType; // <-- NEW MEMBER VARIABLE
    bool m_active;

    std::shared_ptr<TransformComponent> m_transform;
    ComponentList m_componentList;
};