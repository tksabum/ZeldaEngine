#include "pch.h"
#include "Rigidbody.h"

#include "ZeldaMath.h"

#include "PhysicsSystem.h"
#include "GameObject.h"
#include "Transform.h"
#include "Collider.h"

#include "EngineSetting.h"

#include <queue>
#include <set>

namespace ZeldaEngine
{
	using namespace ZeldaMath;

	Rigidbody::Rigidbody() :
		parent(nullptr),
		isKinematic(false),
		mass(1.0f),
		restitution(0.05f),
		kfriction(0.1f),
		sfriction(0.2f),
		linearDrag(0.1f),
		angularDrag(0.1f),
		useGravity(true),
		freezePositionX(false),
		freezePositionY(false),
		freezePositionZ(false),
		freezeRotationX(false),
		freezeRotationY(false),
		freezeRotationZ(false),
		isUpdatedVolume(false),
		volume(0.0f),
		isUpdatedTotalMass(false),
		totalMass(1.0f),
		isUpdatedCenterOfMass(false),
		localCenterOfMass(),
		isUpdatedInertiaTensor(false),
		velocity(Vector3D::zero),
		angularVelocity(Vector3D::zero),
		angularAcceleration(Vector3D::zero),
		inertiaTensor(),
		gravity(EngineSetting::GetInstance().GetDefaultGravity())
	{

	}

	Rigidbody::~Rigidbody()
	{
		PhysicsSystem::GetInstance().RemoveRigidbody(this);
	}

	void Rigidbody::PostInitialize_()
	{
		PhysicsSystem::GetInstance().AddRigidbody(this);
	}

	void Rigidbody::ApplyForce(const ZeldaMath::Vector3D& pos, const ZeldaMath::Vector3D& force)
	{
		// isKinematic이면 외부에서 힘을 가할 수 없음
		if (!isKinematic)
		{
			acceleration += (force / mass);

			ZeldaMath::Vector3D torque = ZeldaMath::CrossProduct(pos - GetCenterOfMass(), force);

			ZeldaMath::Matrix3x3 invtensor = GetInverseInertiaTensor();

			angularAcceleration += invtensor * torque;
		}
	}

	void Rigidbody::ApplyAcceledration(const ZeldaMath::Vector3D& acc)
	{
		if (!isKinematic)
		{
			acceleration += acc;
		}
	}

	ZeldaMath::Vector3D Rigidbody::GetCenterOfMass()
	{
		return GetGameObject()->GetTransform()->GetWorldMatrix() * GetLocalCenterOfMass();
	}

	ZeldaMath::Vector3D Rigidbody::GetLocalCenterOfMass()
	{
		if (isUpdatedCenterOfMass == false)
		{
			isUpdatedCenterOfMass = true;

			localCenterOfMass = Vector3D::zero;

			// collider들로 계산한 질량중심
			float colliderVolume = 0.0f;

			std::vector<float> volumeList;

			for (size_t i = 0; i < colliderList.size(); i++)
			{
				float v = colliderList[i]->GetVolume();
				colliderVolume += v;
				volumeList.push_back(v);
			}

			if (colliderVolume != 0.0f)
			{
				for (size_t i = 0; i < colliderList.size(); i++)
				{
					localCenterOfMass += colliderList[i]->GetCenterOfMass() * (volumeList[i] / colliderVolume);
				}
			}

			// 자식rigidbody에서 구한 질량중심과 더한다.
			std::vector<float> massList;
			float tMass = mass;

			for (size_t i = 0; i < children.size(); i++)
			{
				float m = children[i]->GetTotalMass();
				tMass += m;
				massList.push_back(m);
			}

			if (tMass != 0.0f)
			{
				localCenterOfMass *= mass / tMass;

				for (size_t i = 0; i < children.size(); i++)
				{
					localCenterOfMass += children[i]->GetLocalCenterOfMass() * (massList[i] / tMass);
				}
			}
		}

		return localCenterOfMass;
	}

