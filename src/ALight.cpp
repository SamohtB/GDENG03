#include "pch.h"
#include "ALight.h"

ALight::ALight(String name, Vector3 color, float intensity, String type)
	: AGameObject(name), m_color(color), m_intensity(intensity), m_type(type)
{
}

void ALight::Update(float deltaTime)
{
}

Vector3 ALight::GetColor() const
{
	return m_color;
}

float ALight::GetIntensity() const
{
	return m_intensity;
}