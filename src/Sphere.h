#pragma once
#include "AMeshObject.h"

class Sphere : public AMeshObject
{
public:
    Sphere(String name);
    virtual ~Sphere() = default;

    void Update(float deltaTime) override;
};
