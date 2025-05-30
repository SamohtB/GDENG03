#pragma once
#include "ABaseWindow.h"
#include <chrono>

class GameWindow : public ABaseWindow
{
public:
	GameWindow(UINT width, UINT height);
	~GameWindow() = default;

	// Inherited via ABaseWindow
	void OnCreate(HWND hwnd) override;
	void OnUpdate() override;
	void OnRender() override;
	void OnDestroy() override;

private:
	unsigned long m_oldTime = 0;
	float m_deltaTime = 0;
	float m_angle = 0;
};

