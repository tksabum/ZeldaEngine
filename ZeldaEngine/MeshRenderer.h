#pragma once

#include "ZeldaEngineAPI.h"

#include "Renderer.h"

#include "GraphicsDefine.h"

namespace ZeldaEngine
{
	class ZELDAENGINE_API MeshRenderer : public Renderer
	{
	public:
		MeshRenderer();
		virtual ~MeshRenderer();

		void SetColor(const ZeldaColor& color);
		void SetMesh(std::wstring meshname);
		void SetMaterial(std::wstring materialname);
		void SetMaterialInfo(const MaterialInfo& info, int index);
		void PushMaterialInfo(const MaterialInfo& info);

		MaterialInfo& GetMaterialInfo(int materialIndex);

		// IRenderer을(를) 통해 상속됨
		virtual void Render(IGraphics* graphics) override;

	private:
		std::wstring meshName;
		std::vector<std::pair<std::wstring, MaterialInfo>> materialInfoList;
		
	public:
		// Component을(를) 통해 상속됨
		virtual void PreSerialize(json& jsonData) const override;
		virtual void PreDeserialize(const json& jsonData) override;
		virtual void PostSerialize(json& jsonData) const override;
		virtual void PostDeserialize(const json& jsonData) override;
	};
}