#pragma once

#include "ZeldaEngineAPI.h"

#include "Component.h"

#include "ZeldaMath.h"

#include <list>
#include <vector>
#include <map>

namespace ZeldaEngine
{
	class Transform;
	class Camera;
	class MeshRenderer;

	class ZELDAENGINE_API TestComponent : public Component
	{
	public:
		TestComponent();
		virtual ~TestComponent();

		void Update();
		void LateUpdate();
		void OnTriggerEnter();
		void OnTriggerExit();

		float moveSpeed;
		float rotateSpeed;

		std::vector<float> testVectorValue;
		std::vector<ZeldaMath::Vector3D> testVectorSerValue;
		std::vector<Transform*> testVectorPtr;

		std::vector<float> testVectorValue0;
		std::vector<ZeldaMath::Vector3D> testVectorSerValue0;
		std::vector<Transform*> testVectorPtr0;

	private:
		static std::list<TestComponent*> testComponentList;
		static bool updateOnlyOneComponent;

		ZeldaMath::Vector2D lastMousePosition;
		ZeldaMath::Quaternion lastRotation;

		MeshRenderer* renderer;

		// Component을(를) 통해 상속됨
		virtual void PreSerialize(json& jsonData) const override;

		virtual void PreDeserialize(const json& jsonData) override;

		virtual void PostSerialize(json& jsonData) const override;

		virtual void PostDeserialize(const json& jsonData) override;
	};
}