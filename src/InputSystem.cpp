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

	if (first_time)
	{
		first_time = false;
		old_mouse_position = current_mouse_pos;
	}

	if (current_mouse_pos != old_mouse_position)
	{
		/* Mouse Move Event */
		std::map<InputListenerPtr, InputListenerPtr>::iterator it = m_map_listeners.begin();

		while (it != m_map_listeners.end())
		{
			Vector2 current_point;
			XMStoreFloat2(&current_point, current_mouse_pos);

			it->second->OnMouseMove(current_point);
			it++;
		}

		old_mouse_position = current_mouse_pos;
	}

	if (::GetKeyboardState(m_keys_state))
	{
		for (unsigned int i = 0; i < 256; i++)
		{
			/* Key Down */
			if (m_keys_state[i] & 0x80)
			{
				std::map<InputListenerPtr, InputListenerPtr>::iterator it = m_map_listeners.begin();

				while (it != m_map_listeners.end())
				{

					if (i == VK_LBUTTON)
					{
						if (m_keys_state[i] != m_old_keys_state[i])
						{
							it->second->OnLeftMouseDown(current_mouse_pos);
						}
					}
					else if (i == VK_RBUTTON)
					{
						if (m_keys_state[i] != m_old_keys_state[i])
						{
							it->second->OnRightMouseDown(current_mouse_pos);
						}
					}
					else
					{
						it->second->OnKeyDown(i);
					}

					it++;
				}
			}
			/* Key Up */
			else
			{
				if (m_keys_state[i] != m_old_keys_state[i])
				{
					std::map<InputListenerPtr, InputListenerPtr>::iterator it = m_map_listeners.begin();

					while (it != m_map_listeners.end())
					{
						if (i == VK_LBUTTON)
						{
							it->second->OnLeftMouseUp(current_mouse_pos);
						}
						else if (i == VK_RBUTTON)
						{
							it->second->OnRightMouseUp(current_mouse_pos);
						}
						else
						{
							it->second->OnKeyUp(i);
						}

						it++;
					}
				}
			}
		}

		::memcpy(m_old_keys_state, m_keys_state, sizeof(unsigned char) * 256);
	}
}

void InputSystem::AddListener(InputListenerPtr listener)
{
	m_map_listeners.insert(std::make_pair<InputListenerPtr, InputListenerPtr>(std::forward<InputListenerPtr>(listener), std::forward<InputListenerPtr>(listener)));
}

void InputSystem::RemoveListener(InputListenerPtr listener)
{
	std::map<InputListenerPtr, InputListenerPtr>::iterator it = m_map_listeners.find(listener);

	if (it != m_map_listeners.end())
	{
		m_map_listeners.erase(it);
	}
}

bool InputSystem::IsKeyDown(int key)
{
	return (m_keys_state[key] & 0x80) != 0;
}

bool InputSystem::IsKeyUp(int key)
{
	return (m_keys_state[key] & 0x80) == 0;
}

void InputSystem::SetCursorPosition(const Vector2& point)
{
	::SetCursorPos(static_cast<int>(point.x), static_cast<int>(point.y));
}

void InputSystem::ShowCursor(bool show)
{
	::ShowCursor(show);
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

