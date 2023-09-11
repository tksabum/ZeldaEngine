#include "pch.h"
#include "SkinnedMeshRenderer.h"

#include "GameObject.h"
#include "Transform.h"

#include "IGraphics.h"

#include "ResourceManager.h"
#include "Animator.h"


namespace ZeldaEngine
{
	SkinnedMeshRenderer::SkinnedMeshRenderer() :
		meshName(L""),
		materialInfoList()
	{

	}

	SkinnedMeshRenderer::~SkinnedMeshRenderer()
	{

	}

	void SkinnedMeshRenderer::SetColor(const ZeldaColor& color)
	{
		assert(materialInfoList.size() == 1);
		if (materialInfoList.size() == 0) return;

		materialInfoList[0].second.color = color;
	}

	void SkinnedMeshRenderer::SetMesh(std::wstring meshname)
	{
		meshName = meshname;
	}

	void SkinnedMeshRenderer::SetMaterial(std::wstring materialname)
	{
		assert(materialInfoList.size() == 0);
		if (materialInfoList.size() != 0) return;

		materialInfoList.push_back({ materialname, MaterialInfo() });

		materialInfoList[0].second = *ResourceManager::GetInstance().GetMaterial(materialname);
	}

	void SkinnedMeshRenderer::SetMaterialInfo(const MaterialInfo& info, int index)
	{
		materialInfoList[index].first = info.materialName;
		materialInfoList[index].second = info;
	}

	void SkinnedMeshRenderer::PushMaterialInfo(const MaterialInfo& info)
	{
		materialInfoList.push_back({ info.materialName, info });
	}

	std::wstring SkinnedMeshRenderer::GetMeshName()
	{
		return meshName;
	}

	MaterialInfo& SkinnedMeshRenderer::GetMaterialInfo(int materialIndex)
	{
		return materialInfoList[materialIndex].second;
	}

	void SkinnedMeshRenderer::Awake()
	{
		Transform* transform = gameObject->GetTransform()->GetParent();

		if (transform == nullptr)
		{
			return;
		}

		animator = transform->GetGameObject()->GetComponent<Animator>();
		while (animator == nullptr)
		{
			transform = transform->GetParent();

			if (transform == nullptr)
			{
				break;
			}
			animator = transform->GetGameObject()->GetComponent<Animator>();
		}
	}

	void SkinnedMeshRenderer::Render(IGraphics* graphics)
	{
		for (size_t i = 0; i < materialInfoList.size(); i++)
		{
			ZeldaMath::Matrix4x4 worldTM = gameObject->GetTransform()->GetWorldMatrix();
			std::wstring meshPath = ResourceManager::GetInstance().GetMesh(meshName);

			unsigned __int64 id = 0;

			if (animator != nullptr)
			{
				id = animator->GetID();
				// Animator -> unsigned __int64 ID | __int32 frame | __int32 nextFrame | float frameRatio
				graphics->DrawSkinnedMesh(id, animator->GetFrame(), animator->GetNextFrame(), animator->GetFrameRatio(),
					animator->GetCurrentAnimationName() , worldTM, meshPath, materialInfoList[i].second, materialInfoList[i].second.materialName,i);
			}
		}
	}

	void SkinnedMeshRenderer::PreSerialize(json& jsonData) const
	{
		// TODO
		PRESERIALIZE_BASE(SkinnedMeshRenderer);

		PRESERIALIZE_WSTRVALUE(meshName);

		jsonData["Component"]["materialInfoList"]["size"] = materialInfoList.size();
		for (size_t i = 0; i < materialInfoList.size(); i++)
		{
			jsonData["Component"]["materialInfoList"][std::to_string(i)]["materialName"] = wstring_to_string(materialInfoList[i].first);
			materialInfoList[i].second.PreSerialize(jsonData["Component"]["materialInfoList"][std::to_string(i)]["materialInfo"]);
		}
	}

	void SkinnedMeshRenderer::PreDeserialize(const json& jsonData)
	{
		// TODO
		PREDESERIALIZE_BASE(SkinnedMeshRenderer);

		PREDESERIALIZE_WSTRVALUE(meshName);

		size_t materialInfoListSize = jsonData["Component"]["materialInfoList"]["size"];
		for (size_t i = 0; i < materialInfoListSize; i++)
		{
			std::wstring materialname = string_to_wstring(jsonData["Component"]["materialInfoList"][std::to_string(i)]["materialName"]);
			MaterialInfo materialinfo;
			materialinfo.PreDeserialize(jsonData["Component"]["materialInfoList"][std::to_string(i)]["materialInfo"]);
			materialInfoList.push_back({ materialname, materialinfo });
		}
	}

	void SkinnedMeshRenderer::PostSerialize(json& jsonData) const
	{
		// TODO
		POSTSERIALIZE_BASE();

		for (size_t i = 0; i < materialInfoList.size(); i++)
		{
			materialInfoList[i].second.PostSerialize(jsonData["Component"]["materialInfoList"][std::to_string(i)]["materialInfo"]);
		}
	}

	void SkinnedMeshRenderer::PostDeserialize(const json& jsonData)
	{
		// TODO
		POSTDESERIALIZE_BASE();

		for (size_t i = 0; i < materialInfoList.size(); i++)
		{
			materialInfoList[i].second.PostDeserialize(jsonData["Component"]["materialInfoList"][std::to_string(i)]["materialInfo"]);
		}
	}
}

