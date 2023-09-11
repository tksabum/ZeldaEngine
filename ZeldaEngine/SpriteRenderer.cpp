#include "pch.h"
#include "SpriteRenderer.h"

#include "ResourceManager.h"
#include "SceneManager.h"

#include "GameObject.h"
#include "Transform.h"
#include "Camera.h"

#include "ZeldaMath.h"

#include "AnimationSystem.h"

namespace ZeldaEngine
{
	using namespace ZeldaMath;

	void SpriteRenderer::PreInitialize_()
	{
		materialInfo = *ResourceManager::GetInstance().GetMaterial(L"Sprite");
	}

	void SpriteRenderer::SetTexture(const std::wstring& textureName)
	{
		materialInfo.texture[0] = ResourceManager::GetInstance().GetTexture(textureName);
	}

	void SpriteRenderer::Render(IGraphics* graphics)
	{
		std::wstring meshPath = ResourceManager::GetInstance().GetMesh(L"Sprite");

		Matrix4x4 worldScale = Matrix4x4::Scale(GetGameObject()->GetTransform()->GetWorldScale());
		Matrix4x4 worldRotation= GetGameObject()->GetTransform()->GetWorldRotation().GetTransformMatrix();
		Matrix4x4 worldPosition = Matrix4x4::Translation(GetGameObject()->GetTransform()->GetWorldPosition());

		Transform* cameraTransform = SceneManager::GetInstance().GetMainCamera()->GetGameObject()->GetTransform();

		Vector3D orgPos = static_cast<Vector3D>(cameraTransform->GetWorldRotation().GetTransformMatrix() * worldRotation.Inverse() * Vector3D::zero) + static_cast<Vector3D>(worldPosition * Vector3D::zero);


		graphics->DrawMesh(Matrix4x4::Translation(orgPos) * cameraTransform->GetWorldRotation().GetTransformMatrix() * worldScale, meshPath, materialInfo, materialInfo.materialName, 0);
	}

	void SpriteRenderer::SetAlpha(float alpha)
	{
		materialInfo.color.alpha = alpha;
	}

	float SpriteRenderer::GetAlpha()
	{
		return materialInfo.color.alpha;
	}


	void SpriteRenderer::PreSerialize(json& jsonData) const
	{
		PRESERIALIZE_BASE(SpriteRenderer);
		PRESERIALIZE_SERVALUE(materialInfo);
	}

	void SpriteRenderer::PreDeserialize(const json& jsonData)
	{
		PREDESERIALIZE_BASE(SpriteRenderer);
		PREDESERIALIZE_SERVALUE(materialInfo);
	}

	void SpriteRenderer::PostSerialize(json& jsonData) const
	{
		POSTSERIALIZE_BASE();
		POSTSERIALIZE_SERVALUE(materialInfo);
	}

	void SpriteRenderer::PostDeserialize(const json& jsonData)
	{
		POSTDESERIALIZE_BASE();
		POSTDESERIALIZE_SERVALUE(materialInfo);
	}
}