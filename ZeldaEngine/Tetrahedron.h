#pragma once

#include "Shape.h"

namespace ZeldaEngine
{
	// ���ü
	class Tetrahedron : public Shape
	{
	public:
		// �ﰢ�� ���·� �����Ͽ� ������ ���� A, �ظ��� �̷�� �������� �Ʒ����⿡�� ���� �� �ð�������� B, C, D�� �Ѵ�.
		Tetrahedron(ZeldaMath::Vector3D a, ZeldaMath::Vector3D b, ZeldaMath::Vector3D c, ZeldaMath::Vector3D d);
		virtual ~Tetrahedron();

		// IShape��(��) ���� ��ӵ�
		virtual ZeldaMath::Vector3D GetCenterOfMass() override;
		virtual ZeldaMath::Matrix3x3 GetInertiaTensor(ZeldaMath::Matrix4x4 scaleMatrix, float mass, ZeldaMath::Vector3D centerOfMass) override;
		virtual ZeldaMath::Matrix3x3 GetInertiaTensor(ZeldaMath::Matrix4x4 scaleMatrix, float mass) override;
		virtual ZeldaMath::Vector3D GetFarthestPoint(ZeldaMath::Vector3D dirVec) override;
		virtual std::vector<ZeldaMath::Vector3D> GetFarthestPoint(ZeldaMath::Vector3D dirVec, float epsilon) override;
		virtual float GetVolume() override;

		// Shape��(��) ���� ��ӵ�
		virtual float GetMaxRadius(ZeldaMath::Matrix4x4 transform) override;

		// A�� �����߽����� ���� �����ټ� ���ϱ�
		ZeldaMath::Matrix3x3 GetInertiaTensorCenteredAtA();

	private:
		ZeldaMath::Vector3D vertices[4];
		int indices[12];

		ZeldaMath::Vector3D centerOfMass;
		bool isCenterOfMassUpdated;

		ZeldaMath::Matrix3x3 inertiaTensor;
		bool isInertiaTensorUpdated;

		// Shape��(��) ���� ��ӵ�
		virtual void PreSerialize(json& jsonData) const override;
		virtual void PreDeserialize(const json& jsonData) override;
		virtual void PostSerialize(json& jsonData) const override;
		virtual void PostDeserialize(const json& jsonData) override;
	};
}