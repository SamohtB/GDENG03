#pragma once
#include "AUIScreen.h"
#include "SceneReader.h"

class LoadScene : public AUIScreen
{
public:
	LoadScene();
	~LoadScene() = default;

	void DrawUI() override;

private:
	std::unique_ptr<SceneReader> m_sceneReader;
};

