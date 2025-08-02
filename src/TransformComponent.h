#pragma once
#include "AComponent.h"
#include "Math.h"

class TransformComponent : public AComponent
{
public:
    TransformComponent(String name, std::weak_ptr<AGameObject> owner);
	~TransformComponent() = default;

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

private:
    Vector3 m_localPosition;
    rp3d::Quaternion m_localRotation;
    Vector3 m_localScale;

    mutable Matrix m_localMatrix;
    mutable bool m_dirty;


    // Inherited via AComponent
    void Perform() override;

    void DrawUI() override;

};