	ZeldaMath::Matrix3x3 Rigidbody::GetInverseInertiaTensor()
	{
		inertiaTensor = GetInertiaTensor(GetCenterOfMass());

		Matrix3x3 inverseInertia = inertiaTensor.Inverse();
		
		if (freezeRotationX)
		{
			inverseInertia.m1_1 = 0.0f;
			inverseInertia.m1_2 = 0.0f;
			inverseInertia.m1_3 = 0.0f;
		}

		if (freezeRotationY)
		{
			inverseInertia.m2_1 = 0.0f;
			inverseInertia.m2_2 = 0.0f;
			inverseInertia.m2_3 = 0.0f;
		}

		if (freezeRotationZ)
		{
			inverseInertia.m3_1 = 0.0f;
			inverseInertia.m3_2 = 0.0f;
			inverseInertia.m3_3 = 0.0f;
		}

		return inverseInertia;
	}

	float Rigidbody::GetVolume()
	{
		if (isUpdatedVolume)
		{
			isUpdatedVolume = true;

			volume = 0.0f;

			// 모든 자식rigidbody의 부피
			for (size_t i = 0; i < children.size(); i++)
			{
				volume += children[i]->GetVolume();
			}

			// 모든 collider의 부피
			for (size_t i = 0; i < colliderList.size(); i++)
			{
				volume += colliderList[i]->GetVolume();
			}
		}

		return volume;
	}

	void Rigidbody::SetMass(float m)
	{
		mass = m;
	}

	float Rigidbody::GetMass()
	{
		return mass;
	}

	float Rigidbody::GetTotalMass()
	{
		if (isUpdatedTotalMass)
		{
			return mass;
		}

		isUpdatedTotalMass = true;

		totalMass = mass;
		for (size_t i = 0; i < children.size(); i++)
		{
			totalMass += children[i]->GetTotalMass();
		}

		return totalMass;
	}

	Rigidbody* Rigidbody::GetRoot()
	{
		Rigidbody* root = this;

		while (root->GetParent() != nullptr)
		{
			root = root->GetParent();
		}

		return root;
	}

	void Rigidbody::SetParent(Rigidbody* rigidbody)
	{
		parent = rigidbody;
	}

	Rigidbody* Rigidbody::GetParent()
	{
		return parent;
	}

	void Rigidbody::AddChild(Rigidbody* rigidbody)
	{
		ResetUpdatedValue();

		children.push_back(rigidbody);
	}

	void Rigidbody::RemoveChild(Rigidbody* rigidbody)
	{
		ResetUpdatedValue();

		children.erase(remove(children.begin(), children.end(), rigidbody), children.end());
	}

	void Rigidbody::RemoveAllChild()
	{
		ResetUpdatedValue();

		children.clear();
	}

	Rigidbody* Rigidbody::FindParentRigidbody()
	{
		Rigidbody* result = nullptr;

		Transform* now = GetGameObject()->GetTransform()->GetParent();

		while (result == nullptr)
		{
			if (now == nullptr) break;

			result = now->GetGameObject()->GetComponent<Rigidbody>();
			now = now->GetParent();
		}

		return result;
	}

	void Rigidbody::AddCollider(Collider* collider)
	{
		ResetUpdatedValue();

		colliderList.push_back(collider);
	}

	void Rigidbody::RemoveCollider(Collider* collider)
	{
		ResetUpdatedValue();

		colliderList.erase(remove(colliderList.begin(), colliderList.end(), collider), colliderList.end());
	}

	void Rigidbody::RemoveCollider(const std::vector<Collider*>& colliders)
	{
		ResetUpdatedValue();

		std::set<Collider*> removeTarget;

		for (size_t i = 0; i < colliders.size(); i++)
		{
			removeTarget.insert(colliders[i]);
		}

		std::vector<Collider*> newList;
		for (size_t i = 0; i < colliderList.size(); i++)
		{
			if (removeTarget.count(colliderList[i]) == 0)
			{
				newList.push_back(colliderList[i]);
			}
		}

		colliderList = newList;
	}

	void Rigidbody::RemoveAllCollider()
	{
		ResetUpdatedValue();

		colliderList.clear();
	}

	void Rigidbody::SetVelocity(ZeldaMath::Vector3D v)
	{
		velocity = v;
	}

	ZeldaMath::Vector3D Rigidbody::GetVelocity()
	{
		return velocity;
	}

	void Rigidbody::SetAngularVelocity(ZeldaMath::Vector3D v)
	{
		angularVelocity = v;
	}

