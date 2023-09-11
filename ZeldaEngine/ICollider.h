#pragma once

#include "Vector3D.h"
#include "Matrix3x3.h"
#include "Matrix4x4.h"

namespace ZeldaEngine
{
	class ICollider
	{
	public:
		// ���� �߽�
		virtual ZeldaMath::Vector3D GetCenterOfMass() abstract;

		// �ٸ� ��ü�� ������ �����߽��� ��߳� Collider�� �����ټ�
		virtual ZeldaMath::Matrix3x3 GetInertiaTensor(float mass, ZeldaMath::Vector3D centerOfMass) abstract;

		// Collider�� �����ټ�
		virtual ZeldaMath::Matrix3x3 GetInertiaTensor(float mass) abstract;

		// ���� ���� dirVec���� ���� �ָ� �ִ� ���� (SupportPoint�� ã�� �� ����Ѵ�)
		virtual ZeldaMath::Vector3D GetFarthestPoint(ZeldaMath::Vector3D dirVec) abstract;

		// ���� ���� dirVec���� ���� �ָ� �ִ� ������ epsilon��ŭ�� ���̳��� ��� ���ؽ��� ã�´�.
		virtual std::vector<ZeldaMath::Vector3D> GetFarthestPoint(ZeldaMath::Vector3D dirVec, float epsilon) abstract;
	};
}