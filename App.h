#pragma once
#include "Window.h"

class App : public Window
{
public:
	App(UINT width, UINT height);
	~App();

	void Broadcast();

	bool IsRun();
	static HWND GetHWND();

protected:
	static LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

private:
	static HWND m_hwnd;
	bool m_isRun = true;
};

