#include "ABaseRenderer.h"
#include "App.h"

ABaseRenderer::ABaseRenderer(UINT width, UINT height, std::wstring name) : m_width(width), m_height(height), m_title(name)
{
	m_aspectRatio = static_cast<float>(width) / static_cast<float>(height);
}

ABaseRenderer::~ABaseRenderer()
{
}

unsigned int ABaseRenderer::GetWidth() const
{
	return this->m_width;
}

unsigned int ABaseRenderer::GetHeight() const
{
	return this->m_height;
}

void ABaseRenderer::SetCustomWindowText(LPCWSTR text)
{
	std::wstring windowText = m_title + L": " + text;
	SetWindowText(App::GetHWND(), windowText.c_str());
}
