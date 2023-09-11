#pragma once

#include "ZeldaEngineAPI.h"

#include "Renderer.h"
#include "ISprite.h"

namespace ZeldaEngine
{
	class ZELDAENGINE_API SpriteRenderer : public Renderer, public ISprite
	{
	public:
		void PreInitialize_() override;

		virtual void SetTexture(const std::wstring& textureName) override;

		virtual void Render(IGraphics* graphics) override;

		virtual void SetAlpha(float alpha) override;
		float GetAlpha();

	private:
		MaterialInfo materialInfo;


	private:
		virtual void PreSerialize(json& jsonData) const override;
		virtual void PreDeserialize(const json& jsonData) override;
		virtual void PostSerialize(json& jsonData) const override;
		virtual void PostDeserialize(const json& jsonData) override;
	};
}