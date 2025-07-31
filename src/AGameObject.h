#pragma once
#include "Math.h"
#include "AComponent.h"

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

    void SetPosition(float x, float y, float z);
    void SetPosition(Vector3 vector);
    void Move(float x, float y, float z);
    void Move(Vector3 vector);
    Vector3 GetLocalPosition();

    void SetRotation(float pitch, float yaw, float roll);
    void SetRotation(Vector3 vector);
	void SetRotation(rp3d::Quaternion quaternion);
    void Rotate(float pitch, float yaw, float roll);
    Vector3 GetLocalRotation();
	rp3d::Quaternion GetLocalQuaternion() const;

    void SetScale(float x, float y, float z);
    void SetScale(Vector3 vector);
    void Scale(float scale);
    Vector3 GetLocalScale();

    Matrix GetLocalMatrix();
    Matrix GetPhysicsLocalMatrix(); //Transposed
    void SetLocalMatrix(const float* matrixData);

    Vector3 GetForwardVector() const;
    Vector3 GetRightVector() const;
    Vector3 GetUpVector() const;

    void AttachComponent(std::shared_ptr<AComponent> component);
    void DetachComponent(std::shared_ptr<AComponent> component);

    AComponent* FindComponentByName(String name);
    AComponent* FindComponentOfType(AComponent::ComponentType type, String name);
    ComponentList GetComponentsOfType(AComponent::ComponentType type);
    ComponentList GetComponentsOfTypeRecursive(AComponent::ComponentType type);
    ComponentList GetAllComponents();

protected:
    unsigned int m_id;
    String m_name;
    bool m_active;
    bool m_dirty;

    Vector3 m_localPosition;
    rp3d::Quaternion m_localRotation;
    Vector3 m_localScale;
    Matrix m_localMatrix;

    ComponentList m_componentList;
};