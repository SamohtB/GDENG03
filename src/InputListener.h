#pragma once
#include "pch.h"
#include "Math.h"

class InputListener
{
public:
	InputListener()
	{

	}

	~InputListener()
	{

	}

	virtual void OnKeyDown(int key) {}
	virtual void OnKeyUp(int key) {}

	virtual void OnMouseMove(const Vector2& deltaMousePos) {}
	virtual void OnMouseWheel(const float& delta) {}

	virtual void OnLeftMouseDown(const Vector2& mousePos) {}
	virtual void OnLeftMouseUp(const Vector2& mousePos) {}

	virtual void OnRightMouseDown(const Vector2& mousePos) {}
	virtual void OnRightMouseUp(const Vector2& mousePos) {}

protected:
	std::unordered_set<int> m_heldKeys;
	float m_scrollDelta = 0.0f;
};