	ZeldaMath::Vector3D Rigidbody::GetAngularVelocity()
	{
		return angularVelocity;
	}

	void Rigidbody::SetKinematic(bool kinematic)
	{
		isKinematic = kinematic;
	}

	bool Rigidbody::GetKinematic()
	{
		return isKinematic;
	}

	void Rigidbody::SetKfriction(float friction)
	{
		kfriction = friction;
	}

	float Rigidbody::GetKfriction()
	{
		return kfriction;
	}

	void Rigidbody::SetSfriction(float friction)
	{
		sfriction = friction;
	}

	float Rigidbody::GetSfriction()
	{
		return sfriction;
	}

	void Rigidbody::SetRestitution(float value)
	{
		restitution = value;
	}

	float Rigidbody::GetRestitution()
	{
		return restitution;
	}

	void Rigidbody::SetLinearDrag(float drag)
	{
		linearDrag = drag;
	}

	float Rigidbody::GetLinearDrag()
	{
		return linearDrag;
	}

	void Rigidbody::SetAngularDrag(float drag)
	{
		angularDrag = drag;
	}

	float Rigidbody::GetAngularDrag()
	{
		return angularDrag;
	}

	void Rigidbody::SetUseGravity(bool value)
	{
		useGravity = value;
	}

	bool Rigidbody::GetUseGravity()
	{
		return useGravity;
	}

	void Rigidbody::SetFreezePositionX(bool value)
	{
		freezePositionX = value;
	}

	bool Rigidbody::GetFreezePositionX()
	{
		return freezePositionX;
	}

	void Rigidbody::SetFreezePositionY(bool value)
	{
		freezePositionY = value;
	}

	bool Rigidbody::GetFreezePositionY()
	{
		return freezePositionY;
	}

	void Rigidbody::SetFreezePositionZ(bool value)
	{
		freezePositionZ = value;
	}

	bool Rigidbody::GetFreezePositionZ()
	{
		return freezePositionZ;
	}

	void Rigidbody::SetFreezeRotationX(bool value)
	{
		freezeRotationX = value;
	}

	bool Rigidbody::GetFreezeRotationX()
	{
		return freezeRotationX;
	}

	void Rigidbody::SetFreezeRotationY(bool value)
	{
		freezeRotationY = value;
	}

	bool Rigidbody::GetFreezeRotationY()
	{
		return freezeRotationY;
	}

	void Rigidbody::SetFreezeRotationZ(bool value)
	{
		freezeRotationZ = value;
	}

	bool Rigidbody::GetFreezeRotationZ()
	{
		return freezeRotationZ;
	}

	void Rigidbody::LinearAccelerate()
	{
		if (freezePositionX) acceleration.x = 0.0f;
		if (freezePositionY) acceleration.y = 0.0f;
		if (freezePositionZ) acceleration.z = 0.0f;

		velocity += acceleration;

		acceleration = Vector3D::zero;
	}

	void Rigidbody::AngularAccelerate()
	{
		angularVelocity += angularAcceleration;
		angularAcceleration = Vector3D::zero;
	}

	void Rigidbody::SetGravityToDefault()
	{
		gravity = EngineSetting::GetInstance().GetDefaultGravity();
	}

	void Rigidbody::SetGravity(ZeldaMath::Vector3D value)
	{
		gravity = value;
	}

	ZeldaMath::Vector3D Rigidbody::GetGravity()
	{
		return gravity;
	}

	void Rigidbody::Move(ZeldaMath::Vector3D worldMoveVec)
	{
		if (freezePositionX) worldMoveVec.x = 0.0f;
		if (freezePositionY) worldMoveVec.y = 0.0f;
		if (freezePositionZ) worldMoveVec.z = 0.0f;

		Vector3D localMoveVec = GetGameObject()->GetTransform()->GetParentWorldMatrix().Inverse() * worldMoveVec;

		GetGameObject()->GetTransform()->SetLocalPosition(GetGameObject()->GetTransform()->GetLocalPosition() + localMoveVec);
	}

	ZeldaMath::Vector3D Rigidbody::GetAcceleration()
	{
		return acceleration;
	}

	ZeldaMath::Vector3D Rigidbody::GetAngularAcceleration()
	{
		return angularAcceleration;
	}

