#include "pch.h"
#include "SceneCamera.h"
#include "Debug.h"

SceneCamera::SceneCamera(UINT viewportWidth, UINT viewportHeight) : Camera("Scene Camera", viewportWidth, viewportHeight)
{
	this->SetActive(true);
	this->m_isPerspectiveView = true;
	this->m_lastMousePosition = Vector2(0.0f, 0.0f);
}

void SceneCamera::Update(float deltaTime)
{
	Camera::Update(deltaTime);

    switch (this->m_rightMousePressed)
    {
    case true:
    {
        FlyCamMode(deltaTime);
        MouseMovement(deltaTime);
    } 
    break;

    case false: ZoomMode(deltaTime);  break;
    }

    
}

void SceneCamera::OnKeyPressed(int key)
{
    this->m_heldKeys.insert(key);
}

void SceneCamera::OnKeyReleased(int key)
{
    this->m_heldKeys.erase(key);
}

void SceneCamera::OnMouseMove(const Vector2& mousePos)
{
    Vector2 delta = mousePos - this->m_lastMousePosition;
    this->m_mouseDelta = delta;
    this->m_lastMousePosition = mousePos;
}

void SceneCamera::OnMouseWheel(const float& delta)
{
    this->m_scrollDelta = delta;
}

void SceneCamera::OnRightMousePressed(const Vector2& mousePos)
{
    this->m_rightMousePressed = true;
    this->m_mouseDelta = Vector2(0.0f, 0.0f);
    this->m_lastMousePosition = mousePos;
}

void SceneCamera::OnRightMouseReleased(const Vector2& mousePos)
{
    this->m_rightMousePressed = false;
}

void SceneCamera::FlyCamMode(float deltaTime)
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
        this->Move(moveDirection * this->m_cameraMoveSpeed * speedMultiplier * deltaTime);
    }
}

void SceneCamera::ZoomMode(float deltaTime)
{
    if (m_scrollDelta == 0.0f) return;

    if (this->m_isPerspectiveView)
    {
        Vector3 zoomDirection = this->GetForwardVector();
        float zoomAmount = m_scrollDelta * m_cameraMoveSpeed * deltaTime;

        this->Move(zoomDirection * zoomAmount);
    }

    m_scrollDelta = 0.0f;
}

void SceneCamera::MouseMovement(float deltaTime)
{
    constexpr float DEG2RAD = 3.14159265359f / 180.0f;

    float yawDelta = m_mouseDelta.x * m_mouseSensitivity * deltaTime;
    float pitchDelta = m_mouseDelta.y * m_mouseSensitivity * deltaTime;

    m_pitch += pitchDelta;
    m_yaw += yawDelta;


    m_pitch = Clamp(m_pitch, -89.9f, 89.9f);

    rp3d::Quaternion pitchQuat = rp3d::Quaternion::fromEulerAngles(m_pitch * DEG2RAD, 0.0f, 0.0f);
    rp3d::Quaternion yawQuat = rp3d::Quaternion::fromEulerAngles(0.0f, m_yaw * DEG2RAD, 0.0f);

    rp3d::Quaternion finalRotation = yawQuat * pitchQuat;

    SetRotation(finalRotation);

    m_mouseDelta = Vector2(0.0f, 0.0f);
}