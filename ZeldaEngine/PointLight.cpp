#include "pch.h"
#include "PointLight.h"

#include "ResourceManager.h"
#include "GraphicsSystem.h"

#include "GameObject.h"
#include "Transform.h"

namespace ZeldaEngine
{
	PointLight::PointLight()
	{
		lightInfo.lightType = static_cast<__int32>(LIGHT_TYPE::POINT_LIGHT);

		ZeldaMath::Vector2D resolution = GraphicsSystem::GetInstance().GetWindowSize();
		meshName = ResourceManager::GetInstance().GetMesh(L"Sphere");
		ShaderInfo info;
		info.shaderType = SHADER_TYPE::LIGHTING;
		info.rasterizerType = RASTERIZER_TYPE::CULL_NONE;
		info.depthStencilType = DEPTH_STENCIL_TYPE::NO_DEPTH_TEST_NO_WRITE;
		info.blendType = BLEND_TYPE::ONE_TO_ONE_BLEND;
		info.vs = "VS_PointLight";
		info.ps = "PS_PointLight";
		materialInfo.shader = L"LightingPoint";
		materialInfo.texture[0] = ResourceManager::GetInstance().GetTexture(L"D_Position");
		materialInfo.texture[1] = ResourceManager::GetInstance().GetTexture(L"D_Normal");
		materialInfo.materialName = L"PointLight";
		materialInfo.vec2Params[0] = resolution;
	}

	PointLight::~PointLight()
	{

	}

	void PointLight::Render(IGraphics* graphics)
	{
		Transform* trs = GetGameObject()->GetTransform();
		ZeldaMath::Matrix4x4 _T = ZeldaMath::Matrix4x4::Translation(trs->GetLocalPosition()) * trs->GetLocalRotationAsQuaternion().GetTransformMatrix();
		ZeldaMath::Matrix4x4 _R = trs->GetLocalRotationAsQuaternion().GetTransformMatrix();
		ZeldaMath::Matrix4x4 _S = ZeldaMath::Matrix4x4::Scale(ZeldaMath::Vector3D(lightInfo.range * 2.0f, lightInfo.range * 2.0f, lightInfo.range * 2.0f));
		
		ZeldaMath::Matrix4x4 lightMatrix = _T * _R * _S * trs->GetParentWorldMatrix();

		graphics->DrawMesh(lightMatrix, meshName, materialInfo, materialInfo.materialName,0);
	}

	void PointLight::PreSerialize(json& jsonData) const
	{
		PRESERIALIZE_BASE(PointLight);
	}

	void PointLight::PreDeserialize(const json& jsonData)
	{
		PREDESERIALIZE_BASE(PointLight);
	}

	void PointLight::PostSerialize(json& jsonData) const
	{
		POSTSERIALIZE_BASE();
	}

	void PointLight::PostDeserialize(const json& jsonData)
	{
		POSTDESERIALIZE_BASE();
	}
}