#pragma once
#include "AMeshObject.h"

class CustomMesh : public AMeshObject
{
public:
	CustomMesh(String name, String mesh);
	~CustomMesh() = default;

	void Update(float deltaTime) override;
};

