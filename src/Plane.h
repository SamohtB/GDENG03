#pragma once
#include "AMeshObject.h"

class Plane : public AMeshObject
{
public:
	Plane(String name);
	~Plane() = default;

	void Update(float deltaTime) override;
};