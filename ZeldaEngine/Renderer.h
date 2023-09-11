#pragma once

#include "ZeldaEngineAPI.h"

#include "Component.h"
#include "IRenderer.h"

namespace ZeldaEngine
{
	class ZELDAENGINE_API Renderer : public Component, IRenderer
	{
	public:
		Renderer();
		virtual ~Renderer();

		virtual void Render(IGraphics* graphics) abstract;

	public:
		// Component을(를) 통해 상속됨
		virtual void PreSerialize(json& jsonData) const abstract;
		virtual void PreDeserialize(const json& jsonData) abstract;
		virtual void PostSerialize(json& jsonData) const abstract;
		virtual void PostDeserialize(const json& jsonData) abstract;
	};
}