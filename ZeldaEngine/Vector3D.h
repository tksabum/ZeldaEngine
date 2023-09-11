#pragma once

#include "ZeldaEngineAPI.h"

#include "Serializable.h"

namespace ZeldaEngine
{
	namespace ZeldaMath
	{
		class Matrix4x1;

		class ZELDAENGINE_API Vector3D : public Serializable
		{
		public:
			float x;
			float y;
			float z;

			Vector3D(float x = 0.0f, float y = 0.0f, float z = 0.0f);
			Vector3D(const Vector3D& vector3d);
			Vector3D(const Matrix4x1& matrix4x1);

			// 단위벡터를 리턴
			Vector3D Normalize() const;

			// 벡터의 크기를 리턴
			float Size() const;

			Vector3D operator+=(const Vector3D& right);
			Vector3D operator-=(const Vector3D& right);
			Vector3D operator*=(const float& right);
			Vector3D operator/=(const float& right);

			Vector3D operator+(const Vector3D& right) const;
			Vector3D operator-(const Vector3D& right) const;
			Vector3D operator-() const;
			Vector3D operator*(const float& right) const;
			Vector3D operator/(const float& right) const;
			Vector3D operator=(const Vector3D& right);

			Vector3D operator+(const float& right) = delete;
			Vector3D operator-(const float& right) = delete;

			bool operator==(const Vector3D& right) const;
			bool operator!=(const Vector3D& right) const;
			bool operator<(const Vector3D& right) const;


			// Serializable을(를) 통해 상속됨
			virtual void PreSerialize(json& jsonData) const override;

			virtual void PreDeserialize(const json& jsonData) override;

			virtual void PostSerialize(json& jsonData) const override;

			virtual void PostDeserialize(const json& jsonData) override;

			const static Vector3D zero;
			const static Vector3D front;
			const static Vector3D right;
			const static Vector3D up;
		};
	}
}