#pragma once

#include "ZeldaEngineAPI.h"

#include <string>
#include <map>

#include "GraphicsDefine.h"

#include "Serializable.h"

namespace ZeldaEngine
{
	class IGraphics;

	class ZELDAENGINE_API ResourceManager : public Serializable
	{
	public:
		void Initialize(IGraphics* graphics);

		void CreateMesh(const std::wstring & meshName, const std::wstring & filePath);
		void CreateMaterial(const std::wstring& materialName, const MaterialInfo& materialInfo);
		void CreateShader(const std::wstring & shaderName, const std::wstring & filePath, const ShaderInfo& shaderInfo);
		void CreateTexture(const std::wstring & textureName, const std::wstring & filePath);
		void FBXLoad(const std::wstring& fbxName, const std::wstring& filePath, const std::wstring& shaderPath);

		const std::wstring& GetMesh(const std::wstring & meshName) { return meshMap[meshName]; };
		MaterialInfo* GetMaterial(const std::wstring & materialName) { return  materialMap[materialName]; };
		const std::pair<std::wstring, ShaderInfo>& GetShader(const std::wstring & shaderName) { return  shaderMap[shaderName]; };
		const std::wstring& GetTexture(const std::wstring& textureName) { return  textureMap[textureName]; };
		const std::vector<FF::FFMeshData>& GetFFMeshData(const std::wstring& fileName);
		FF::FFAnimationClip& GetAnimationClip(const std::wstring animationName);
		
		// 그래픽스엔진에 리소스를 해제하라는 명령을 하는 것은 아님, 리소스관리를 에디터에서 하지 못하는 동안 관리를 위한 함수
		void Clear();

	private:
		IGraphics* graphics;

		// 메쉬
		std::map<std::wstring, std::wstring> meshMap;

		// 머테리얼
		std::map<std::wstring, MaterialInfo*> materialMap;

		// 셰이더
		std::map<std::wstring, std::pair<std::wstring, ShaderInfo>> shaderMap;

		// 텍스쳐
		std::map<std::wstring, std::wstring> textureMap;

		// FBX File
		// fbx name , <fbx path, shader path>
		std::map<std::wstring, std::pair<std::wstring, std::wstring>> fbxMap;

		// AnimationClip
		std::map<std::wstring, FF::FFAnimationClip> animClipMap;


		// Serializable을(를) 통해 상속됨
		virtual void PreSerialize(json& jsonData) const override;
		virtual void PreDeserialize(const json& jsonData) override;
		virtual void PostSerialize(json& jsonData) const override;
		virtual void PostDeserialize(const json& jsonData) override;


		//singleton
	public:
		static ResourceManager& GetInstance();

	private:
		ResourceManager();
		~ResourceManager();
		ResourceManager(const ResourceManager& ref) = delete;
		ResourceManager& operator=(const ResourceManager& ref) = delete;
	};
}

