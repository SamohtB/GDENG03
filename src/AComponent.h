#pragma once
class AGameObject;

class AComponent : public std::enable_shared_from_this<AComponent>
{
public:
    enum ComponentType { NotSet = -1, Script = 0, Renderer, Input, Physics, Transform };

    AComponent(String name, ComponentType type, std::weak_ptr<AGameObject> owner);
    ~AComponent() = default;

    void AttachOwner(std::weak_ptr<AGameObject> owner);
    void DetachOwner();
    AGameObject* GetOwner();
    ComponentType GetType();
    String GetName();

    virtual void Perform() = 0;
    virtual void DrawUI() = 0;

    void DetachSelfFromOwner();

protected:
    std::weak_ptr<AGameObject> m_owner;
    ComponentType m_component_type;
    String m_name;
};