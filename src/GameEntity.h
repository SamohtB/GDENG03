#pragma once
#include "AGameObject.h"
class GameEntity : public AGameObject
{
public:
    GameEntity(String name);
    ~GameEntity() = default;

    void Update(float deltaTime) override;
};

