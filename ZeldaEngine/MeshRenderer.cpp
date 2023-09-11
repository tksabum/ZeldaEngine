#include "pch.h"
#include "MeshRenderer.h"

#include "GameObject.h"
#include "Transform.h"

#include "IGraphics.h"

#include "ResourceManager.h"

namespace ZeldaEngine
{
	MeshRenderer::MeshRenderer() :
		meshName(L""),
		materialInfoList()
	{

	}

	MeshRenderer::~MeshRenderer()
	{

	}

	void MeshRenderer::SetColor(const ZeldaColor& color)
	{
		assert(materialInfoList.size() == 1);
		if (materialInfoList.size() == 0) return;

		materialInfoList[0].second.color = color;
	}

	void MeshRenderer::SetMesh(std::wstring meshname)
	{
		meshName = meshname;
	}

	void MeshRenderer::SetMaterial(std::wstring materialname)
	{
		assert(materialInfoList.size() == 0);
		if (materialInfoList.size() != 0) return;

		materialInfoList.push_back({ materialname, MaterialInfo() });

		materialInfoList[0].second = *ResourceManager::GetInstance().GetMaterial(materialname);
	}

	void MeshRenderer::SetMaterialInfo(const MaterialInfo& info, int index)
	{
		materialInfoList[index].first = info.materialName;
		materialInfoList[index].second = info;
	}

	void MeshRenderer::PushMaterialInfo(const MaterialInfo& info)
	{
		materialInfoList.push_back({ info.materialName, info });
	}

	MaterialInfo& MeshRenderer::GetMaterialInfo(int materialIndex)
	{
		return materialInfoList[materialIndex].second;
	}

	void MeshRenderer::Render(IGraphics* graphics)
	{
		for (size_t i = 0; i < materialInfoList.size(); i++)
		{
			ZeldaMath::Matrix4x4 worldTM = gameObject->GetTransform()->GetWorldMatrix();
			std::wstring meshPath = ResourceManager::GetInstance().GetMesh(meshName);

			graphics->DrawMesh(worldTM, meshPath, materialInfoList[i].second, materialInfoList[i].second.materialName, i);
		}
	}

	void MeshRenderer::PreSerialize(json& jsonData) const
	{
		PRESERIALIZE_BASE(MeshRenderer);

		PRESERIALIZE_WSTRVALUE(meshName);

		jsonData["Component"]["materialInfoList"]["size"] = materialInfoList.size();
		for (size_t i = 0; i < materialInfoList.size(); i++)
		{
			jsonData["Component"]["materialInfoList"][std::to_string(i)]["materialName"] = wstring_to_string(materialInfoList[i].first);
			materialInfoList[i].second.PreSerialize(jsonData["Component"]["materialInfoList"][std::to_string(i)]["materialInfo"]);
		}
	}

	void MeshRenderer::PreDeserialize(const json& jsonData)
	{
		PREDESERIALIZE_BASE(MeshRenderer);

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

	void MeshRenderer::PostSerialize(json& jsonData) const
	{
		POSTSERIALIZE_BASE();
		for (size_t i = 0; i < materialInfoList.size(); i++)
		{
			materialInfoList[i].second.PostSerialize(jsonData["Component"]["materialInfoList"][std::to_string(i)]["materialInfo"]);
		}
	}

	void MeshRenderer::PostDeserialize(const json& jsonData)
	{
		POSTDESERIALIZE_BASE();
		for (size_t i = 0; i < materialInfoList.size(); i++)
		{
			materialInfoList[i].second.PostDeserialize(jsonData["Component"]["materialInfoList"][std::to_string(i)]["materialInfo"]);
		}
	}
}