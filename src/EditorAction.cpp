#include "pch.h"
#include "EditorAction.h"
#include "AGameObject.h"

EditorAction::EditorAction(AGameObject* gameObject)
{
	this->m_objectName = gameObject->GetName();
	this->m_localPosition = gameObject->Transform()->GetLocalPosition();
	this->m_orientation = gameObject->Transform()->GetLocalQuaternion();
	this->m_localScale = gameObject->Transform()->GetLocalScale();
}

String EditorAction::GetOwnerName()
{
	return this->m_objectName;
}

Vector3 EditorAction::GetStorePos()
{
	return this->m_localPosition;
}

Vector3 EditorAction::GetStoredScale()
{
	return this->m_localScale;
}

rp3d::Quaternion EditorAction::GetStoredOrientation()
{
	return this->m_orientation;
}