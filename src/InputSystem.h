#pragma once
#include <map>
#include <directxtk12/SimpleMath.h>

class InputListener;

class InputSystem
{
public:
	using Vector2 = DirectX::SimpleMath::Vector2;

	static InputSystem* GetInstance();
	static void Initialize();
	static void Destroy();

	void Update();
	void AddListener(InputListener* listener);
	void RemoveListener(InputListener* listener);

	bool IsKeyDown(int key);
	bool IsKeyUp(int key);

	void SetCursorPosition(const Vector2& point);
	void ShowCursor(bool show);

private:
	InputSystem();
	~InputSystem() = default;
	InputSystem(InputSystem const&) {}
	InputSystem& operator=(InputSystem const&) {}

	static InputSystem* sharedInstance;

	std::map<InputListener*, InputListener*> m_map_listeners;
	unsigned char m_keys_state[256] = {};
	unsigned char m_old_keys_state[256] = {};
	Vector2 old_mouse_position;
	bool first_time = true;

	Vector2 GetMousePosition();
};

