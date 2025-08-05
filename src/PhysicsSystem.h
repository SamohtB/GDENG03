#pragma once
class PhysicsComponent;

class PhysicsSystem
{
public:
    using PhysicsPtr = std::shared_ptr<PhysicsComponent>;
    using ComponentTable = std::unordered_map<String, PhysicsPtr>;
    using PhysicsComponentList = std::vector<PhysicsPtr>;

    static PhysicsSystem* GetInstance();
    static void Initialize();
    static void Destroy();

    void RegisterComponent(PhysicsPtr physicsComponent);
    void UnregisterComponent(PhysicsPtr physicsComponent);
    void UnregisterComponentByName(String name);
    PhysicsComponent* FindComponentByName(String name);
    PhysicsComponentList GetAllComponents();

    void SynchronizeTransformsToRigidBodies();

    void UpdateAllComponents(float deltaTime);
    reactphysics3d::PhysicsWorld* GetPhysicsWorld();
    reactphysics3d::PhysicsCommon* GetPhysicsCommon();

    PhysicsSystem();
    ~PhysicsSystem();
    PhysicsSystem(PhysicsSystem const&) = delete;
    PhysicsSystem& operator=(PhysicsSystem const&) = delete;

private:
    static std::unique_ptr<PhysicsSystem> sharedInstance;

    ComponentTable m_componentTable;
    PhysicsComponentList m_componentList;

    std::unique_ptr<reactphysics3d::PhysicsCommon> m_physicsCommon;
    reactphysics3d::PhysicsWorld* m_physicsWorld;

};
