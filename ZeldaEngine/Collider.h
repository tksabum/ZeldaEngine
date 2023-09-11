#pragma once

#include "ZeldaEngineAPI.h"

#include "Component.h"
#include "ICollider.h"

#include "Matrix4x4.h"
#include "Vector3D.h"

namespace ZeldaEngine
{
	class Shape;
	class Rigidbody;
	class IGraphics;

	class ZELDAENGINE_API Collider : public Component, ICollider
	{
	protected:
		Collider();
		Shape* colliderShape;

		Rigidbody* master;

		void PostInitialize_() override;

		bool isTrigger;

	public:
		virtual ~Collider();

		Rigidbody* FindMasterRigidbody();
		void SetMaster(Rigidbody* rigidbody);
		Rigidbody* GetMaster();

		float GetVolume();
		ZeldaMath::Vector3D GetCenter();
		float GetMaxRadius();

		void SetTrigger(bool value);
		bool IsTrigger();

		virtual void Render(IGraphics* graphics) abstract;

	public:
		// ICollider을(를) 통해 상속됨
		virtual ZeldaMath::Vector3D GetCenterOfMass() override;
		virtual ZeldaMath::Matrix3x3 GetInertiaTensor(float mass, ZeldaMath::Vector3D centerOfMass) override;
		virtual ZeldaMath::Matrix3x3 GetInertiaTensor(float mass) override;
		virtual ZeldaMath::Vector3D GetFarthestPoint(ZeldaMath::Vector3D dirVec) override;
		virtual std::vector<ZeldaMath::Vector3D> GetFarthestPoint(ZeldaMath::Vector3D dirVec, float epsilon) override;

		// Component을(를) 통해 상속됨
		virtual void PreSerialize(json& jsonData) const abstract;
		virtual void PreDeserialize(const json& jsonData) abstract;
		virtual void PostSerialize(json& jsonData) const abstract;
		virtual void PostDeserialize(const json& jsonData) abstract;
	};
}