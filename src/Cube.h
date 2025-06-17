#pragma once
#include "AMeshObject.h"

class Cube : public AMeshObject
{
public:
	Cube(String name, Vector3 color = ColorPalette::White);
	~Cube() = default;

	// Inherited via AMeshObject
	void OnUpdate(float deltaTime) override;
	
	void SetRandomRotation();

private:
	float m_ticks = 0.0f;
};

