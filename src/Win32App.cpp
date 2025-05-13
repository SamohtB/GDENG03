#include "Win32App.h"
#include "stdafx.h"

Win32App::Win32App(ABaseWindow* window, std::wstring windowName) : m_window(window), m_isRun(true)
{
	WNDCLASSEX wc{};
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.lpfnWndProc = &WindowProc;
	wc.hInstance = GetModuleHandle(nullptr);
	wc.lpszClassName = L"MyWindowClass";

	if (!RegisterClassEx(&wc)) 
	{
		throw std::runtime_error("Failed to register window class.");
	}

	const UINT width = window->GetWidth();
	const UINT height = window->GetHeight();

	RECT rc = { 0, 0, static_cast<LONG>(width), static_cast<LONG>(height) };

	if (!AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE))
	{
		throw std::runtime_error("Window adjust rect failed.");
	}

	m_hwnd = CreateWindowEx(
		WS_EX_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX,
		L"MyWindowClass",		//Window
		windowName.c_str(),		//Window Name
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU, //Style
		CW_USEDEFAULT, CW_USEDEFAULT, // Window Position
		rc.right - rc.left, rc.bottom - rc.top, //Window Size
		NULL, NULL, wc.hInstance, this);

	if (!m_hwnd)
	{
		throw std::runtime_error("CreateWindowEx failed");
	}

	window->OnCreate(m_hwnd);

	::ShowWindow(m_hwnd, SW_SHOW);
}

HWND Win32App::GetHwnd()
{
	return m_hwnd;
}

bool Win32App::IsRun()
{
	return m_isRun;
}

LRESULT Win32App::WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	if (msg == WM_CREATE)
	{
		auto* createStruct = reinterpret_cast<LPCREATESTRUCT>(lparam);
		auto* app = reinterpret_cast<Win32App*>(createStruct->lpCreateParams);
		SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(app));
		return 0;
	}

	auto* app = reinterpret_cast<Win32App*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
	if (!app) return DefWindowProc(hwnd, msg, wparam, lparam);

	switch (msg)
	{
	case WM_PAINT:
		if (app->m_window)
		{
			app->m_window->OnUpdate();
			app->m_window->OnRender();
		}
		return 0;

	case WM_DESTROY:
		if (app->m_window)
		{
			app->m_window->OnDestroy();
		}
		app->m_isRun = false;
		::PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hwnd, msg, wparam, lparam);
}

void Win32App::Broadcast()
{
	MSG msg = {};

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		Sleep(1);
	}
}


