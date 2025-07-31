#pragma once
#include "AComponent.h"
#include "MeshTypes.h"

class PhysicsComponent : public AComponent, public std::enable_shared_from_this<PhysicsComponent>
{
public:
    PhysicsComponent(String name, std::weak_ptr<AGameObject> owner, String meshType);
    ~PhysicsComponent();

    void Perform(float deltaTime) override;
	void DrawUI() override;

    void SetSelf(std::shared_ptr<PhysicsComponent> self) { m_self = self; }

    reactphysics3d::RigidBody* GetRigidBody();

    void SetBodyType(reactphysics3d::BodyType type);
	void SetMass(float mass);

private:
    float m_mass;
    reactphysics3d::BodyType m_bodyType;
    reactphysics3d::RigidBody* m_rigidbody;
    std::weak_ptr<PhysicsComponent> m_self;
};
