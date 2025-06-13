#pragma once
#include <directxtk12/SimpleMath.h>

using namespace DirectX::SimpleMath;

class InputListener
{
public:
	InputListener()
	{

	}

	~InputListener()
	{

	}

	virtual void OnKeyDown(int key) = 0;
	virtual void OnKeyUp(int key) = 0;

	virtual void OnMouseMove(const Vector2& deltaMousePos) = 0;

	virtual void OnLeftMouseDown(const Vector2& mousePos) = 0;
	virtual void OnLeftMouseUp(const Vector2& mousePos) = 0;

	virtual void OnRightMouseDown(const Vector2& mousePos) = 0;
	virtual void OnRightMouseUp(const Vector2& mousePos) = 0;
};