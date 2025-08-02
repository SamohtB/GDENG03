#pragma once
class GameEntity;
class AGameObject;

class GameObjectBuilder
{
public:
	GameObjectBuilder();
	~GameObjectBuilder() = default;

	GameObjectBuilder& SetName(const String& name);
	GameObjectBuilder& AddTransformComponent(String name);
	GameObjectBuilder& AddMeshComponent(String mesh, String material);
	GameObjectBuilder& AddPhysicsComponent(String meshType, bool isStatic);

	std::shared_ptr<AGameObject> Build();

private:
	std::shared_ptr<GameEntity> m_entity;
};

