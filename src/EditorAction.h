#pragma once
#include "Math.h"

class AGameObject;

class EditorAction
{
public:
    EditorAction(AGameObject* gameObject);
    ~EditorAction() = default;

    String GetOwnerName();
    Vector3 GetStorePos();
    Vector3 GetStoredScale();
    rp3d::Quaternion GetStoredOrientation();

private:
    String m_objectName;
    Vector3 m_localPosition;
    Vector3 m_localScale;
    rp3d::Quaternion m_orientation;
};

