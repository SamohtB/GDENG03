#pragma once
#include "AComponent.h"
#include "InputListener.h"
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

	Matrix GetWorldMatrix();
	void UpdateWorldMatrix(Matrix parentMatrix);

    Vector3 GetForwardVector() const;
    Vector3 GetRightVector() const;
    Vector3 GetUpVector() const;

	void PropagateDirtyFlag();

private:
    Vector3 m_localPosition;
    rp3d::Quaternion m_localRotation;
    Vector3 m_localScale;

    Matrix m_localMatrix;
    bool m_dirty;

    Matrix m_worldMatrix;
	bool m_worldDirty;

    bool m_recordedPosition = false;
    bool m_recordedRotation = false;
    bool m_recordedScale = false;

    // Inherited via AComponent
    void Perform() override;
    void DrawUI() override;

};

