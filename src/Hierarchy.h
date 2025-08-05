#pragma once
#include "AUIScreen.h"

class AGameObject;

class Hierarchy : public AUIScreen
{
public:
    Hierarchy();
    ~Hierarchy() = default;

    // Inherited via AUIScreen
    void DrawUI() override;

private:
    void DrawGameObjectNode(std::shared_ptr<AGameObject> gameObject);
	void DrawGameObjectNodeRecursive(std::shared_ptr<AGameObject> gameObject);
	void ReparentOrPromote(std::shared_ptr<AGameObject> gameObject, std::shared_ptr<AGameObject> parent = nullptr);
};

