#include "pch.h"
#include "PointLight.h"

PointLight::PointLight(String name, Vector3 color, float intensity, float range)
	: ALight(name, color, intensity, LightType::POINT_LIGHT), m_range(range)
{

}

float PointLight::GetRange() const
{
	return m_range;
}