#include "pch.h"
#include "Collider.h"

#include "Shape.h"
#include "Tetrahedron.h"
#include "ConvexPolyhedron.h"

#include "PhysicsSystem.h"

#include "GameObject.h"
#include "Transform.h"
#include "Rigidbody.h"

namespace ZeldaEngine
{
	using namespace ZeldaMath;

	Collider::Collider() : colliderShape(nullptr), master(nullptr), isTrigger(false)
	{
		
	}

	void Collider::PostInitialize_()
	{
		PhysicsSystem::GetInstance().AddCollider(this);
	}

	Collider::~Collider()
	{
		PhysicsSystem::GetInstance().RemoveCollider(this);
	}

	Rigidbody* Collider::FindMasterRigidbody()
	{
		Rigidbody* result = nullptr;

		Transform* now = GetGameObject()->GetTransform();

		while (result == nullptr)
		{
			result = now->GetGameObject()->GetComponent<Rigidbody>();
			now = now->GetParent();

			if (now == nullptr) break;
		}

		return result;
	}

	void Collider::SetMaster(Rigidbody* rigidbody)
	{
		master = rigidbody;
	}

	Rigidbody* Collider::GetMaster()
	{
		return master;
	}

	float Collider::GetVolume()
	{
		return colliderShape->GetVolume();
	}

	ZeldaMath::Vector3D Collider::GetCenter()
	{
		// 현재는 Collider에 추가적인 위치이동 옵션이 없지만 추가될 경우 수정 필요
		return GetGameObject()->GetTransform()->GetWorldMatrix() * Vector3D::zero;
	}

	float Collider::GetMaxRadius()
	{
		// 현재는 Collider에 추가적인 위치이동 옵션이 없지만 추가될 경우 수정 필요
		
		return colliderShape->GetMaxRadius(Matrix4x4::Scale(GetGameObject()->GetTransform()->GetWorldScale()));
	}

	void Collider::SetTrigger(bool value)
	{
		isTrigger = value;
	}

	bool Collider::IsTrigger()
	{
		return isTrigger;
	}

	ZeldaMath::Vector3D Collider::GetCenterOfMass()
	{
		return colliderShape->GetCenterOfMass();
	}

	ZeldaMath::Matrix3x3 Collider::GetInertiaTensor(float mass, ZeldaMath::Vector3D centerOfMass)
	{
		Vector3D worldPos = GetGameObject()->GetTransform()->GetWorldMatrix() * Vector3D::zero;
		Matrix4x4 scaleMatrix = Matrix4x4::Scale(GetGameObject()->GetTransform()->GetWorldScale());
		return colliderShape->GetInertiaTensor(scaleMatrix, mass, worldPos - centerOfMass);
	}

	ZeldaMath::Matrix3x3 Collider::GetInertiaTensor(float mass)
	{
		Matrix4x4 scaleMatrix = Matrix4x4::Scale(GetGameObject()->GetTransform()->GetWorldScale());
		return colliderShape->GetInertiaTensor(scaleMatrix, mass);
	}

	ZeldaMath::Vector3D Collider::GetFarthestPoint(ZeldaMath::Vector3D dirVec)
	{
		Vector3D invDirVec = GetGameObject()->GetTransform()->GetWorldRotation().GetTransformMatrix().Inverse() * dirVec.Normalize();
		
		return GetGameObject()->GetTransform()->GetWorldMatrix() * colliderShape->GetFarthestPoint(invDirVec);
	}

	std::vector<ZeldaMath::Vector3D> Collider::GetFarthestPoint(ZeldaMath::Vector3D dirVec, float epsilon)
	{
		Vector3D invDirVec = GetGameObject()->GetTransform()->GetWorldRotation().GetTransformMatrix().Inverse() * dirVec.Normalize();

		Vector3D worldScale = GetGameObject()->GetTransform()->GetWorldScale();
		assert(Max(worldScale.x, Max(worldScale.y, worldScale.z)) != 0.0f);
		float invMaxScale = 1.0f / Max(worldScale.x, Max(worldScale.y, worldScale.z));
		if (Max(worldScale.x, Max(worldScale.y, worldScale.z)) == 0.0f)
		{
			invMaxScale = 0.0f;
		}

		std::vector<ZeldaMath::Vector3D> result = colliderShape->GetFarthestPoint(invDirVec, epsilon * invMaxScale);

		for (size_t i = 0; i < result.size(); i++)
		{
			result[i] = GetGameObject()->GetTransform()->GetWorldMatrix() * result[i];
		}

		return result;
	}
}