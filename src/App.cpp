#include "App.h"
#include "Helper.h"

HWND App::m_hwnd = nullptr;

App::App(UINT width, UINT height) : RenderSystem(width, height)
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

	RECT rc{ 0,0, static_cast<LONG>(width), static_cast<LONG>(height) };
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

	this->OnInit();

	::ShowWindow(m_hwnd, SW_SHOW);
	::UpdateWindow(m_hwnd);
}

App::~App()
{
	if (m_hwnd)
	{
		::DestroyWindow(m_hwnd);
	}
}

void App::Broadcast()
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

	this->OnDestroy();
}

bool App::IsRun()
{
    if (this->m_isRun) Broadcast();
    return this->m_isRun;
}

HWND App::GetHWND()
{
    return m_hwnd;
}

LRESULT App::WindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	RenderSystem* window = reinterpret_cast<RenderSystem*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

	switch (msg)
	{
		case WM_CREATE:
		{
			LPCREATESTRUCT pCreateStruct = reinterpret_cast<LPCREATESTRUCT>(lparam);
			SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pCreateStruct->lpCreateParams));
			break;
		}

		case WM_PAINT:
		{
			if (window)
			{
				window->OnUpdate();
				window->OnRender();
			}
			return 0;
		} 
			
		case WM_DESTROY:
		{
			::PostQuitMessage(0);
			return 0;
		}
	}

	return ::DefWindowProc(hwnd, msg, wparam, lparam);
}