	std::vector<Collider*> Rigidbody::FindColliders()
	{
		std::vector<Collider*> result;

		std::queue<Transform*> q;
		
		q.push(GetGameObject()->GetTransform());

		while (!q.empty())
		{
			Transform* now = q.front();
			q.pop();

			// 자기 자신이 아닌 Rigidbody를 가진 오브젝트를 만나면 continue
			if (now != GetGameObject()->GetTransform() && now->GetGameObject()->GetComponent<Rigidbody>() != nullptr)
			{
				continue;
			}

			Collider* collider = now->GetGameObject()->GetComponent<Collider>();
			if (collider != nullptr)
			{
				result.push_back(collider);
			}

			std::vector<Transform*> transformChildren = now->GetChildren();
			for (size_t i = 0; i < transformChildren.size(); i++)
			{
				q.push(transformChildren[i]);
			}
		}

		return result;
	}

	ZeldaMath::Matrix3x3 Rigidbody::GetInertiaTensor(Vector3D centerofmass)
	{
		if (isUpdatedInertiaTensor == false)
		{
			isUpdatedInertiaTensor = true;

			inertiaTensor = Matrix3x3::zero;

			for (size_t i = 0; i < colliderList.size(); i++)
			{
				float m = GetMass();
				inertiaTensor += colliderList[i]->GetInertiaTensor(m, centerofmass);
			}

			for (size_t i = 0; i < children.size(); i++)
			{
				inertiaTensor += children[i]->GetInertiaTensor(centerofmass);
			}
		}

		return inertiaTensor;
	}

	void Rigidbody::ResetUpdatedValue()
	{
		if (parent != nullptr)
		{
			parent->ResetUpdatedValue();
		}

		isUpdatedVolume = false;
		isUpdatedCenterOfMass = false;
		isUpdatedInertiaTensor = false;
		isUpdatedTotalMass = false;
	}

	void Rigidbody::PreSerialize(json& jsonData) const
	{
		PRESERIALIZE_BASE(Rigidbody);

		PRESERIALIZE_VALUE(isKinematic);
		PRESERIALIZE_VALUE(mass);

		PRESERIALIZE_VALUE(kfriction);
		PRESERIALIZE_VALUE(sfriction);
		PRESERIALIZE_VALUE(restitution);
		PRESERIALIZE_VALUE(linearDrag);
		PRESERIALIZE_VALUE(angularDrag);
		PRESERIALIZE_VALUE(useGravity);
		PRESERIALIZE_VALUE(freezePositionX);
		PRESERIALIZE_VALUE(freezePositionY);
		PRESERIALIZE_VALUE(freezePositionZ);
		PRESERIALIZE_VALUE(freezeRotationX);
		PRESERIALIZE_VALUE(freezeRotationY);
		PRESERIALIZE_VALUE(freezeRotationZ);

		PRESERIALIZE_SERVALUE(gravity);
	}

	void Rigidbody::PreDeserialize(const json& jsonData)
	{
		PREDESERIALIZE_BASE(Rigidbody);

		PREDESERIALIZE_VALUE(isKinematic);
		PREDESERIALIZE_VALUE(mass);

		PREDESERIALIZE_VALUE(kfriction);
		PREDESERIALIZE_VALUE(sfriction);
		PREDESERIALIZE_VALUE(restitution);
		PREDESERIALIZE_VALUE(linearDrag);
		PREDESERIALIZE_VALUE(angularDrag);
		PREDESERIALIZE_VALUE(useGravity);
		PREDESERIALIZE_VALUE(freezePositionX);
		PREDESERIALIZE_VALUE(freezePositionY);
		PREDESERIALIZE_VALUE(freezePositionZ);
		PREDESERIALIZE_VALUE(freezeRotationX);
		PREDESERIALIZE_VALUE(freezeRotationY);
		PREDESERIALIZE_VALUE(freezeRotationZ);

		PREDESERIALIZE_SERVALUE(gravity);
	}

	void Rigidbody::PostSerialize(json& jsonData) const
	{
		POSTSERIALIZE_BASE();

		POSTSERIALIZE_SERVALUE(gravity);
	}
	
	void Rigidbody::PostDeserialize(const json& jsonData)
	{
		POSTDESERIALIZE_BASE();

		POSTDESERIALIZE_SERVALUE(gravity);
	}
}