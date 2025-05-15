#pragma once
#include "stdafx.h"

class AGameObject
{
public:
    using String = std::string;

    AGameObject(int id, String name);
    virtual ~AGameObject() = default;

    virtual void Update() = 0;
    virtual void Draw(ID3D12GraphicsCommandList* cmdList) = 0;

    bool IsActive();
    void SetActive(bool value);

    int GetId();

    String GetName();
    void SetName(String name);

private:
    unsigned int m_id = 0;    
    String m_name{};
    bool m_active = true;
};

