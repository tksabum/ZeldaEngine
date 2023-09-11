#include "pch.h"
#include "ResourceManager.h"

#include "IGraphics.h"

namespace ZeldaEngine
{
	void ResourceManager::Initialize(IGraphics* graphics)
	{
		this->graphics = graphics;

		for (auto meshData : meshMap)
		{
			graphics->CreateMesh(meshData.second);
		}

		for (auto materialData : materialMap)
		{
			graphics->CreateMaterial(materialData.first, *materialData.second);
		}

		for (auto shaderData : shaderMap)
		{
			graphics->CreateShader(shaderData.second.first, shaderData.second.second);
		}

		for (auto textureData : textureMap)
		{
			graphics->CreateTexture(textureData.second);
		}

		for (auto fbxData : fbxMap)
		{
			graphics->LoadFBX(fbxData.second.first, fbxData.second.second);

			const std::vector<FF::FFMeshData>& meshData = ResourceManager::GetInstance().GetFFMeshData(fbxData.first);

			for (size_t i = 0; i < meshData.size(); ++i)
			{
				ResourceManager::GetInstance().CreateMesh(meshData[i].meshName, meshData[i].meshName);

				if (meshData[i].hasAnimation)
				{
					for (auto& clip : meshData[i].vecAnimClip)
					{
						animClipMap.insert({ clip.animName, clip });
					}
				}
			}
		}
	}

	void ResourceManager::CreateMesh(const std::wstring & meshName, const std::wstring & filePath)
	{
		assert(meshMap.count(meshName) == 0);
		meshMap[meshName] = filePath;

		if (graphics != nullptr)
		{
			graphics->CreateMesh(filePath);
		}
	}

	void ResourceManager::CreateMaterial(const std::wstring & materialName, const MaterialInfo& materialInfo)
	{
		assert(materialMap.count(materialName) == 0);

		MaterialInfo* result = nullptr;
		if (materialMap.count(materialName) == 0)
		{
			result = new MaterialInfo();
			*result = materialInfo;
			result->materialName = materialName;
			materialMap[materialName] = result;
		}

		if (graphics != nullptr && result != nullptr)
		{
			graphics->CreateMaterial(materialName, *result);
		}
	}

	void ResourceManager::CreateShader(const std::wstring & shaderName, const std::wstring & filePath, const ShaderInfo& shaderInfo)
	{
		assert(shaderMap.count(shaderName) == 0);

		shaderMap[shaderName] = { filePath, shaderInfo };

		if (graphics != nullptr)
		{
			graphics->CreateShader(filePath, shaderInfo);
		}
	}

	void ResourceManager::CreateTexture(const std::wstring & textureName, const std::wstring & filePath)
	{
		assert(textureMap.count(textureName) == 0);

		textureMap[textureName] = filePath;

		if (graphics != nullptr)
		{
			graphics->CreateTexture(filePath);
		}
	}

	void ResourceManager::FBXLoad(const std::wstring& fbxName, const std::wstring& filePath, const std::wstring& shaderPath)
	{
		assert(fbxMap.count(fbxName) == 0);

		fbxMap[fbxName] = { filePath ,shaderPath };

		if (graphics != nullptr)
		{
			graphics->LoadFBX(filePath, shaderPath);

			const std::vector<FF::FFMeshData>& meshData = ResourceManager::GetInstance().GetFFMeshData(fbxName);

			for (size_t i = 0; i < meshData.size(); ++i)
			{
				ResourceManager::GetInstance().CreateMesh(meshData[i].meshName, meshData[i].meshName);

				if (meshData[i].hasAnimation)
				{
					for (auto& clip : meshData[i].vecAnimClip)
					{
						animClipMap.insert({ clip.animName, clip });
					}
				}
			}
		}
	}

	const std::vector<FF::FFMeshData>& ResourceManager::GetFFMeshData(const std::wstring& fileName)
	{
		return graphics->GetMeshDataFromFBX(fbxMap[fileName].first);
	}

