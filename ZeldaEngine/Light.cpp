#include "pch.h"
#include "Light.h"

#include "GraphicsSystem.h"

namespace ZeldaEngine
{
	Light::Light()
	{
		GraphicsSystem::GetInstance().AddLight(this);
	}

	Light::~Light()
	{
		GraphicsSystem::GetInstance().RemoveLight(this);
	}

	LightInfo Light::GetLightInfo()
	{
		UpdateLightInfo();
		return lightInfo;
	}

	void Light::SetLightIndex(__int8 idx)
	{
		materialInfo.intParams[0] = idx;
	}

	void Light::UpdateLightInfo()
	{

	}
}
