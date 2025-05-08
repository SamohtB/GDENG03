#pragma once
#include "ABaseRenderer.h"

class Window : public ABaseRenderer
{
public:
	Window(UINT width, UINT height, std::wstring name);
	virtual ~Window();

	virtual void OnInit();
	virtual void OnUpdate();
	virtual void OnRender();
	virtual void OnDestroy();

private:
	
	/* Pipeline Objects */
	/* add swap chains, command queues, etc. here */

	void LoadPipeline();
	void LoadAssets();
	void PopulateCommandList();
	void WaitForPreviousFrame();
};



