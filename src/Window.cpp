#include "Window.h"
#include "Helper.h"

HWND Window::m_hwnd = nullptr;

Window::Window(UINT width, UINT height, std::wstring name) : m_width(width), m_height(height)
{
	m_aspectRatio = static_cast<float>(width) / static_cast<float>(height);

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

	RECT rc{ 0,0, static_cast<LONG>(width), static_cast<LONG>(height) };
	AdjustWindowRect(&rc, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU, false);

	m_hwnd = CreateWindowEx(
		WS_EX_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX, //ExStyle
		MAKEINTATOM(windowClassId), //Window
		name.data(), //Window Name
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU, //Style
		CW_USEDEFAULT, CW_USEDEFAULT, // Window Position
		rc.right - rc.left, rc.bottom - rc.top, //Window Size
		NULL, NULL, NULL, NULL);

	if (!m_hwnd)
	{
		throw std::runtime_error("CreateWindowEx failed");
	}

	::ShowWindow(m_hwnd, SW_SHOW);
	::UpdateWindow(m_hwnd);

	this->m_isRun = true;
}

Window::~Window()
{
	::DestroyWindow(m_hwnd);
}

unsigned int Window::GetWidth() const
{
	return this->m_width;
}

unsigned int Window::GetHeight() const
{
	return this->m_height;
}

bool Window::IsRun()
{
	if (m_isRun) Broadcast();
	return m_isRun;
}

HWND Window::GetHWND()
{
	return m_hwnd;
}

LRESULT Window::WindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
		case WM_CREATE:
		{
			break;
		}

		case WM_PAINT:
		{
			Window* window = reinterpret_cast<Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

			if (window)
			{
				window->OnUpdate();
				window->OnRender();
			}

			break;
		}

		case WM_DESTROY:
		{
			Window* window = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
			if (window) window->OnDestroy();
			::PostQuitMessage(0);
			break;
		}
	}

	return ::DefWindowProc(hwnd, msg, wparam, lparam);
}

bool Window::Broadcast()
{
	if (!this->isInit)
	{
		SetWindowLongPtr(m_hwnd, GWLP_USERDATA, (LONG_PTR)this);
		this->OnCreate();
		this->isInit = true;
	}

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

	return true;
}
