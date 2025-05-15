#include "AGameObject.h"

AGameObject::AGameObject(int id, String name) : m_id(id), m_name(name), m_active(true) {}

bool AGameObject::IsActive()
{
	return m_active;
}

void AGameObject::SetActive(bool value)
{
	this->m_active = value;
}

int AGameObject::GetId()
{
	return this->m_id;
}

AGameObject::String AGameObject::GetName()
{
	return this->m_name;
}

void AGameObject::SetName(String name)
{
	this->m_name = name;
}
