#include "SceneCamera.h"

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
    case true: FlyCamMode(); break;
    case false: ZoomMode();  break;
    }
}

void SceneCamera::OnKeyDown(int key)
{
    this->m_heldKeys.insert(key);
}

void SceneCamera::OnKeyUp(int key)
{
    this->m_heldKeys.erase(key);
}

void SceneCamera::OnMouseMove(const Vector2& deltaMousePos)
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

void SceneCamera::OnMouseWheel(const float& delta)
{
    this->m_scrollDelta = delta;
}

void SceneCamera::OnRightMouseDown(const Vector2& mousePos)
{
    this->m_rightMousePressed = true;
}

void SceneCamera::OnRightMouseUp(const Vector2& mousePos)
{
    this->m_rightMousePressed = false;
}

void SceneCamera::FlyCamMode()
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

void SceneCamera::ZoomMode()
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