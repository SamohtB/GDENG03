#pragma once
#include "AMeshObject.h"

class Plane : public AMeshObject
{
public:
	Plane(String name, Vector3 color = colors::white);
	~Plane() = default;

	// Inherited via AMeshObject
	void OnUpdate(float deltaTime) override;

private:
	float m_ticks = 0.0f;
};

