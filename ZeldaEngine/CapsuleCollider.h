#pragma once

#include "Collider.h"

namespace ZeldaEngine
{
	class ZELDAENGINE_API CapsuleCollider : public Collider
	{
	public:
		CapsuleCollider();
		virtual ~CapsuleCollider();

		virtual void Render(IGraphics* graphics) override;

	private:
		// Collider을(를) 통해 상속됨
		virtual void PreSerialize(json& jsonData) const override;
		virtual void PreDeserialize(const json& jsonData) override;
		virtual void PostSerialize(json& jsonData) const override;
		virtual void PostDeserialize(const json& jsonData) override;
	};
}