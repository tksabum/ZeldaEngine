#pragma once

#include "Collider.h"

namespace ZeldaEngine
{
	class ZELDAENGINE_API CylinderCollider : public Collider
	{
	public:
		CylinderCollider();
		virtual ~CylinderCollider();

		virtual void Render(IGraphics* graphics) override;

	private:
		// Collider��(��) ���� ��ӵ�
		virtual void PreSerialize(json& jsonData) const override;
		virtual void PreDeserialize(const json& jsonData) override;
		virtual void PostSerialize(json& jsonData) const override;
		virtual void PostDeserialize(const json& jsonData) override;
	};
}