#pragma once
#include <string>

class DeviceContext;

class AGameObject
{
public:
    using String = std::string;

    AGameObject(int id, String name);
    virtual ~AGameObject() = default;

    virtual void Update() = 0;
    virtual void Draw(DeviceContext* dvcContext) = 0;

    bool IsActive() const;
    void SetActive(bool value);

    int GetId() const;

    String GetName() const;
    void SetName(String name);

private:
    unsigned int m_id = 0;    
    String m_name{};
    bool m_active = true;
};

