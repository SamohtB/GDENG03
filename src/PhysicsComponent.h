#pragma once
#include "AComponent.h"
#include "MeshTypes.h"

class PhysicsComponent : public AComponent, public std::enable_shared_from_this<PhysicsComponent>
{
public:
    PhysicsComponent(String name, std::weak_ptr<AGameObject> owner, String meshType);
    ~PhysicsComponent();

    void Perform(float deltaTime) override;

    reactphysics3d::RigidBody* GetRigidBody();
    void SetBodyType(reactphysics3d::BodyType type);

private:
    float m_mass = 1000.0f;
    reactphysics3d::RigidBody* m_rigidbody;
};
