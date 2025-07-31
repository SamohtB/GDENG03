#include "pch.h"
#include "AGameObject.h"
#include "PhysicsSystem.h"
#include "PhysicsComponent.h"
#include "Debug.h"

AGameObject::AGameObject(String name) : m_id(0), m_name(name), m_active(true), m_dirty(false)
{
	this->m_localPosition = Vector3(0, 0, 0);
	this->m_localRotation = rp3d::Quaternion::identity();
	this->m_localScale = Vector3(1, 1, 1);
	this->m_localMatrix = GetLocalMatrix();
}

AGameObject::~AGameObject()
{
	for (auto& component : m_componentList)
	{
		if (component->GetType() == AComponent::ComponentType::Physics)
		{
			if (auto phys = std::dynamic_pointer_cast<PhysicsComponent>(component))
			{
				PhysicsSystem::GetInstance()->UnregisterComponent(phys);
			}
		}

		component->DetachOwner();
	}
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

String AGameObject::GetName() const
{
	return this->m_name;
}

void AGameObject::SetName(String name)
{
	this->m_name = name;
}

void AGameObject::SetPosition(float x, float y, float z)
{
	this->m_localPosition = Vector3(x, y, z);
	this->m_dirty = true;
}

void AGameObject::SetPosition(Vector3 vector)
{
	this->m_localPosition = vector;
	this->m_dirty = true;
}

void AGameObject::Move(float x, float y, float z)
{
	this->m_localPosition += Vector3(x, y, z);
	this->m_dirty = true;
}

void AGameObject::Move(Vector3 vector)
{
	this->m_localPosition += vector;
	this->m_dirty = true;
}

Vector3 AGameObject::GetLocalPosition()
{
	return this->m_localPosition;
}

void AGameObject::SetRotation(float pitch, float yaw, float roll)
{
	using namespace DirectX;

	XMVECTOR q = XMQuaternionRotationRollPitchYaw(
		XMConvertToRadians(pitch),
		XMConvertToRadians(yaw),
		XMConvertToRadians(roll)
	);

	m_localRotation = rp3d::Quaternion(
		XMVectorGetX(q),
		XMVectorGetY(q),
		XMVectorGetZ(q),
		XMVectorGetW(q)
	);

	m_dirty = true;
}

void AGameObject::SetRotation(Vector3 vector)
{
	SetRotation(vector.x, vector.y, vector.z);
}

void AGameObject::SetRotation(rp3d::Quaternion quaternion)
{
	this->m_localRotation = quaternion;
	this->m_dirty = true;
}

void AGameObject::Rotate(float pitch, float yaw, float roll)
{
	using namespace DirectX;
	XMVECTOR deltaQ = XMQuaternionRotationRollPitchYaw(
		XMConvertToRadians(pitch),
		XMConvertToRadians(yaw),
		XMConvertToRadians(roll)
	);

	rp3d::Quaternion q(
		XMVectorGetX(deltaQ),
		XMVectorGetY(deltaQ),
		XMVectorGetZ(deltaQ),
		XMVectorGetW(deltaQ)
	);

	m_localRotation = q * m_localRotation;
	m_localRotation.normalize();

	m_dirty = true;
}

Vector3 AGameObject::GetLocalRotation()
{
	float x = m_localRotation.x;
	float y = m_localRotation.y;
	float z = m_localRotation.z;
	float w = m_localRotation.w;

	// Convert quaternion to Euler angles (pitch, yaw, roll)
	float pitch = std::asin(Clamp(2.0f * (w * y - z * x), -1.0f, 1.0f));
	float roll = std::atan2(2.0f * (w * x + y * z), 1.0f - 2.0f * (x * x + y * y));
	float yaw = std::atan2(2.0f * (w * z + x * y), 1.0f - 2.0f * (y * y + z * z));

	return Vector3(
		DirectX::XMConvertToDegrees(pitch),
		DirectX::XMConvertToDegrees(yaw),
		DirectX::XMConvertToDegrees(roll)
	);
}

rp3d::Quaternion AGameObject::GetLocalQuaternion() const
{
	return this->m_localRotation;
}

void AGameObject::SetScale(float x, float y, float z)
{
	this->m_localScale = Vector3(x, y, z);
	this->m_dirty = true;
}

void AGameObject::SetScale(Vector3 vector)
{
	this->m_localScale = vector;
	this->m_dirty = true;
}

void AGameObject::Scale(float scale)
{
	this->m_localScale += Vector3(scale, scale, scale);
	this->m_dirty = true;
}

Vector3 AGameObject::GetLocalScale()
{
	return this->m_localScale;
}

Matrix AGameObject::GetLocalMatrix()
{
	if (this->m_dirty)
	{
		this->m_dirty = false;

		Matrix scaleMatrix = Matrix::CreateScale(m_localScale);

		DirectX::XMVECTOR q = DirectX::XMVectorSet(
			m_localRotation.x,
			m_localRotation.y,
			m_localRotation.z,
			m_localRotation.w
		);

		Matrix rotationMatrix = Matrix::CreateFromQuaternion(q);
		Matrix translationMatrix = Matrix::CreateTranslation(m_localPosition);

		m_localMatrix = scaleMatrix * rotationMatrix * translationMatrix;
	}

	return m_localMatrix;
}

Matrix AGameObject::GetPhysicsLocalMatrix()
{
	Matrix localMatrix = this->GetLocalMatrix();
	return localMatrix.Transpose();
}

void AGameObject::SetLocalMatrix(const float* matrixData)
{
	Matrix rawMatrix = *reinterpret_cast<const Matrix*>(matrixData);
	this->m_localMatrix = rawMatrix.Transpose();
	this->m_dirty = false;
}

Vector3 AGameObject::GetForwardVector() const
{
	DirectX::XMVECTOR q = DirectX::XMVectorSet(
		m_localRotation.x,
		m_localRotation.y,
		m_localRotation.z,
		m_localRotation.w
	);

	Matrix rotationMatrix = Matrix::CreateFromQuaternion(q);
	Vector3 forward = Vector3(rotationMatrix._31, rotationMatrix._32, rotationMatrix._33);
	forward.Normalize();
	return forward;
}

Vector3 AGameObject::GetRightVector() const
{
	DirectX::XMVECTOR q = DirectX::XMVectorSet(
		m_localRotation.x,
		m_localRotation.y,
		m_localRotation.z,
		m_localRotation.w
	);

	Matrix rotationMatrix = Matrix::CreateFromQuaternion(q);
	Vector3 right = Vector3(rotationMatrix._11, rotationMatrix._12, rotationMatrix._13);
	right.Normalize();
	return right;
}

Vector3 AGameObject::GetUpVector() const
{
	DirectX::XMVECTOR q = DirectX::XMVectorSet(
		m_localRotation.x,
		m_localRotation.y,
		m_localRotation.z,
		m_localRotation.w
	);

	Matrix rotationMatrix = Matrix::CreateFromQuaternion(q);
	Vector3 up = Vector3(rotationMatrix._21, rotationMatrix._22, rotationMatrix._23);
	up.Normalize();
	return up;
}

void AGameObject::AttachComponent(std::shared_ptr<AComponent> component)
{
	this->m_componentList.push_back(component);
	component->AttachOwner(shared_from_this());
}

void AGameObject::DetachComponent(std::shared_ptr<AComponent> component)
{
	auto it = std::find(m_componentList.begin(), m_componentList.end(), component);
	if (it == m_componentList.end()) return;

	if (component->GetType() == AComponent::ComponentType::Physics)
	{
		if (auto physicsComponent = std::dynamic_pointer_cast<PhysicsComponent>(component))
		{
			PhysicsSystem::GetInstance()->UnregisterComponent(physicsComponent);
		}
	}

	component->DetachOwner();
	m_componentList.erase(it);

	Debug::Log("Detached " + component->GetName());
}

AComponent* AGameObject::FindComponentByName(String name)
{
	for (const auto& comp : m_componentList)
	{
		if (comp->GetName() == name)
		{
			return comp.get();
		}
	}

	return nullptr;
}

AComponent* AGameObject::FindComponentOfType(AComponent::ComponentType type, String name)
{
	for (const auto& comp : m_componentList)
	{
		if (comp->GetType() == type && comp->GetName() == name)
		{
			return comp.get();
		}		
	}

	return nullptr;
}

AGameObject::ComponentList AGameObject::GetComponentsOfType(AComponent::ComponentType type)
{
	ComponentList foundList;

	for (const auto& comp : m_componentList)
	{
		if (comp->GetType() == type)
		{
			foundList.push_back(comp);
		}		
	}

	return foundList;
}

AGameObject::ComponentList AGameObject::GetComponentsOfTypeRecursive(AComponent::ComponentType type)
{
	ComponentList foundList;

	// Add from this object
	for (const auto& comp : m_componentList)
	{
		if (comp->GetType() == type)
			foundList.push_back(comp);
	}

	// Recurse if you have children
	/*for (const auto& child : m_children)
	{
		ComponentList childList = child->GetComponentsOfTypeRecursive(type);
		foundList.insert(foundList.end(), childList.begin(), childList.end());
	}*/

	return foundList;
}

AGameObject::ComponentList AGameObject::GetAllComponents()
{
	return this->m_componentList;
}
