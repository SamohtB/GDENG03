#include "pch.h"
#include "PointLight.h"

PointLight::PointLight(String name, Vector3 color, float intensity, float range)
	: ALight(name, color, intensity, LightType::POINT_LIGHT), m_range(range)
{

}

std::shared_ptr<PointLight> PointLight::Create(String name, Vector3 color, float intensity, float range)
{
	auto light = std::make_shared<PointLight>(name, color, intensity, range);
	light->Initialize();
	return light;
}

void PointLight::Initialize()
{
	auto transform = std::make_shared<TransformComponent>("Point Light Transform", shared_from_this());
	this->AttachComponent(transform);
	this->m_transform = transform;
}

float PointLight::GetRange() const
{
	return m_range;
}