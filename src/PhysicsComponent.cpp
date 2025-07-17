#include "pch.h"
#include "PhysicsComponent.h"
#include "PhysicsSystem.h"
#include "AGameObject.h"
#include "Debug.h"

PhysicsComponent::PhysicsComponent(String name, std::weak_ptr<AGameObject> owner) : AComponent(name, ComponentType::Physics, owner)
{
    reactphysics3d::PhysicsCommon* physicsCommon = PhysicsSystem::GetInstance()->GetPhysicsCommon();
    reactphysics3d::PhysicsWorld* physicsWorld = PhysicsSystem::GetInstance()->GetPhysicsWorld();
 
    Vector3 scale = this->m_owner.lock()->GetLocalScale();
    reactphysics3d::BoxShape* boxShape = physicsCommon->createBoxShape(reactphysics3d::Vector3(scale.x / 2, scale.y / 2, scale.z / 2));


    Vector3 position = this->m_owner.lock()->GetLocalPosition();
	Vector3 rotation = this->m_owner.lock()->GetLocalRotation();

    reactphysics3d::Vector3 rp3dPosition(position.x, position.y, position.z);
    reactphysics3d::Quaternion orientation = reactphysics3d::Quaternion::fromEulerAngles(
        DirectX::XMConvertToRadians(rotation.x),
        DirectX::XMConvertToRadians(rotation.y),
        DirectX::XMConvertToRadians(rotation.z)
	);

    reactphysics3d::Transform transform(rp3dPosition, orientation);

    this->m_rigidbody = physicsWorld->createRigidBody(transform);
    this->m_rigidbody->addCollider(boxShape, transform);
    this->m_rigidbody->updateMassPropertiesFromColliders();
    this->m_rigidbody->setMass(this->m_mass);
    this->m_rigidbody->setType(reactphysics3d::BodyType::DYNAMIC);
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
    float matrix[16];
    transform.getOpenGLMatrix(matrix);

    rp3d::Vector3 position = transform.getPosition();
    this->GetOwner()->SetPosition(position.x, position.y, position.z);

    rp3d::Quaternion orientation = transform.getOrientation();
    DirectX::XMVECTOR quaternion = DirectX::XMVectorSet(orientation.x, orientation.y, orientation.z, orientation.w);

    // Convert to Euler angles in radians
    DirectX::XMFLOAT3 eulerRadians;
    XMStoreFloat3(&eulerRadians, DirectX::XMQuaternionRotationRollPitchYawFromVector(quaternion));

    Vector3 eulerDegrees = Vector3(
        DirectX::XMConvertToDegrees(eulerRadians.x),
        DirectX::XMConvertToDegrees(eulerRadians.y),
        DirectX::XMConvertToDegrees(eulerRadians.z)
    );

    this->GetOwner()->SetRotation(eulerDegrees);
}

reactphysics3d::RigidBody* PhysicsComponent::GetRigidBody()
{
    return this->m_rigidbody;
}

void PhysicsComponent::SetBodyType(reactphysics3d::BodyType type)
{
    this->m_rigidbody->setType(type);
}

