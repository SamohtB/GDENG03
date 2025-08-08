#pragma once
#include "AComponent.h"
#include "MeshTypes.h"

// Forward-declare reactphysics3d classes
namespace reactphysics3d {
    class RigidBody;
    class Collider;
    class BoxShape;
}

class PhysicsComponent : public AComponent
{
public:
    PhysicsComponent(String name, String meshType, std::weak_ptr<AGameObject> owner);
    ~PhysicsComponent();

    void Perform() override;
    void DrawUI() override;

    void SetSelf(std::shared_ptr<PhysicsComponent> self) { m_self = self; }

    reactphysics3d::RigidBody* GetRigidBody();
    void UpdateTransformFromOwner();
    void UpdateColliderShape(); // [NEW] Add this declaration

    void SetBodyType(reactphysics3d::BodyType type);
    void SetMass(float mass);

    reactphysics3d::BodyType GetBodyType() const;

private:
    float m_deltaTime;
    float m_mass;
    reactphysics3d::BodyType m_bodyType;
    reactphysics3d::RigidBody* m_rigidbody;
    reactphysics3d::Collider* m_collider;   // [NEW] Store the collider
    reactphysics3d::BoxShape* m_boxShape;     // [NEW] Store the shape

    std::weak_ptr<PhysicsComponent> m_self;
};