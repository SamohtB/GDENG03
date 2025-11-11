#pragma once
#include "AUIScreen.h"
#include "HotkeyListener.hpp"

class AGameObject;

class Hierarchy : public AUIScreen, public HotkeyListener
{
public:
    Hierarchy();
    ~Hierarchy();

    // Inherited via AUIScreen
    void DrawUI() override;

    void CreateObjectPopup();

	void OnActionPressed(Hotkey::Action action) override;

private:
    void DrawGameObjectNode(std::shared_ptr<AGameObject> gameObject);
	void DrawGameObjectNodeRecursive(std::shared_ptr<AGameObject> gameObject);
	void ReparentOrPromote(std::shared_ptr<AGameObject> gameObject, std::shared_ptr<AGameObject> parent = nullptr);
};

