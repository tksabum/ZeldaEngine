#pragma once

#include "IShape.h"
#include "Serializable.h"

namespace ZeldaEngine
{
	class Shape : public IShape, Serializable
	{
	public:
		virtual float GetMaxRadius(ZeldaMath::Matrix4x4 transform) abstract;

		// IShape을(를) 통해 상속됨
		virtual ZeldaMath::Vector3D GetCenterOfMass() abstract;
		virtual ZeldaMath::Matrix3x3 GetInertiaTensor(ZeldaMath::Matrix4x4 scaleMatrix, float mass, ZeldaMath::Vector3D centerOfMass) abstract;
		virtual ZeldaMath::Matrix3x3 GetInertiaTensor(ZeldaMath::Matrix4x4 scaleMatrix, float mass) abstract;
		virtual ZeldaMath::Vector3D GetFarthestPoint(ZeldaMath::Vector3D dirVec) abstract;
		virtual std::vector<ZeldaMath::Vector3D> GetFarthestPoint(ZeldaMath::Vector3D dirVec, float epsilon) abstract;
		virtual float GetVolume() abstract;

		// Serializable을(를) 통해 상속됨
		virtual void PreSerialize(json& jsonData) const abstract;
		virtual void PreDeserialize(const json& jsonData) abstract;
		virtual void PostSerialize(json& jsonData) const abstract;
		virtual void PostDeserialize(const json& jsonData) abstract;

	};
}