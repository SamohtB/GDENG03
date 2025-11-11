#include "pch.h"
#include "TransformComponent.h"
#include "ActionHistory.h"
#include "AGameObject.h"
#include "PhysicsComponent.h"
#include "Debug.h"
#include "IconsMaterialDesign.h"
#include "EngineGUIManager.h"

TransformComponent::TransformComponent(String name, std::weak_ptr<AGameObject> owner) 
	: AComponent(name, ComponentType::Transform, owner)
{
	this->m_localPosition = Vector3(0, 0, 0);
	this->m_localRotation = rp3d::Quaternion::identity();
	this->m_localScale = Vector3(1, 1, 1);
	this->m_localMatrix = this->m_worldMatrix = GetLocalMatrix();
	this->isLinked = false;
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

void TransformComponent::UpdateOwnerPhysicsShape()
{
	if (auto owner = m_owner.lock())
	{
		auto physicsComp = static_cast<PhysicsComponent*>(owner->FindComponentOfType(AComponent::ComponentType::Physics));
		if (physicsComp)
		{
			physicsComp->UpdateColliderShape();
		}
	}
}

void TransformComponent::UpdateOwnerPhysics()
{
	if (auto owner = m_owner.lock())
	{
		// Find the physics component on the owner object
		auto physicsComp = static_cast<PhysicsComponent*>(owner->FindComponentOfType(AComponent::ComponentType::Physics));
		if (physicsComp)
		{
			// If it exists, tell it to update its rigid body's transform
			physicsComp->UpdateTransformFromOwner();
		}
	}
}


void TransformComponent::SetScale(float x, float y, float z)
{
	this->m_localScale = Vector3(x, y, z);
	PropagateDirtyFlag();
	UpdateOwnerPhysics();
	UpdateOwnerPhysicsShape(); // [NEW] Notify physics of scale change
}

void TransformComponent::SetScale(Vector3 vector)
{
	this->m_localScale = vector;
	PropagateDirtyFlag();
	UpdateOwnerPhysics();
	UpdateOwnerPhysicsShape(); // [NEW] Notify physics of scale change
}

void TransformComponent::Scale(float scale)
{
	this->m_localScale = this->m_localScale * scale;
	PropagateDirtyFlag();
	UpdateOwnerPhysics();
	UpdateOwnerPhysicsShape(); // [NEW] Notify physics of scale change
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
		float maxWidth = ImGui::GetWindowWidth();
		float labelWidth = std::max(60.0f, maxWidth * 0.25f);

		Vector3 position = GetLocalPosition();
		Vector3 rotation = GetLocalRotation();
		Vector3 scale = GetLocalScale();

		if (ImGui::BeginTable("TransformTable", 3, ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_NoBordersInBody))
		{
			ImGui::TableSetupColumn("Label", ImGuiTableColumnFlags_WidthFixed, labelWidth);
			ImGui::TableSetupColumn("Button", ImGuiTableColumnFlags_WidthFixed, 28.0f); // reserve space for link button
			ImGui::TableSetupColumn("Fields", ImGuiTableColumnFlags_WidthStretch);

			ImGui::TableNextRow();
			ImGui::TableSetColumnIndex(0);
			ImGui::AlignTextToFramePadding();
			ImGui::Text("Position");
			ImGui::TableSetColumnIndex(1);
			ImGui::Dummy(ImVec2(28.0f, 0.0f)); // placeholder to keep alignment
			ImGui::TableSetColumnIndex(2);
			DrawVector3Field("Pos", position);

			ImGui::TableNextRow();
			ImGui::TableSetColumnIndex(0);
			ImGui::TextUnformatted("Rotation");
			ImGui::TableSetColumnIndex(1);
			ImGui::Dummy(ImVec2(28.0f, 0.0f));
			ImGui::TableSetColumnIndex(2);
			DrawVector3Field("Rot", rotation);

			ImGui::TableNextRow();
			ImGui::TableSetColumnIndex(0);
			ImGui::TextUnformatted("Scale");
			ImGui::TableSetColumnIndex(1);
			ImGui::PushID("ScaleLink");

			ImGui::PushFont(EngineGUIManager::GetInstance()->GetIconFont());

			/* Aligns text inside button to centered */
			float offset = (ImGui::GetFrameHeight() - 28.0f) * 0.5f;
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + offset);
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));
			if (ImGui::Button(isLinked ? ICON_MD_INSERT_LINK : ICON_MD_LINK, ImVec2(24.0f, 0)))
				isLinked = !isLinked;
			ImGui::PopStyleVar();

			ImGui::PopFont();
			ImGui::PopID();
			 
			ImGui::TableSetColumnIndex(2);
			DrawVector3Field("Scale", scale);

			ImGui::EndTable();
		}
	}
}

void TransformComponent::DrawVector3Field(const char* label, Vector3& value)
{
	ImGui::PushID(label);

	float totalAvail = ImGui::GetContentRegionAvail().x;
	float spacing = ImGui::GetStyle().ItemSpacing.x;

	float labelWidth = ImGui::CalcTextSize("X").x;
	labelWidth = std::max(labelWidth, ImGui::CalcTextSize("Y").x);
	labelWidth = std::max(labelWidth, ImGui::CalcTextSize("Z").x);

	labelWidth += ImGui::GetStyle().ItemInnerSpacing.x;

	float totalLabelSpace = (labelWidth + spacing) * 3.0f - spacing;

	// Divide remaining width equally among the three input fields
	float inputWidth = (totalAvail - totalLabelSpace) / 3.0f;
	if (inputWidth < 1.0f) inputWidth = 1.0f; // safety clamp

	auto axisInput = [&](const char* name, float& v)
		{
			ImGui::AlignTextToFramePadding();
			ImGui::Text(name);
			ImGui::SameLine();
		
			ImGui::PushItemWidth(inputWidth);

			std::string id = "##";
			id += label;
			id += name;

			if (ImGui::InputFloat(id.c_str(), &v, 0))
			{
				if (std::strcmp(label, "Pos") == 0)
				{
					if (!m_recordedPosition)
					{
						ActionHistory::GetInstance()->RecordAction(this->GetOwner());
						m_recordedPosition = true;
					}
					this->SetPosition(value);
				}

				else if (std::strcmp(label, "Rot") == 0)
				{
					if (!m_recordedRotation)
					{
						ActionHistory::GetInstance()->RecordAction(this->GetOwner());
						m_recordedRotation = true;
					}
					this->SetRotation(value);
				}

				else if (std::strcmp(label, "Scale") == 0)
				{
					if (!m_recordedScale)
					{
						ActionHistory::GetInstance()->RecordAction(this->GetOwner());
						m_recordedScale = true;
					}
					this->SetScale(value);
				}
			}

			ImGui::PopItemWidth();

			if (ImGui::IsItemDeactivatedAfterEdit())
			{
				if (std::strcmp(label, "Pos") == 0) m_recordedPosition = false;
				else if (std::strcmp(label, "Rot") == 0) m_recordedRotation = false;
				else if (std::strcmp(label, "Scale") == 0) m_recordedScale = false;
			}

			ImGui::SameLine();
		};

	axisInput("X", value.x);
	axisInput("Y", value.y);
	axisInput("Z", value.z);

	ImGui::PopID();
}
