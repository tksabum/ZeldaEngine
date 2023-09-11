#include "pch.h"
#include "SpotLight.h"

#include "ResourceManager.h"
#include "GraphicsSystem.h"

namespace ZeldaEngine
{
	SpotLight::SpotLight()
	{
		lightInfo.lightType = static_cast<__int32>(LIGHT_TYPE::SPOT_LIGHT);

		ZeldaMath::Vector2D resolution = GraphicsSystem::GetInstance().GetWindowSize();
		meshName = ResourceManager::GetInstance().GetMesh(L"Sphere");
		ShaderInfo info;
		info.shaderType = SHADER_TYPE::LIGHTING;
		info.rasterizerType = RASTERIZER_TYPE::CULL_NONE;
		info.depthStencilType = DEPTH_STENCIL_TYPE::NO_DEPTH_TEST_NO_WRITE;
		info.blendType = BLEND_TYPE::ONE_TO_ONE_BLEND;
		info.vs = "VS_PointLight";
		info.ps = "PS_PointLight";
		materialInfo.shader = L"LightingSpot";
		materialInfo.texture[0] = ResourceManager::GetInstance().GetTexture(L"D_Position");
		materialInfo.texture[1] = ResourceManager::GetInstance().GetTexture(L"D_Normal");
		materialInfo.materialName = L"PointLight";
		materialInfo.vec2Params[0] = resolution;
	}

	SpotLight::~SpotLight()
	{

	}

	void ZeldaEngine::SpotLight::Render(IGraphics* graphics)
	{
		graphics->DrawMesh(ZeldaMath::Matrix4x4::identity, meshName, materialInfo, materialInfo.materialName,0);
	}

	void ZeldaEngine::SpotLight::PreSerialize(json& jsonData) const
	{
		PRESERIALIZE_BASE(SpotLight);
	}

	void ZeldaEngine::SpotLight::PreDeserialize(const json& jsonData)
	{
		PREDESERIALIZE_BASE(SpotLight);
	}

	void ZeldaEngine::SpotLight::PostSerialize(json& jsonData) const
	{
		POSTSERIALIZE_BASE();
	}

	void ZeldaEngine::SpotLight::PostDeserialize(const json& jsonData)
	{
		POSTDESERIALIZE_BASE();
	}
}