#pragma once

#include "Light.h"

#include "GraphicsDefine.h"

namespace ZeldaEngine
{
	class SpotLight : public Light
	{
	public:
		SpotLight();
		virtual ~SpotLight();


		// ILight��(��) ���� ��ӵ�
		virtual void Render(IGraphics* graphics) override;

	public:
		// Component��(��) ���� ��ӵ�
		virtual void PreSerialize(json& jsonData) const override;
		virtual void PreDeserialize(const json& jsonData) override;
		virtual void PostSerialize(json& jsonData) const override;
		virtual void PostDeserialize(const json& jsonData) override;
	};
}
