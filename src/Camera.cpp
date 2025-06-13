#include "Camera.h"
#include "InputSystem.h"
#include "Debug.h"

Camera::Camera(String name, UINT width, UINT height) : AGameObject(name), m_viewportWidth(width), m_viewportHeight(height)
{
    this->SetActive(true);
    this->m_isPerspectiveView = true; // Default to perspective projection
    this->m_lastMousePosition = Vector2(0.0f, 0.0f);
    this->m_aspectRatio = static_cast<float>(width) / static_cast<float>(height);
}

void Camera::Update(float deltaTime)
{
    this->m_deltaTime = deltaTime;
    Vector3 position = this->m_local_position;
    Vector3 rotation = this->m_local_rotation;

    Matrix rotationMatrix = Matrix::CreateFromYawPitchRoll(rotation);

    Vector3 moveDirection = Vector3(0.0f, 0.0f, 0.0f);

    if (InputSystem::GetInstance()->IsKeyDown('W'))
    {
        moveDirection += Vector3::Forward;
    }
    if (InputSystem::GetInstance()->IsKeyDown('S'))
    {
        moveDirection -= Vector3::Forward;
    }
    if (InputSystem::GetInstance()->IsKeyDown('A'))
    {
        moveDirection -= Vector3::Right;
    }
    if (InputSystem::GetInstance()->IsKeyDown('D'))
    {
        moveDirection += Vector3::Right;
    }
    if (InputSystem::GetInstance()->IsKeyDown(VK_SPACE))
    {
        moveDirection += Vector3::Up;
    }
    if (InputSystem::GetInstance()->IsKeyDown(VK_LSHIFT))
    {
        moveDirection -= Vector3::Up;
    }

    if (moveDirection != Vector3::Zero)
    {
        moveDirection = XMVector3Normalize(moveDirection);
    }

    moveDirection *= this->m_moveSpeed * deltaTime;
    this->Move(moveDirection);
}

Matrix Camera::GetViewMatrix()
{
	return this->GetLocalMatrix().Invert();
}

Matrix Camera::GetProjectionMatrix()
{
	if (this->m_isPerspectiveView)
    {
        return DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(m_fov), m_aspectRatio, m_pNearZ, m_pFarZ);
    }

    return DirectX::XMMatrixOrthographicLH(static_cast<float>(this->m_viewportWidth), static_cast<float>(this->m_viewportHeight), m_oNearZ, m_oFarZ);
}

void Camera::OnKeyDown(int key)
{
    Debug::Log("Down: " + std::to_string(key));
}

void Camera::OnKeyUp(int key)
{
}

void Camera::OnMouseMove(const Vector2& deltaMousePos)
{
}

void Camera::OnLeftMouseDown(const Vector2& mousePos)
{
}

void Camera::OnLeftMouseUp(const Vector2& mousePos)
{
}

void Camera::OnRightMouseDown(const Vector2& mousePos)
{
}

void Camera::OnRightMouseUp(const Vector2& mousePos)
{
}

void Camera::SetViewportSize(UINT width, UINT height)
{
    this->m_viewportHeight = height;
    this->m_viewportWidth = width;
    this->m_aspectRatio = static_cast<float>(width) / static_cast<float>(height);
}

void Camera::Draw(DeviceContext* dvcContext)
{
}