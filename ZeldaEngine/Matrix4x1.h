#pragma once

#include "ZeldaEngineAPI.h"

#include "Serializable.h"

namespace ZeldaEngine
{
	namespace ZeldaMath
	{
		class Vector3D;

		class ZELDAENGINE_API Matrix4x1 : public Serializable
		{
		public:
			float m1_1;
			float m2_1;
			float m3_1;
			float m4_1;

			Matrix4x1(float x, float y, float z, float w = 1.0f);
			Matrix4x1(const Vector3D& vector3d);
			Matrix4x1(const Matrix4x1& matrix4x1);

			Matrix4x1 operator=(const Vector3D& right);

			// Serializable을(를) 통해 상속됨
			virtual void PreSerialize(json& jsonData) const override;

			virtual void PreDeserialize(const json& jsonData) override;

			virtual void PostSerialize(json& jsonData) const override;

			virtual void PostDeserialize(const json& jsonData) override;

		};
	}
}