#include "Camera.h"
#include "InputSystem.h"
#include "Debug.h"
#include <cmath>

Camera::Camera(String name, UINT width, UINT height) : AGameObject(name), m_viewportWidth(width), m_viewportHeight(height)
{
    this->SetActive(true);
    this->m_isPerspectiveView = true;
    this->m_lastMousePosition = Vector2(0.0f, 0.0f);
    this->m_aspectRatio = static_cast<float>(width) / static_cast<float>(height);
}

void Camera::Update(float deltaTime)
{
    this->m_deltaTime = deltaTime;

    switch (this->m_rightMousePressed)
    {
    case true: FlyCamMode(); break;
    case false: ZoomMode();  break;
    }
}

void Camera::FlyCamMode()
{
    Vector3 moveDirection = Vector3(0.0f, 0.0f, 0.0f);

    if (m_heldKeys.contains('W')) moveDirection += this->GetForwardVector();
    if (m_heldKeys.contains('S')) moveDirection -= this->GetForwardVector();
    if (m_heldKeys.contains('D')) moveDirection += this->GetRightVector();
    if (m_heldKeys.contains('A')) moveDirection -= this->GetRightVector();
    if (m_heldKeys.contains('E')) moveDirection += this->GetUpVector();
    if (m_heldKeys.contains('Q')) moveDirection -= this->GetUpVector();

    float speedMultiplier = m_heldKeys.contains(VK_LSHIFT) ? 4.0f : 1.0f;

    if (moveDirection != Vector3::Zero)
    {
        moveDirection.Normalize();
        this->Move(moveDirection * this->m_moveSpeed * speedMultiplier * m_deltaTime);
    }
}

void Camera::ZoomMode()
{
    if (m_scrollDelta == 0.0f) return;

    if (this->m_isPerspectiveView)
    {
        Vector3 zoomDirection = this->GetForwardVector();
        float zoomAmount = m_scrollDelta * m_moveSpeed * m_deltaTime;

        this->Move(zoomDirection * zoomAmount);
    }

    m_scrollDelta = 0.0f;
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
    if (!this->m_rightMousePressed) return;

    float sensitivity = 0.01f;
    float yawDelta = deltaMousePos.x * sensitivity;
    float pitchDelta = deltaMousePos.y * sensitivity;

    Vector3 rotation = this->GetLocalRotation();

    rotation.y += yawDelta;
    rotation.x += pitchDelta;
    rotation.x = Clamp(rotation.x, -89.9f, 89.9f);

	this->SetRotation(rotation);
}

void Camera::OnMouseWheel(const float& delta)
{
    this->m_scrollDelta = delta;
}

void Camera::OnRightMouseDown(const Vector2& mousePos)
{
	this->m_rightMousePressed = true;
}

void Camera::OnRightMouseUp(const Vector2& mousePos)
{
	this->m_rightMousePressed = false;
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