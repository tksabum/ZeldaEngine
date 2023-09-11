#include "pch.h"
#include "DirectionalLight.h"

#include "GameObject.h"
#include "Transform.h"

#include "IGraphics.h"

#include "ResourceManager.h"
#include "GraphicsSystem.h"

namespace ZeldaEngine
{
	using namespace ZeldaMath;

	DirectionalLight::DirectionalLight()
	{
		lightInfo.lightType = static_cast<__int32>(LIGHT_TYPE::DIRECTIONAL_LIGHT);

		meshName = ResourceManager::GetInstance().GetMesh(L"Rectangle");
		materialInfo = *ResourceManager::GetInstance().GetMaterial(L"DirLight");

		lightInfo.direction = ZeldaMath::Vector4D(1.f, 0.f, 1.f);

		lightInfo.color.diffuse = ZeldaMath::Vector4D(1.0f, 1.0f, 1.0f, 0.f);
		lightInfo.color.ambient = ZeldaMath::Vector4D(0.3f,0.3f,0.3f, 0.f);
		lightInfo.color.specular = ZeldaMath::Vector4D(0.f,0.f,0.f,0.f);
	}

	DirectionalLight::~DirectionalLight()
	{
		
	}

	void DirectionalLight::Render(IGraphics* graphics)
	{
		ZeldaMath::Matrix4x4 worldTM = gameObject->GetTransform()->GetWorldMatrix();
		std::wstring meshPath = ResourceManager::GetInstance().GetMesh(meshName);
		graphics->DrawMesh(worldTM, meshPath, materialInfo, materialInfo.materialName,0);
	}

	void DirectionalLight::UpdateLightInfo()
	{
		Vector3D frontVec = GetGameObject()->GetTransform()->GetFront();
		lightInfo.direction = Vector4D(frontVec.x, frontVec.y, frontVec.z, 0.0f);
	}

	void DirectionalLight::PreSerialize(json& jsonData) const
	{
		PRESERIALIZE_BASE(DirectionalLight);
	}

	void DirectionalLight::PreDeserialize(const json& jsonData)
	{
		PREDESERIALIZE_BASE(DirectionalLight);
	}

	void DirectionalLight::PostSerialize(json& jsonData) const
	{
		POSTSERIALIZE_BASE();
	}

	void DirectionalLight::PostDeserialize(const json& jsonData)
	{
		POSTDESERIALIZE_BASE();
	}
}