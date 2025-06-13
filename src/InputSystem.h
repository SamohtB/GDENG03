#pragma once
#include "pch.h"
#include "Math.h"

class InputListener;

class InputSystem
{
public:
	using InputListenerPtr = std::shared_ptr<InputListener>;

	static InputSystem* GetInstance();
	static void Initialize();
	static void Destroy();

	void ProcessInput();
	void AddListener(InputListenerPtr listener);
	void RemoveListener(InputListenerPtr listener);

	bool IsKeyDown(int key);
	bool IsKeyUp(int key);

	void SetCursorPosition(const Vector2& point);
	void ShowCursor(bool show);

	void GetMouseScrollDelta(float delta);

private:
	InputSystem();
	~InputSystem() = default;
	InputSystem(InputSystem const&) {}
	InputSystem& operator=(InputSystem const&) {}

	static InputSystem* sharedInstance;

	std::unordered_set<InputListenerPtr> m_listenersMap;
	unsigned char m_keysState[256] = {};
	unsigned char m_oldKeysState[256] = {};
	Vector2 m_oldMousePosition;
	bool m_firstTime = true;
	float m_mouseWheelDelta = 0.0f;

	Vector2 GetMousePosition();
};