	FF::FFAnimationClip& ResourceManager::GetAnimationClip(const std::wstring animationName)
	{
		auto iter = animClipMap.find(animationName);
		if (iter != animClipMap.end())
		{
			int a = 1;
		}
		FF::FFAnimationClip& clip =  animClipMap[animationName];
		return animClipMap[animationName];
	}

	void ResourceManager::Clear()
	{
		meshMap.clear();
		materialMap.clear();
		shaderMap.clear();
		textureMap.clear();
		fbxMap.clear();
		animClipMap.clear();
	}

	void ResourceManager::PreSerialize(json& jsonData) const
	{
		// mesh
		size_t meshCounter = 0;
		for (auto iter = meshMap.begin(); iter != meshMap.end(); iter++)
		{
			jsonData["ResourceManager"]["meshList"][std::to_string(meshCounter)]["MeshName"] = wstring_to_string((*iter).first);
			jsonData["ResourceManager"]["meshList"][std::to_string(meshCounter)]["MeshInfo"] = wstring_to_string((*iter).second);
			meshCounter++;
		}
		jsonData["ResourceManager"]["meshList"]["size"] = meshCounter;

		// material
		size_t materialCounter = 0;
		for (auto iter = materialMap.begin(); iter != materialMap.end(); iter++)
		{
			jsonData["ResourceManager"]["materialList"][std::to_string(materialCounter)]["MaterialName"] = wstring_to_string((*iter).first);
			(*iter).second->PreSerialize(jsonData["ResourceManager"]["materialList"][std::to_string(materialCounter)]["MaterialInfo"]);
			materialCounter++;
		}
		jsonData["ResourceManager"]["materialList"]["size"] = materialCounter;

		// shader
		size_t shaderCounter = 0;
		for (auto iter = shaderMap.begin(); iter != shaderMap.end(); iter++)
		{
			jsonData["ResourceManager"]["shaderList"][std::to_string(shaderCounter)]["ShaderName"] = wstring_to_string((*iter).first);
			jsonData["ResourceManager"]["shaderList"][std::to_string(shaderCounter)]["ShaderFilePath"] = wstring_to_string((*iter).second.first);
			(*iter).second.second.PreSerialize(jsonData["ResourceManager"]["shaderList"][std::to_string(shaderCounter)]["ShaderInfo"]);
			shaderCounter++;
		}
		jsonData["ResourceManager"]["shaderList"]["size"] = shaderCounter;

		// texture
		size_t textureCounter = 0;
		for (auto iter = textureMap.begin(); iter != textureMap.end(); iter++)
		{
			jsonData["ResourceManager"]["textureList"][std::to_string(textureCounter)]["textureName"] = wstring_to_string((*iter).first);
			jsonData["ResourceManager"]["textureList"][std::to_string(textureCounter)]["textureInfo"] = wstring_to_string((*iter).second);
			textureCounter++;
		}
		jsonData["ResourceManager"]["textureList"]["size"] = textureCounter;

		// fbx
		size_t fbxCounter = 0;
		for (auto iter = fbxMap.begin(); iter != fbxMap.end(); iter++)
		{
			jsonData["ResourceManager"]["fbxList"][std::to_string(fbxCounter)]["fbxName"] = wstring_to_string((*iter).first);
			jsonData["ResourceManager"]["fbxList"][std::to_string(fbxCounter)]["filePath"] = wstring_to_string((*iter).second.first);
			jsonData["ResourceManager"]["fbxList"][std::to_string(fbxCounter)]["shaderPath"] = wstring_to_string((*iter).second.second);
			fbxCounter++;
		}
		jsonData["ResourceManager"]["fbxList"]["size"] = fbxCounter;
	}

