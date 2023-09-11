#pragma once

#include "ZeldaEngineAPI.h"

#include "UIRenderer.h"
#include "ISprite.h"

namespace ZeldaEngine
{
	class ZELDAENGINE_API Image : public UIRenderer, public ISprite
	{
	public:
		void PreInitialize_() override;

		virtual void SetTexture(const std::wstring& textureName) override;

		// IRenderer을(를) 통해 상속됨
		virtual void Render(IGraphics* graphics) override;

		virtual void SetAlpha(float alpha) override;
		float GetAlpha();

	private:
		MaterialInfo materialInfo;

		// UIRenderer을(를) 통해 상속됨
		virtual void PreSerialize(json& jsonData) const override;
		virtual void PreDeserialize(const json& jsonData) override;
		virtual void PostSerialize(json& jsonData) const override;
		virtual void PostDeserialize(const json& jsonData) override;
	};
}