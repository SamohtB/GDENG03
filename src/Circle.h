#pragma once
#include "AMeshObject.h"

class Circle : public AMeshObject
{
public:
	Circle(String name, Vector3 color = ColorPalette::White);
	~Circle() override = default;

	// Inherited via AMeshObject
	void OnUpdate(float deltaTime) override;

	void SetDirection(const Vector3& direction);
	Vector3 GetDirection();

private:
	static const int SEGMENT_COUNT = 32;
	const float RADIUS = 1.0f;
	float m_moveSpeed = 2.0f;
	Vector3 m_direction;

	float m_xEdge = 5.5225f;
	float m_yEdge = 4.142f;
};

