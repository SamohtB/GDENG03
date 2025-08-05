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
    void DrawGameObjectNode(AGameObject* gameObject);
	void DrawGameObjectNodeRecursive(AGameObject* gameObject);
	void ReparentOrPromote(std::shared_ptr<AGameObject> gameObject, AGameObject* parent = nullptr);
};

