#pragma once

#include "ZeldaEngineAPI.h"

#include "Light.h"

#include "GraphicsDefine.h"

namespace ZeldaEngine
{
	class ZELDAENGINE_API DirectionalLight : public Light
	{
	public:
		DirectionalLight();
		virtual ~DirectionalLight();

		// ILight��(��) ���� ��ӵ�
		virtual void Render(IGraphics* graphics) override;

	private:
		void UpdateLightInfo() override;

	public:
		// Component��(��) ���� ��ӵ�
		virtual void PreSerialize(json& jsonData) const override;
		virtual void PreDeserialize(const json& jsonData) override;
		virtual void PostSerialize(json& jsonData) const override;
		virtual void PostDeserialize(const json& jsonData) override;
	};
}