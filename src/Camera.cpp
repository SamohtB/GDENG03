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

    Vector3 moveDirection = Vector3(0.0f, 0.0f, 0.0f);
    
    if (m_heldKeys.contains('W'))       moveDirection -= Vector3::Forward;
    if (m_heldKeys.contains('S'))       moveDirection += Vector3::Forward;
    if (m_heldKeys.contains('A'))       moveDirection -= Vector3::Right;
    if (m_heldKeys.contains('D'))       moveDirection += Vector3::Right;

    if (m_scrollDelta != 0.0f)
    {
        moveDirection -= Vector3::Up * m_scrollDelta * 0.1f;
        m_scrollDelta = 0.0f;
    }

    if (moveDirection != Vector3::Zero)
        moveDirection.Normalize(); 

    this->Move(moveDirection * this->m_moveSpeed * deltaTime);
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
    this->m_heldKeys.insert(key);
}

void Camera::OnKeyUp(int key)
{
    this->m_heldKeys.erase(key);
}

void Camera::OnMouseMove(const Vector2& deltaMousePos)
{
}

void Camera::OnMouseWheel(const float& delta)
{
    this->m_scrollDelta = delta;
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