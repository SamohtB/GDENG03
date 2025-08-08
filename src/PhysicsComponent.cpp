#include "pch.h"
#include "PhysicsComponent.h"
#include "PhysicsSystem.h"
#include "AGameObject.h"
#include "TransformComponent.h" // [NEW] Include TransformComponent for scale access
#include "Debug.h"

PhysicsComponent::PhysicsComponent(String name, String meshType, std::weak_ptr<AGameObject> owner)
    : AComponent(name, ComponentType::Physics, owner), m_mass(100.0f), m_bodyType(reactphysics3d::BodyType::STATIC), m_deltaTime(0)
{
    auto* physicsCommon = PhysicsSystem::GetInstance()->GetPhysicsCommon();
    auto* physicsWorld = PhysicsSystem::GetInstance()->GetPhysicsWorld();

    Vector3 scale = m_owner.lock()->Transform()->GetLocalScale();

    // Todo: Fix Shape Setting
    if (meshType == MeshType::PRIMITIVE_PLANE)
    {
        float half = 5.0f;
        m_boxShape = physicsCommon->createBoxShape(rp3d::Vector3(half * scale.x, 0.10f, half * scale.z)); // [CHANGE] Assign to m_boxShape
    }
    else
    {
        float half = 0.5f;
        m_boxShape = physicsCommon->createBoxShape(rp3d::Vector3(half * scale.x, half * scale.y, half * scale.z)); // [CHANGE] Assign to m_boxShape
    }

    Vector3 pos = m_owner.lock()->Transform()->GetLocalPosition();
    rp3d::Quaternion rot = m_owner.lock()->Transform()->GetLocalQuaternion();
    rp3d::Transform startTransform(rp3d::Vector3(pos.x, pos.y, pos.z), rot);

    m_rigidbody = physicsWorld->createRigidBody(startTransform);
    m_rigidbody->setType(m_bodyType);
    m_rigidbody->enableGravity(true);
    m_rigidbody->setIsAllowedToSleep(true);
    m_rigidbody->setAngularDamping(0.8f);
    m_rigidbody->setLinearDamping(0.5f);

    rp3d::Transform identity = rp3d::Transform::identity();
    m_collider = m_rigidbody->addCollider(m_boxShape, identity); // [CHANGE] Assign to m_collider
    m_collider->getMaterial().setFrictionCoefficient(1.0f);
    m_collider->getMaterial().setBounciness(0.0f);

    m_rigidbody->updateMassPropertiesFromColliders();
    m_rigidbody->setMass(this->m_mass);
}

PhysicsComponent::~PhysicsComponent()
{
    // The destructor doesn't need changes.
    // The physics world will destroy the rigid body, which in turn destroys the attached colliders and shapes.
    if (this->m_rigidbody)
    {
        auto* physicsWorld = PhysicsSystem::GetInstance()->GetPhysicsWorld();
        if (physicsWorld)
        {
            physicsWorld->destroyRigidBody(this->m_rigidbody);
        }
    }
}

// [NEW] Implement the collider update function
void PhysicsComponent::UpdateColliderShape()
{
    if (!m_owner.lock() || !m_boxShape) return;

    Vector3 scale = m_owner.lock()->Transform()->GetLocalScale();
    std::string meshType = "SOME_DEFAULT"; // You need a way to get the mesh type here.
    // For this example, we assume we can recalculate without it,
    // but ideally, you'd store the mesh type in the component.

// A simple way to differentiate between plane and other objects without storing meshType
// This is a bit of a hack; storing the mesh type or shape parameters would be cleaner.
    bool isPlane = (m_boxShape->getHalfExtents().y < 0.5f); // Inferring based on initial small height

    if (isPlane)
    {
        float half = 5.0f;
        m_boxShape->setHalfExtents(rp3d::Vector3(half * scale.x, 0.10f, half * scale.z));
    }
    else
    {
        float half = 0.5f;
        m_boxShape->setHalfExtents(rp3d::Vector3(half * scale.x, half * scale.y, half * scale.z));
    }

    // After changing the shape, we need to recompute the mass properties of the rigid body
    m_rigidbody->updateMassPropertiesFromColliders();

    // You might need to re-apply the mass if it's not dynamic or if you want to override the computed inertia tensor
    if (m_bodyType == reactphysics3d::BodyType::DYNAMIC)
    {
        m_rigidbody->setMass(m_mass);
    }

    Debug::Log("Collider shape updated.");
}


