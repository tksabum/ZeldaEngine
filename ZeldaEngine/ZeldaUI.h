#pragma once

#include "ZeldaEngineAPI.h"

#include "Component.h"

namespace ZeldaEngine
{
	class ZELDAENGINE_API ZeldaUI : public Component
	{
	public:
		void PostInitialize_();
		virtual ~ZeldaUI();


	private:
		// Component��(��) ���� ��ӵ�
		virtual void PreSerialize(json& jsonData) const override;
		virtual void PreDeserialize(const json& jsonData) override;
		virtual void PostSerialize(json& jsonData) const override;
		virtual void PostDeserialize(const json& jsonData) override;

	};
}