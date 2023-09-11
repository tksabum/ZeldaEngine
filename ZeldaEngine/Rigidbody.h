#pragma once

#include "ZeldaEngineAPI.h"

#include "Component.h"

#include "Matrix3x3.h"
#include "Matrix4x4.h"
#include "Matrix4x1.h"
#include "Vector3D.h"

namespace ZeldaEngine
{
	class Collider;

	class ZELDAENGINE_API Rigidbody : public Component
	{
	public:
		Rigidbody();
		virtual ~Rigidbody();

		void PostInitialize_() override;

		// pos에 force만큼 힘을 가함
		void ApplyForce(const ZeldaMath::Vector3D& pos, const ZeldaMath::Vector3D& force);
		void ApplyAcceledration(const ZeldaMath::Vector3D& acc);
		void ApplyAcceleration(const ZeldaMath::Vector3D& pos, const ZeldaMath::Vector3D& acc) = delete;

		ZeldaMath::Vector3D GetCenterOfMass();
		ZeldaMath::Vector3D GetLocalCenterOfMass();
		ZeldaMath::Matrix3x3 GetInverseInertiaTensor();

		float GetVolume();

		void SetMass(float m);
		float GetMass();
		float GetTotalMass();

		Rigidbody* GetRoot();
		void SetParent(Rigidbody* rigidbody);
		Rigidbody* GetParent();
		void AddChild(Rigidbody* rigidbody);
		void RemoveChild(Rigidbody* rigidbody);
		void RemoveAllChild();

		Rigidbody* FindParentRigidbody();

		void AddCollider(Collider* collider);
		void RemoveCollider(Collider* collider);
		void RemoveCollider(const std::vector<Collider*>& colliders);
		void RemoveAllCollider();

		void SetVelocity(ZeldaMath::Vector3D v);
		ZeldaMath::Vector3D GetVelocity();

		void SetAngularVelocity(ZeldaMath::Vector3D v);
		ZeldaMath::Vector3D GetAngularVelocity();

		void SetKinematic(bool kinematic);
		bool GetKinematic();

		void SetKfriction(float friction);
		float GetKfriction();

		void SetSfriction(float friction);
		float GetSfriction();

		void SetRestitution(float value);
		float GetRestitution();

		void SetLinearDrag(float drag);
		float GetLinearDrag();

		void SetAngularDrag(float drag);
		float GetAngularDrag();

		void SetUseGravity(bool value);
		bool GetUseGravity();

		void SetFreezePositionX(bool value);
		bool GetFreezePositionX();

		void SetFreezePositionY(bool value);
		bool GetFreezePositionY();

		void SetFreezePositionZ(bool value);
		bool GetFreezePositionZ();

		void SetFreezeRotationX(bool value);
		bool GetFreezeRotationX();

		void SetFreezeRotationY(bool value);
		bool GetFreezeRotationY();

		void SetFreezeRotationZ(bool value);
		bool GetFreezeRotationZ();

		// 가속도를 속도로 변환한다.
		void LinearAccelerate();

		// 각가속도를 각속도로 변환한다.
		void AngularAccelerate();

		void SetGravityToDefault();
		void SetGravity(ZeldaMath::Vector3D value);
		ZeldaMath::Vector3D GetGravity();

		void Move(ZeldaMath::Vector3D worldMoveVec);

		ZeldaMath::Vector3D GetAcceleration();
		ZeldaMath::Vector3D GetAngularAcceleration();

	private:
		std::vector<Collider*> FindColliders();
		ZeldaMath::Matrix3x3 GetInertiaTensor(ZeldaMath::Vector3D centerofmass);

		Rigidbody* parent;
		std::vector<Rigidbody*> children;
		std::vector<Collider*> colliderList;

		ZeldaMath::Vector3D velocity;
		ZeldaMath::Vector3D acceleration;

		ZeldaMath::Vector3D angularVelocity;
		ZeldaMath::Vector3D angularAcceleration;

		ZeldaMath::Vector3D gravity;

		bool isKinematic;
		
		float mass;

		// 운동마찰계수 (Kinetic Coefficient of Friction)
		float kfriction;
		// 정지마찰계수 (Static Coefficient of Friction)
		float sfriction;

		// 반발계수
		float restitution;

		float linearDrag;
		float angularDrag;

		bool useGravity;

		bool freezePositionX;
		bool freezePositionY;
		bool freezePositionZ;

		bool freezeRotationX;
		bool freezeRotationY;
		bool freezeRotationZ;

		bool isUpdatedVolume;
		float volume;

		bool isUpdatedTotalMass;
		float totalMass;

		bool isUpdatedCenterOfMass;
		ZeldaMath::Vector3D localCenterOfMass;

		bool isUpdatedInertiaTensor;
		ZeldaMath::Matrix3x3 inertiaTensor;

		void ResetUpdatedValue();

		// Component을(를) 통해 상속됨
		virtual void PreSerialize(json& jsonData) const override;
		virtual void PreDeserialize(const json& jsonData) override;
		virtual void PostSerialize(json& jsonData) const override;
		virtual void PostDeserialize(const json& jsonData) override;

		friend class PhysicsSystem;
	};
}