	void ResourceManager::PreDeserialize(const json& jsonData)
	{
		// mesh
		size_t meshCounter = jsonData["ResourceManager"]["meshList"]["size"];
		for (size_t i = 0; i < meshCounter; i++)
		{
			std::wstring key = string_to_wstring(jsonData["ResourceManager"]["meshList"][std::to_string(i)]["MeshName"]);
			std::wstring value = string_to_wstring(jsonData["ResourceManager"]["meshList"][std::to_string(i)]["MeshInfo"]);
			CreateMesh(key, value);
		}

		// shader
		size_t shaderCounter = jsonData["ResourceManager"]["shaderList"]["size"];
		for (size_t i = 0; i < shaderCounter; i++)
		{
			std::wstring key = string_to_wstring(jsonData["ResourceManager"]["shaderList"][std::to_string(i)]["ShaderName"]);
			std::wstring value = string_to_wstring(jsonData["ResourceManager"]["shaderList"][std::to_string(i)]["ShaderFilePath"]);
			ShaderInfo shaderinfo;
			shaderinfo.PreDeserialize(jsonData["ResourceManager"]["shaderList"][std::to_string(i)]["ShaderInfo"]);
			CreateShader(key, value, shaderinfo);
		}

		// texture
		size_t textureCounter = jsonData["ResourceManager"]["textureList"]["size"];
		for (size_t i = 0; i < textureCounter; i++)
		{
			std::wstring key = string_to_wstring(jsonData["ResourceManager"]["textureList"][std::to_string(i)]["textureName"]);
			std::wstring value = string_to_wstring(jsonData["ResourceManager"]["textureList"][std::to_string(i)]["textureInfo"]);
			CreateTexture(key, value);
		}

		// material
		size_t materialCounter = jsonData["ResourceManager"]["materialList"]["size"];
		for (size_t i = 0; i < materialCounter; i++)
		{
			std::wstring key = string_to_wstring(jsonData["ResourceManager"]["materialList"][std::to_string(i)]["MaterialName"]);
			MaterialInfo value;
			value.PreDeserialize(jsonData["ResourceManager"]["materialList"][std::to_string(i)]["MaterialInfo"]);
			CreateMaterial(key, value);
		}

		// fbx
		size_t fbxCounter = jsonData["ResourceManager"]["fbxList"]["size"];
		for (size_t i = 0; i < fbxCounter; i++)
		{
			std::wstring fbxName = string_to_wstring(jsonData["ResourceManager"]["fbxList"][std::to_string(i)]["fbxName"]);
			std::wstring filePath = string_to_wstring(jsonData["ResourceManager"]["fbxList"][std::to_string(i)]["filePath"]);
			std::wstring shaderPath = string_to_wstring(jsonData["ResourceManager"]["fbxList"][std::to_string(i)]["shaderPath"]);
			FBXLoad(fbxName, filePath, shaderPath);
		}
	}

	void ResourceManager::PostSerialize(json& jsonData) const
	{
		// material
		size_t materialCounter = 0;
		for (auto iter = materialMap.begin(); iter != materialMap.end(); iter++)
		{
			jsonData["ResourceManager"]["materialList"][std::to_string(materialCounter)]["MaterialName"] = wstring_to_string((*iter).first);
			(*iter).second->PostSerialize(jsonData["ResourceManager"]["materialList"][std::to_string(materialCounter)]["MaterialInfo"]);
			materialCounter++;
		}
		jsonData["ResourceManager"]["materialList"]["size"] = materialCounter;
	}

	void ResourceManager::PostDeserialize(const json& jsonData)
	{
		// material
		size_t materialCounter = jsonData["ResourceManager"]["materialList"]["size"];
		for (size_t i = 0; i < materialCounter; i++)
		{
			std::wstring key = string_to_wstring(jsonData["ResourceManager"]["materialList"][std::to_string(i)]["MaterialName"]);
			MaterialInfo* value = GetMaterial(key);
			if (value != nullptr)
			{
				value->PostDeserialize(jsonData["ResourceManager"]["materialList"][std::to_string(i)]["MaterialInfo"]);
			}
		}
	}

	ResourceManager& ResourceManager::GetInstance()
	{
		static ResourceManager instance;
		return instance;
	}

	ResourceManager::ResourceManager() : graphics(nullptr)
	{

	}

	ResourceManager::~ResourceManager()
	{

	}
}