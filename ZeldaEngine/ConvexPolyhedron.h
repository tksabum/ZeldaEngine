#pragma once

#include "Shape.h"

namespace ZeldaEngine
{
	class ConvexPolyhedron : public Shape
	{
	public:
		ConvexPolyhedron(const std::vector<ZeldaMath::Vector3D>& vtx, const std::vector<int>& idx);
		virtual ~ConvexPolyhedron();

		// IShape을(를) 통해 상속됨
		virtual ZeldaMath::Vector3D GetCenterOfMass() override;
		virtual ZeldaMath::Matrix3x3 GetInertiaTensor(ZeldaMath::Matrix4x4 scaleMatrix, float mass, ZeldaMath::Vector3D centerOfMass) override;
		virtual ZeldaMath::Matrix3x3 GetInertiaTensor(ZeldaMath::Matrix4x4 scaleMatrix, float mass) override;
		virtual ZeldaMath::Vector3D GetFarthestPoint(ZeldaMath::Vector3D dirVec) override;
		virtual std::vector<ZeldaMath::Vector3D> GetFarthestPoint(ZeldaMath::Vector3D dirVec, float epsilon) override;
		virtual float GetVolume() override;

		// Shape을(를) 통해 상속됨
		virtual float GetMaxRadius(ZeldaMath::Matrix4x4 transform) override;

	private:
		std::vector<ZeldaMath::Vector3D> vertices;
		std::vector<int> indices;

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