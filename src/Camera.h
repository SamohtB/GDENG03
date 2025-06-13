#pragma once
#include "AGameObject.h"
#include "InputListener.h"
#include "Math.h"

class DeviceContext;

class Camera : public AGameObject, public InputListener
{
public:
	Camera(String name, UINT viewportWidth, UINT viewportHeight);
	~Camera() = default;

	void Update(float deltaTime) override;

	Matrix GetViewMatrix();
	Matrix GetProjectionMatrix();

	virtual void OnKeyDown(int key) override;
	virtual void OnKeyUp(int key) override;

	virtual void OnMouseMove(const Vector2& deltaMousePos) override;
	virtual void OnMouseWheel(const float& delta) override;

	virtual void OnRightMouseDown(const Vector2& mousePos) override;
	virtual void OnRightMouseUp(const Vector2& mousePos) override;

	void SetViewportSize(UINT width, UINT height);

private:
	void FlyCamMode();
	void ZoomMode();

	bool m_isPerspectiveView = true;
	float m_deltaTime = 0.0f;

	/* Perspective */
	float m_fov = 45.0f;
	float m_aspectRatio = 0.0f;
	float m_pNearZ = 0.01f;
	float m_pFarZ = 100.0f;

	/* Orthographic */
	UINT m_viewportWidth = 0;
	UINT m_viewportHeight = 0;
	float m_oNearZ = -4.0f;
	float m_oFarZ = 4.0f;

	float m_moveSpeed = 50.0f;
	Vector2 m_lastMousePosition;
	bool m_rightMousePressed = false;

	void Draw(DeviceContext* dvcContext) override;
};