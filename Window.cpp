#include <stdexcept>
#include "Window.h"

Window* m_window = NULL;

static LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
		case WM_CREATE:
		{
			Window* window = (Window*)((LPCREATESTRUCT)lparam)->lpCreateParams;
			SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)window);
			window->start();
			break;
		}

		case WM_DESTROY:
		{
			Window* window = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
			window->stop();
			::PostQuitMessage(0);
			break;
		}

		default:
		{
			return ::DefWindowProc(hwnd, msg, wparam, lparam);
		}
	}

	return NULL;
}

Window::Window()
{
	WNDCLASSEX wc{};
	wc.cbClsExtra = NULL;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.cbWndExtra = NULL;
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.hCursor = LoadCursor(NULL, IDI_APPLICATION);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wc.hInstance = NULL;
	wc.lpszClassName = L"MyWindowClass";
	wc.lpszMenuName = L"";
	wc.style = NULL;
	wc.lpfnWndProc = &WindowProcedure;

	static const auto windowClassId = RegisterClassEx(&wc);

	if (!windowClassId)
	{
		throw std::runtime_error("RegisterClassEx failed");
	}

	RECT rc{ 0,0, 1024, 768 };
	AdjustWindowRect(&rc, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU, false);

	m_hwnd = CreateWindowEx(
		WS_EX_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX, //ExStyle
		MAKEINTATOM(windowClassId), //Window
		L"DirectX Application", //Window Name
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU, //Style
		CW_USEDEFAULT, CW_USEDEFAULT, // Window Position
		rc.right - rc.left, rc.bottom - rc.top, //Window Size
		NULL, NULL, NULL, this);

	if (!m_hwnd)
	{
		throw std::runtime_error("CreateWindowEx failed");
	}

	::ShowWindow(m_hwnd, SW_SHOW);
	::UpdateWindow(m_hwnd);
}

Window::~Window()
{
	if (m_hwnd)
	{
		::DestroyWindow(m_hwnd);
	}
}

void Window::start()
{
	this->m_isRun = true;
}

void Window::stop()
{
	this->m_isRun = false;
}

bool Window::broadcast()
{
	MSG msg;

	while (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	this->onUpdate();

	Sleep(1);

	return true;
}

bool Window::isRun()
{
	if (this->m_isRun) broadcast();
	return this->m_isRun;
}
