#pragma once
#include <Windows.h>

class Window
{
public:
	Window();
	~Window();

	void start();
	void stop();

	bool broadcast();
	bool isRun();

	virtual void onUpdate() = 0;

private:
	HWND m_hwnd{};
	bool m_isRun = true;
};



