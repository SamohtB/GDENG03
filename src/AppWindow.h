#pragma once
#include <memory>
#include "Window.h"
#include "stdafx.h"

class AppWindow : public Window
{
public:
	AppWindow(UINT width, UINT height);
	~AppWindow() = default;
	
	// Inherited via Window
	void OnCreate() override;
	void OnUpdate() override;
	void OnRender() override;
	void OnDestroy() override;

private:
};

