#pragma once

#include "Shape.h"

namespace ZeldaEngine
{
	// 사면체
	class Tetrahedron : public Shape
	{
	public:
		// 삼각뿔 형태로 생각하여 위쪽의 점을 A, 밑면을 이루는 꼭지점을 아랫방향에서 봤을 때 시계방향으로 B, C, D로 한다.
		Tetrahedron(ZeldaMath::Vector3D a, ZeldaMath::Vector3D b, ZeldaMath::Vector3D c, ZeldaMath::Vector3D d);
		virtual ~Tetrahedron();

		// IShape을(를) 통해 상속됨
		virtual ZeldaMath::Vector3D GetCenterOfMass() override;
		virtual ZeldaMath::Matrix3x3 GetInertiaTensor(ZeldaMath::Matrix4x4 scaleMatrix, float mass, ZeldaMath::Vector3D centerOfMass) override;
		virtual ZeldaMath::Matrix3x3 GetInertiaTensor(ZeldaMath::Matrix4x4 scaleMatrix, float mass) override;
		virtual ZeldaMath::Vector3D GetFarthestPoint(ZeldaMath::Vector3D dirVec) override;
		virtual std::vector<ZeldaMath::Vector3D> GetFarthestPoint(ZeldaMath::Vector3D dirVec, float epsilon) override;
		virtual float GetVolume() override;

		// Shape을(를) 통해 상속됨
		virtual float GetMaxRadius(ZeldaMath::Matrix4x4 transform) override;

		// A를 질량중심으로 보고 관성텐서 구하기
		ZeldaMath::Matrix3x3 GetInertiaTensorCenteredAtA();

	private:
		ZeldaMath::Vector3D vertices[4];
		int indices[12];

		ZeldaMath::Vector3D centerOfMass;
		bool isCenterOfMassUpdated;

		ZeldaMath::Matrix3x3 inertiaTensor;
		bool isInertiaTensorUpdated;

		// Shape을(를) 통해 상속됨
		virtual void PreSerialize(json& jsonData) const override;
		virtual void PreDeserialize(const json& jsonData) override;
		virtual void PostSerialize(json& jsonData) const override;
		virtual void PostDeserialize(const json& jsonData) override;
	};
}