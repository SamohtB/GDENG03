#pragma once
#include "AMeshObject.h"

class Cylinder : public AMeshObject
{
public:
	Cylinder(String name);
	~Cylinder() = default;

	void Update(float deltaTime) override;
};