#pragma once
#include "AUIScreen.h"

class DebugWindow : public AUIScreen
{
public:
	DebugWindow();
	~DebugWindow() = default;

	void DrawUI() override;

	void ClearLog();
	void LogMessage(const String& message);

private:
	std::vector<String> m_logMessages;
	UINT m_logLimit;
};

