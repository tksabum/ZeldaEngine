#pragma once

#include "ZeldaEngineAPI.h"

#include "Component.h"

#include "ZeldaMath.h"

#include <list>

namespace ZeldaEngine
{
	class Rigidbody;

	class ZELDAENGINE_API TestComponent2 : public Component
	{
	public:
		TestComponent2();
		virtual ~TestComponent2();

		void Awake();
		void Update();

		void SetLimitLine(float limitY);

	private:
		Rigidbody* rigid;
		float limitLine;
		ZeldaMath::Vector3D originPos;
		ZeldaMath::Quaternion originRot;

		// Component을(를) 통해 상속됨
		virtual void PreSerialize(json& jsonData) const override;
		virtual void PreDeserialize(const json& jsonData) override;
		virtual void PostSerialize(json& jsonData) const override;
		virtual void PostDeserialize(const json& jsonData) override;
	};
}