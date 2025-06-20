#pragma once
#include <deque>
#include "AGameObject.h"
#include "InputListener.h"

class Circle;

class CircleManager : public AGameObject, public InputListener
{
public:
	using String = std::string;
	CircleManager();
	~CircleManager() = default;

	void Update(float deltaTime) override;

	void OnKeyPressed(int key) override;

private:
	void SpawnCircle();
	void ActivateCircle(const std::shared_ptr<Circle>& circle);
	void DeleteLastestCircle();
	void DeleteAllCircles();
	void CloseApplication();

	void Draw(DeviceContext* dvcContext) override;

	std::vector<std::shared_ptr<Circle>> m_circlePool;
	std::deque<std::shared_ptr<Circle>> m_activeCircles;

	static const int MAX_CIRCLES = 100;
};

