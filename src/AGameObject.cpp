#include "AGameObject.h"

AGameObject::AGameObject(String name) : m_id(0), m_name(name), m_active(true), m_dirty(false)
{
	this->m_local_position = Vector3(0, 0, 0);
	this->m_local_rotation = Vector3(0, 0, 0);
	this->m_local_scale = Vector3(1, 1, 1);
}

bool AGameObject::IsActive() const
{
	return m_active;
}

void AGameObject::SetActive(bool value)
{
	this->m_active = value;
}

int AGameObject::GetId() const
{
	return this->m_id;
}

void AGameObject::SetId(const UINT id)
{
	this->m_id = id;
}

AGameObject::String AGameObject::GetName() const
{
	return this->m_name;
}

void AGameObject::SetName(String name)
{
	this->m_name = name;
}

void AGameObject::SetPosition(float x, float y, float z)
{
	this->m_local_position = Vector3(x, y, z);
	this->m_dirty = true;
}

void AGameObject::SetPosition(Vector3 vector)
{
	this->m_local_position = vector;
	this->m_dirty = true;
}

Vector3 AGameObject::GetLocalPosition()
{
	return this->m_local_position;
}

void AGameObject::SetRotation(float pitch, float yaw, float roll)
{
	this->m_local_rotation = Vector3(pitch, yaw, roll);
	this->m_dirty = true;
}

void AGameObject::SetRotation(Vector3 vector)
{
	this->m_local_rotation = vector;
	this->m_dirty = true;
}

void AGameObject::Rotate(float pitch, float yaw, float roll)
{
	this->m_local_rotation += Vector3(pitch, yaw, roll);
	this->m_dirty = true;
}

Vector3 AGameObject::GetLocalRotation()
{
	return this->m_local_rotation;
}

void AGameObject::SetScale(float x, float y, float z)
{
	this->m_local_scale = Vector3(x, y, z);
	this->m_dirty = true;
}

void AGameObject::SetScale(Vector3 vector)
{
	this->m_local_scale = vector;
	this->m_dirty = true;
}

void AGameObject::Scale(float scale)
{
	this->m_local_scale += Vector3(scale, scale, scale);
	this->m_dirty = true;
}

Vector3 AGameObject::GetLocalScale()
{
	return this->m_local_scale;
}

Matrix AGameObject::GetLocalMatrix()
{
	if (this->m_dirty)
	{
		this->m_dirty = false;

		Matrix scaleMatrix = Matrix::CreateScale(m_local_scale);
		Matrix rotationMatrix = Matrix::CreateFromYawPitchRoll(
			m_local_rotation.y, m_local_rotation.x, m_local_rotation.z);
		Matrix translationMatrix = Matrix::CreateTranslation(m_local_position);

		m_local_matrix = scaleMatrix * rotationMatrix * translationMatrix;
	}
	
	return m_local_matrix;
}
