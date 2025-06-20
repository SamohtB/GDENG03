#include "Circle.h"

Circle::Circle(String name, Vector3 color) : AMeshObject(name, color)
{
	std::vector<POS_COL> vertices;
	std::vector<unsigned int> indices;

	// Center vertex
	vertices.push_back(POS_COL(Vector3(0.0f, 0.0f, 0.0f), color));

    // Circle edge vertices
    for (int i = 0; i <= SEGMENT_COUNT; ++i)
    {
        float angle = (float)i / SEGMENT_COUNT * (M_PI * 2.0f);
        float x = RADIUS * std::cos(angle);
        float y = RADIUS * std::sin(angle);
        vertices.push_back(POS_COL(Vector3(x, y, 0.0f), color));
    }

    for (int i = 1; i <= SEGMENT_COUNT; ++i)
    {
        indices.push_back(0); 
        indices.push_back(i + 1);
        indices.push_back(i);         
    }

    this->SetGeometry(vertices, indices);


}

void Circle::OnUpdate(float deltaTime)
{
    Vector3 nextPos = this->GetLocalPosition() + m_direction * m_moveSpeed * deltaTime;
    float scaledRadius = RADIUS * this->GetLocalScale().x;

    // Check X bounds
    if (nextPos.x - scaledRadius < -m_xEdge)
    {
        m_direction.x *= -1.0f;
        nextPos.x = -m_xEdge + scaledRadius;
    }
    else if (nextPos.x + scaledRadius > m_xEdge)
    {
        m_direction.x *= -1.0f;
        nextPos.x = m_xEdge - scaledRadius;
    }

    // Check Y bounds
    if (nextPos.y - scaledRadius < -m_yEdge)
    {
        m_direction.y *= -1.0f;
        nextPos.y = -m_yEdge + scaledRadius;
    }
    else if (nextPos.y + scaledRadius > m_yEdge)
    {
        m_direction.y *= -1.0f;
        nextPos.y = m_yEdge - scaledRadius;
    }

    SetPosition(nextPos);

}

void Circle::SetDirection(const Vector3& direction)
{
	this->m_direction = direction;
}

Vector3 Circle::GetDirection()
{
	return this->m_direction;
}
