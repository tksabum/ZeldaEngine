#pragma once

#include "Vector3D.h"
#include "Matrix3x3.h"
#include "Matrix4x4.h"

namespace ZeldaEngine
{
	class ICollider
	{
	public:
		// 질량 중심
		virtual ZeldaMath::Vector3D GetCenterOfMass() abstract;

		// 다른 물체와 합쳐져 질량중심이 어긋난 Collider의 관성텐서
		virtual ZeldaMath::Matrix3x3 GetInertiaTensor(float mass, ZeldaMath::Vector3D centerOfMass) abstract;

		// Collider의 관성텐서
		virtual ZeldaMath::Matrix3x3 GetInertiaTensor(float mass) abstract;

		// 방향 벡터 dirVec으로 가장 멀리 있는 정점 (SupportPoint를 찾을 때 사용한다)
		virtual ZeldaMath::Vector3D GetFarthestPoint(ZeldaMath::Vector3D dirVec) abstract;

		// 방향 벡터 dirVec으로 가장 멀리 있는 정점과 epsilon만큼만 차이나는 모든 버텍스를 찾는다.
		virtual std::vector<ZeldaMath::Vector3D> GetFarthestPoint(ZeldaMath::Vector3D dirVec, float epsilon) abstract;
	};
}