void PhysicsComponent::Perform()
{
    const reactphysics3d::Transform transform = this->m_rigidbody->getTransform();

    rp3d::Vector3 position = transform.getPosition();
    this->GetOwner()->Transform()->SetPosition(position.x, position.y, position.z);

    rp3d::Quaternion orientation = transform.getOrientation();
    this->GetOwner()->Transform()->SetRotation(orientation);
}

void PhysicsComponent::DrawUI()
{
    if (ImGui::CollapsingHeader("Physics Component", ImGuiTreeNodeFlags_DefaultOpen))
    {
        // === Set Mass ===
        ImGui::Text("Mass:");
        ImGui::SetNextItemWidth(200.0f);
        if (ImGui::DragFloat("##MassInput", &this->m_mass, 0.1f, 1.0f, 1000000.0f, "%.2f"))
        {
            this->m_rigidbody->setMass(this->m_mass);
        }

        // === Set Body Type ===
        static const char* bodyTypeNames[] = { "Static", "Kinematic", "Dynamic" };
        int selectedBodyType = static_cast<int>(this->m_bodyType);
        ImGui::Text("Body Type:");
        if (ImGui::Combo("##BodyType", &selectedBodyType, bodyTypeNames, IM_ARRAYSIZE(bodyTypeNames)))
        {
            this->SetBodyType(static_cast<reactphysics3d::BodyType>(selectedBodyType));
            this->m_bodyType = static_cast<reactphysics3d::BodyType>(selectedBodyType);
        }

        // === Gravity Settings ===
        bool gravityEnabled = this->m_rigidbody->isGravityEnabled();
        if (ImGui::Checkbox("Enable Gravity", &gravityEnabled))
        {
            this->m_rigidbody->enableGravity(gravityEnabled);
        }

        // === Linear Damping ===
        ImGui::Text("Linear Damping:");
        ImGui::SetNextItemWidth(200.0f);
        float linearDamping = this->m_rigidbody->getLinearDamping();
        if (ImGui::SliderFloat("##LinearDamping", &linearDamping, 0.0f, 10.0f, "%.2f"))
        {
            this->m_rigidbody->setLinearDamping(linearDamping);
        }

        // === Angular Damping ===
        ImGui::Text("Angular Damping:");
        ImGui::SetNextItemWidth(200.0f);
        float angularDamping = this->m_rigidbody->getAngularDamping();
        if (ImGui::SliderFloat("##AngularDamping", &angularDamping, 0.0f, 10.0f, "%.2f"))
        {
            this->m_rigidbody->setAngularDamping(angularDamping);
        }


        // === Detach Component Button ===
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        if (ImGui::Button("Delete Physics Component"))
        {
			this->DetachSelfFromOwner();
        }
    }
}

reactphysics3d::RigidBody* PhysicsComponent::GetRigidBody()
{
    return this->m_rigidbody;
}

void PhysicsComponent::UpdateTransformFromOwner()
{
    Vector3 pos = m_owner.lock()->Transform()->GetLocalPosition();
    rp3d::Quaternion rot = m_owner.lock()->Transform()->GetLocalQuaternion();
    rp3d::Transform newTransform(rp3d::Vector3(pos.x, pos.y, pos.z), rot);
    m_rigidbody->setTransform(newTransform);
}

void PhysicsComponent::SetBodyType(reactphysics3d::BodyType type)
{
    this->m_rigidbody->setType(type);
    this->m_bodyType = type;
}

void PhysicsComponent::SetMass(float mass)
{
    this->m_rigidbody->setMass(mass);
    this->m_mass = mass;
}

reactphysics3d::BodyType PhysicsComponent::GetBodyType() const
{
    return m_bodyType;
}

