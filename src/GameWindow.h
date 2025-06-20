#pragma once
#include "ABaseWindow.h"

class GameWindow : public ABaseWindow
{
public:
	GameWindow(UINT width, UINT height);
	~GameWindow() = default;

	// Inherited via ABaseWindow
	void OnCreate(HWND hwnd) override;
	void Load_6();
	void Load_4();
	void Load_7();
	void OnUpdate() override;
	void OnRender() override;
	void OnDestroy() override;

private:
	float m_ticks = 0.0f;
};

