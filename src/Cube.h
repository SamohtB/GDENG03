#pragma once
#include "AMeshObject.h"

class Cube : public AMeshObject
{
public:
	Cube(String name);
	~Cube() = default;

	void Update(float deltaTime) override;
};