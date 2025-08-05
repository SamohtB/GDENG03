#pragma once
#include "ALight.h"

class PointLight : public ALight
{
public:
	PointLight(String name, Vector3 color, float intensity, float range);
	~PointLight() = default;

	static std::shared_ptr<PointLight> Create(String name, Vector3 color, float intensity, float range);
	void Initialize();

	float GetRange() const;

private:
	float m_range;

};