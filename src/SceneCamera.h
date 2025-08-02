#pragma once
#include "Camera.h"

class SceneCamera :  public Camera, public InputListener
{
public:
	SceneCamera(UINT viewportWidth, UINT viewportHeight);
	~SceneCamera() = default;

	static std::shared_ptr<SceneCamera> Create(UINT width, UINT height);
	void Initialize();

	void Update(float deltaTime) override final;

	virtual void OnKeyPressed(int key) override;
	virtual void OnKeyReleased(int key) override;

	virtual void OnMouseMove(const Vector2& mousePos) override;
	virtual void OnMouseWheel(const float& delta) override;

	virtual void OnRightMousePressed(const Vector2& mousePos) override;
	virtual void OnRightMouseReleased(const Vector2& mousePos) override;

private:
	void FlyCamMode(float deltaTime);
	void ZoomMode(float deltaTime);
	void MouseMovement(float deltaTime);

	float m_mouseSensitivity = 15.0f;
	float m_cameraMoveSpeed = 20.0f;
	float m_scrollDelta = 0.0f;

	bool m_rightMousePressed = false;

	float m_pitch = 0.0f;
	float m_yaw = 0.0f;

	Vector2 m_mouseDelta = Vector2(0.0f, 0.0f);
	Vector2 m_lastMousePosition = Vector2(0.0f, 0.0f);

	std::unordered_set<int> m_heldKeys;
};

