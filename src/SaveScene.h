#pragma once
#include "AUIScreen.h"
#include "SceneWriter.h"

class SaveScene : public AUIScreen
{
public:
	SaveScene();
	~SaveScene() = default;

	void DrawUI() override;

private:
	char m_sceneNameBuffer[128] = "NewScene";
	std::unique_ptr<SceneWriter> m_sceneWriter;
};

