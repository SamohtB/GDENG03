#include "pch.h"
#include "TransformComponent.h"

TransformComponent::TransformComponent(String name, std::weak_ptr<AGameObject> owner) 
	: AComponent(name, ComponentType::Transform, owner)
{
	this->m_localPosition = Vector3(0, 0, 0);
	this->m_localRotation = rp3d::Quaternion::identity();
	this->m_localScale = Vector3(1, 1, 1);
	this->m_localMatrix = GetLocalMatrix();
}

void TransformComponent::SetPosition(float x, float y, float z)
{
	this->m_localPosition = Vector3(x, y, z);
	this->m_dirty = true;
}

void TransformComponent::SetPosition(Vector3 vector)
{
	this->m_localPosition = vector;
	this->m_dirty = true;
}

void TransformComponent::Move(float x, float y, float z)
{
	this->m_localPosition += Vector3(x, y, z);
	this->m_dirty = true;
}

void TransformComponent::Move(Vector3 vector)
{
	this->m_localPosition += vector;
	this->m_dirty = true;
}

Vector3 TransformComponent::GetLocalPosition()
{
	return this->m_localPosition;
}

void TransformComponent::SetRotation(float pitch, float yaw, float roll)
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

void TransformComponent::SetRotation(Vector3 vector)
{
	SetRotation(vector.x, vector.y, vector.z);
}

void TransformComponent::SetRotation(rp3d::Quaternion quaternion)
{
	this->m_localRotation = quaternion;
	this->m_dirty = true;
}

void TransformComponent::Rotate(float pitch, float yaw, float roll)
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

Vector3 TransformComponent::GetLocalRotation()
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

rp3d::Quaternion TransformComponent::GetLocalQuaternion() const
{
	return this->m_localRotation;
}

void TransformComponent::SetScale(float x, float y, float z)
{
	this->m_localScale = Vector3(x, y, z);
	this->m_dirty = true;
}

void TransformComponent::SetScale(Vector3 vector)
{
	this->m_localScale = vector;
	this->m_dirty = true;
}

void TransformComponent::Scale(float scale)
{
	this->m_localScale += Vector3(scale, scale, scale);
	this->m_dirty = true;
}

Vector3 TransformComponent::GetLocalScale()
{
	return this->m_localScale;
}

Matrix TransformComponent::GetLocalMatrix()
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

Matrix TransformComponent::GetPhysicsLocalMatrix()
{
	Matrix localMatrix = this->GetLocalMatrix();
	return localMatrix.Transpose();
}

void TransformComponent::SetLocalMatrix(const float* matrixData)
{
	Matrix rawMatrix = *reinterpret_cast<const Matrix*>(matrixData);
	this->m_localMatrix = rawMatrix.Transpose();
	this->m_dirty = false;
}

Vector3 TransformComponent::GetForwardVector() const
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

Vector3 TransformComponent::GetRightVector() const
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

Vector3 TransformComponent::GetUpVector() const
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

void TransformComponent::Perform()
{
}

void TransformComponent::DrawUI()
{
	if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
	{
		Vector3 position = this->GetLocalPosition();
		Vector3 rotation = this->GetLocalRotation();
		Vector3 scale = this->GetLocalScale();

		if (ImGui::DragFloat3("Position", &position.x, 0.1f)) { this->SetPosition(position); }
		if (ImGui::DragFloat3("Rotation", &rotation.x, 0.1f)) { this->SetRotation(rotation); }
		if (ImGui::DragFloat3("Scale", &scale.x, 0.1f)) { this->SetScale(scale); }
	}
}
