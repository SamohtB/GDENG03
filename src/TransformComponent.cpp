#include "pch.h"
#include "TransformComponent.h"
#include "ActionHistory.h"
#include "AGameObject.h"

TransformComponent::TransformComponent(String name, std::weak_ptr<AGameObject> owner) 
	: AComponent(name, ComponentType::Transform, owner)
{
	this->m_localPosition = Vector3(0, 0, 0);
	this->m_localRotation = rp3d::Quaternion::identity();
	this->m_localScale = Vector3(1, 1, 1);
	this->m_localMatrix = this->m_worldMatrix = GetLocalMatrix();
}

void TransformComponent::SetPosition(float x, float y, float z)
{
	this->m_localPosition = Vector3(x, y, z);
	PropagateDirtyFlag();
}

void TransformComponent::SetPosition(Vector3 vector)
{
	this->m_localPosition = vector;
	PropagateDirtyFlag();
}

void TransformComponent::Move(float x, float y, float z)
{
	this->m_localPosition += Vector3(x, y, z);
	PropagateDirtyFlag();
}

void TransformComponent::Move(Vector3 vector)
{
	this->m_localPosition += vector;
	PropagateDirtyFlag();
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

	PropagateDirtyFlag();
}

void TransformComponent::SetRotation(Vector3 vector)
{
	SetRotation(vector.x, vector.y, vector.z);
}

void TransformComponent::SetRotation(rp3d::Quaternion quaternion)
{
	this->m_localRotation = quaternion;
	PropagateDirtyFlag();
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

	PropagateDirtyFlag();
}

Vector3 TransformComponent::GetLocalRotation()
{
	using namespace DirectX;
	auto q = this->m_localRotation;
	
	SimpleMath::Quaternion dxQuaternion = SimpleMath::Quaternion(q.x, q.y, q.z, q.w);
	dxQuaternion.Normalize();

	Vector3 euler = dxQuaternion.ToEuler();

	return Vector3(
		XMConvertToDegrees(euler.x),
		XMConvertToDegrees(euler.y),
		XMConvertToDegrees(euler.z)
	);

}

rp3d::Quaternion TransformComponent::GetLocalQuaternion() const
{
	return this->m_localRotation;
}

void TransformComponent::SetScale(float x, float y, float z)
{
	this->m_localScale = Vector3(x, y, z);
	PropagateDirtyFlag();
}

void TransformComponent::SetScale(Vector3 vector)
{
	this->m_localScale = vector;
	PropagateDirtyFlag();
}

void TransformComponent::Scale(float scale)
{
	this->m_localScale += Vector3(scale, scale, scale);
	PropagateDirtyFlag();
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
	PropagateDirtyFlag();
}

Matrix TransformComponent::GetWorldMatrix()
{
	return this->m_worldMatrix;
}

void TransformComponent::UpdateWorldMatrix(Matrix parentMatrix)
{
	if (this->m_worldDirty) 
	{
		this->m_worldMatrix = parentMatrix * this->GetLocalMatrix();
		this->m_worldDirty = false;
	}

	auto children = this->GetOwner()->GetChildren();

	for (const auto& child : children)
	{
		child->Transform()->UpdateWorldMatrix(this->m_worldMatrix);
	}
	
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

void TransformComponent::PropagateDirtyFlag()
{
	this->m_dirty = true;
	this->m_worldDirty = true;

	auto children = this->GetOwner()->GetChildren();

	for (const auto& child : children)
	{
		child->Transform()->PropagateDirtyFlag();
	}
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

		if (ImGui::DragFloat3("Position", &position.x, 0.1f)) 
		{ 
			ActionHistory::GetInstance()->RecordAction(this->GetOwner());
			this->SetPosition(position);
		}

		if (ImGui::DragFloat3("Rotation", &rotation.x, 0.1f)) 
		{ 
			ActionHistory::GetInstance()->RecordAction(this->GetOwner());
			this->SetRotation(rotation);
		}

		if (ImGui::DragFloat3("Scale", &scale.x, 0.1f)) 
		{ 
			ActionHistory::GetInstance()->RecordAction(this->GetOwner());
			this->SetScale(scale); 
		}
	}
}
