#pragma once

#include "Component.h"
#include "IRenderer.h"

namespace ZeldaEngine
{
	class ZELDAENGINE_API UIRenderer : public Component, IRenderer
	{
	public:
		UIRenderer();
		void PostInitialize_() override;
		virtual ~UIRenderer();

		virtual void Render(IGraphics* graphics) abstract;

		// -999 ~ 999
		virtual void SetLayer(int value);
		virtual int GetLayer();

	public:
		// Component을(를) 통해 상속됨
		virtual void PreSerialize(json& jsonData) const abstract;
		virtual void PreDeserialize(const json& jsonData) abstract;
		virtual void PostSerialize(json& jsonData) const abstract;
		virtual void PostDeserialize(const json& jsonData) abstract;

	protected:
		int layer;
	};
}