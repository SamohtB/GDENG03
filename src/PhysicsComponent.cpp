#include "pch.h"
#include "PhysicsComponent.h"
#include "PhysicsSystem.h"
#include "AGameObject.h"
#include "Debug.h"

PhysicsComponent::PhysicsComponent(String name, std::weak_ptr<AGameObject> owner, String meshType) : AComponent(name, ComponentType::Physics, owner)
{
    auto* physicsCommon = PhysicsSystem::GetInstance()->GetPhysicsCommon();
    auto* physicsWorld = PhysicsSystem::GetInstance()->GetPhysicsWorld();

    Vector3 scale = m_owner.lock()->GetLocalScale();
    rp3d::BoxShape* boxShape;

    if (meshType == MeshType::PRIMITIVE_PLANE)
    {
        float half = 5.0f;
        boxShape = physicsCommon->createBoxShape(rp3d::Vector3(half * scale.x, 0.01f , half * scale.z));
    }
    else
    {
        float half = 0.5f;
        boxShape = physicsCommon->createBoxShape(rp3d::Vector3(half * scale.x, half * scale.y, half * scale.z));
    }

    Vector3 pos = m_owner.lock()->GetLocalPosition();
    rp3d::Quaternion rot = m_owner.lock()->GetLocalQuaternion();
    rp3d::Transform startTransform(rp3d::Vector3(pos.x, pos.y, pos.z), rot);

    m_rigidbody = physicsWorld->createRigidBody(startTransform);
    m_rigidbody->setType(rp3d::BodyType::DYNAMIC);
    m_rigidbody->enableGravity(true);

    rp3d::Transform identity = rp3d::Transform::identity();
    rp3d::Collider* collider = m_rigidbody->addCollider(boxShape, identity);
    collider->getMaterial().setFrictionCoefficient(0.5f);
    collider->getMaterial().setBounciness(0.0f);

    m_rigidbody->updateMassPropertiesFromColliders();
    m_rigidbody->setMass(this->m_mass);
}

PhysicsComponent::~PhysicsComponent()
{
    PhysicsSystem::GetInstance()->UnregisterComponent(shared_from_this());

    if (this->m_rigidbody)
    {
        reactphysics3d::PhysicsWorld* physicsWorld = PhysicsSystem::GetInstance()->GetPhysicsWorld();

        if (physicsWorld)
        {
            physicsWorld->destroyRigidBody(this->m_rigidbody);
        }

        this->m_rigidbody = nullptr;
    }
}

void PhysicsComponent::Perform(float deltaTime)
{
    const reactphysics3d::Transform transform = this->m_rigidbody->getTransform();

    rp3d::Vector3 position = transform.getPosition();
    this->GetOwner()->SetPosition(position.x, position.y, position.z);

    rp3d::Quaternion orientation = transform.getOrientation();
    this->GetOwner()->SetRotation(orientation);
}

reactphysics3d::RigidBody* PhysicsComponent::GetRigidBody()
{
    return this->m_rigidbody;
}

void PhysicsComponent::SetBodyType(reactphysics3d::BodyType type)
{
    this->m_rigidbody->setType(type);
}

