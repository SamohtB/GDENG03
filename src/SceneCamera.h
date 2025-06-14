#pragma once
#include "Camera.h"

class SceneCamera :  public Camera, public InputListener
{
public:
	SceneCamera(UINT viewportWidth, UINT viewportHeight);
	~SceneCamera() = default;

	void Update(float deltaTime) override final;

	virtual void OnKeyDown(int key) override;
	virtual void OnKeyUp(int key) override;

	virtual void OnMouseMove(const Vector2& deltaMousePos) override;
	virtual void OnMouseWheel(const float& delta) override;

	virtual void OnRightMouseDown(const Vector2& mousePos) override;
	virtual void OnRightMouseUp(const Vector2& mousePos) override;

private:
	void FlyCamMode();
	void ZoomMode();

	float m_moveSpeed = 50.0f;
	Vector2 m_lastMousePosition;
	bool m_rightMousePressed = false;
};

