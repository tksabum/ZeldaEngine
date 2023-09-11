#include "pch.h"
#include "Image.h"

#include "SceneManager.h"

#include "GameObject.h"
#include "Transform.h"
#include "Camera.h"

#include "ResourceManager.h"

namespace ZeldaEngine
{
	void Image::PreInitialize_()
	{
		materialInfo = *ResourceManager::GetInstance().GetMaterial(L"UI");
	}

	void Image::SetTexture(const std::wstring& textureName)
	{
		materialInfo.texture[0] = ResourceManager::GetInstance().GetTexture(textureName);
	}

	void Image::Render(IGraphics* graphics)
	{
		ZeldaMath::Matrix4x4 worldTM = gameObject->GetTransform()->GetWorldMatrix();
		std::wstring meshPath = ResourceManager::GetInstance().GetMesh(L"UI");

		float cameraNear = SceneManager::GetInstance().GetMainCamera()->GetCameraNear();
		float cameraFar = SceneManager::GetInstance().GetMainCamera()->GetCameraFar();

		ZeldaMath::Matrix4x4 tmp = ZeldaMath::Matrix4x4::Translation(ZeldaMath::Vector3D(0, 0, cameraNear + ((10000 + layer) / 100000.0f) * (cameraFar - cameraNear))) * worldTM;

		graphics->DrawMesh(tmp, meshPath, materialInfo, materialInfo.materialName,0);
	}

	void Image::SetAlpha(float alpha)
	{
		materialInfo.color.alpha = alpha;
	}

	float Image::GetAlpha()
	{
		return materialInfo.color.alpha;
	}

	void Image::PreSerialize(json& jsonData) const
	{
		PRESERIALIZE_BASE(Image);
		PRESERIALIZE_VALUE(layer);
		PRESERIALIZE_SERVALUE(materialInfo);
	}

	void Image::PreDeserialize(const json& jsonData)
	{
		PREDESERIALIZE_BASE(Image);
		PREDESERIALIZE_VALUE(layer);
		PREDESERIALIZE_SERVALUE(materialInfo);
	}

	void Image::PostSerialize(json& jsonData) const
	{
		POSTSERIALIZE_BASE();

		POSTSERIALIZE_SERVALUE(materialInfo);
	}

	void Image::PostDeserialize(const json& jsonData)
	{
		POSTDESERIALIZE_BASE();

		POSTDESERIALIZE_SERVALUE(materialInfo);
	}
}