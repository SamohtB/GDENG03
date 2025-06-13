#include "InputSystem.h"
#include "InputListener.h"
#include "Windows.h"

InputSystem* InputSystem::sharedInstance = nullptr;

InputSystem* InputSystem::GetInstance()
{
	return sharedInstance;
}

void InputSystem::Initialize()
{
	try
	{
		sharedInstance = new InputSystem();
	}
	catch (...)
	{
		throw std::exception("InputSystem not created successfully");
	}
}

void InputSystem::Destroy()
{
	delete sharedInstance;
}

InputSystem::InputSystem() {}

void InputSystem::ProcessInput()
{
	Vector2 current_mouse_pos = GetMousePosition();

	if (m_firstTime)
	{
		m_firstTime = false;
		m_oldMousePosition = current_mouse_pos;
	}

	Vector2 delta = current_mouse_pos - m_oldMousePosition;

	/* Mouse Movement */
	if (fabs(delta.x) > 0.001f || fabs(delta.y) > 0.001f)
	{	
		// Mouse Move Event
		for (const auto& listener : m_listenersMap)
		{
			listener->OnMouseMove(delta);
		}

		m_oldMousePosition = current_mouse_pos;
	}

	/* Mouse Scroll */
	if (m_mouseWheelDelta != 0.0f)
	{
		for (const auto& listener : m_listenersMap)
		{
			listener->OnMouseWheel(m_mouseWheelDelta);
		}
		m_mouseWheelDelta = 0.0f; // Reset after notifying listeners
	}

	/* Keyboard States */
	if (::GetKeyboardState(m_keysState))
	{
		for (unsigned int i = 0; i < 256; i++)
		{
			// Key Down
			if (m_keysState[i] & 0x80)
			{
				for (const auto& listener : m_listenersMap)
				{
					if (i == VK_LBUTTON)
					{
						if (m_keysState[i] != m_oldKeysState[i])
						{
							listener->OnLeftMouseDown(current_mouse_pos);
						}
					}
					else if (i == VK_RBUTTON)
					{
						if (m_keysState[i] != m_oldKeysState[i])
						{
							listener->OnRightMouseDown(current_mouse_pos);
						}
					}
					else
					{
						listener->OnKeyDown(i);
					}
				}
			}
			// Key Up
			else
			{
				if (m_keysState[i] != m_oldKeysState[i])
				{
					for (const auto& listener : m_listenersMap)
					{
						if (i == VK_LBUTTON)
						{
							listener->OnLeftMouseUp(current_mouse_pos);
						}
						else if (i == VK_RBUTTON)
						{
							listener->OnRightMouseUp(current_mouse_pos);
						}
						else
						{
							listener->OnKeyUp(i);
						}
					}
				}
			}
		}

		::memcpy(m_oldKeysState, m_keysState, sizeof(unsigned char) * 256);
	}
}

void InputSystem::AddListener(InputListenerPtr listener)
{
	m_listenersMap.insert(listener);
}

void InputSystem::RemoveListener(InputListenerPtr listener)
{
	m_listenersMap.erase(listener);
}

bool InputSystem::IsKeyDown(int key)
{
	return (m_keysState[key] & 0x80) != 0;
}

bool InputSystem::IsKeyUp(int key)
{
	return (m_keysState[key] & 0x80) == 0;
}

void InputSystem::SetCursorPosition(const Vector2& point)
{
	::SetCursorPos(static_cast<int>(point.x), static_cast<int>(point.y));
}

void InputSystem::ShowCursor(bool show)
{
	::ShowCursor(show);
}

void InputSystem::GetMouseScrollDelta(float delta)
{
	this->m_mouseWheelDelta += delta;
}

Vector2 InputSystem::GetMousePosition()
{
	POINT point;
	if (::GetCursorPos(&point))
	{
		return DirectX::XMVectorSet(static_cast<float>(point.x), static_cast<float>(point.y), 0.0f, 0.0f);
	}
	else
	{
		return DirectX::XMVectorZero();
	}
}

