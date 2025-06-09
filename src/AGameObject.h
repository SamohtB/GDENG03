#pragma once
#include <string>
#include <directxtk12/SimpleMath.h>

using namespace DirectX::SimpleMath;

class DeviceContext;

class AGameObject
{
public:
    using String = std::string;

    AGameObject(String name);
    virtual ~AGameObject() = default;

    virtual void Update(float deltaTime) = 0;
    virtual void Draw(DeviceContext* dvcContext) = 0;

    bool IsActive() const;
    void SetActive(bool value);

    int GetId() const;
    void SetId(const UINT id);

    String GetName() const;
    void SetName(String name);

    void SetPosition(float x, float y, float z);
    void SetPosition(Vector3 vector);
    Vector3 GetLocalPosition();

    void SetRotation(float pitch, float yaw, float roll);
    void SetRotation(Vector3 vector);
    void Rotate(float pitch, float yaw, float roll);
    Vector3 GetLocalRotation();

    void SetScale(float x, float y, float z);
    void SetScale(Vector3 vector);
    void Scale(float scale);
    Vector3 GetLocalScale();

    Matrix GetLocalMatrix();

protected:
    unsigned int m_id = 0;    
    String m_name{};
    bool m_active = true;
    bool m_dirty = false;

    Vector3 m_local_position;
    Vector3 m_local_rotation;
    Vector3 m_local_scale;
    Matrix m_local_matrix;
};

