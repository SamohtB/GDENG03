#pragma once
#include "AGameObject.h"
#include "InputListener.h"

class CircleManager :  public AGameObject, public InputListener
{
public:
	using String = std::string;
	CircleManager();

	void Update(float deltaTime) override;

	virtual void OnKeyPressed(int key) override;
	virtual void OnKeyReleased(int key) override;

private:
	void SpawnCircle();
	void CloseApp();
	void DeleteCircles();

	std::unordered_set<int> m_heldKeys;

	// Inherited via AGameObject

	void Draw(DeviceContext* dvcContext) override;
};

