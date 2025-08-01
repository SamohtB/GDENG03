#pragma once
#include "AComponent.h"
#include "MeshTypes.h"

class PhysicsComponent : public AComponent
{
public:
    PhysicsComponent(String name, String meshType,  std::weak_ptr<AGameObject> owner);
    ~PhysicsComponent();

    void Perform() override;
	void DrawUI() override;

    void SetSelf(std::shared_ptr<PhysicsComponent> self) { m_self = self; }

    reactphysics3d::RigidBody* GetRigidBody();

    void SetBodyType(reactphysics3d::BodyType type);
	void SetMass(float mass);

private:
    float m_deltaTime;
    float m_mass;
    reactphysics3d::BodyType m_bodyType;
    reactphysics3d::RigidBody* m_rigidbody;
    std::weak_ptr<PhysicsComponent> m_self;
};
