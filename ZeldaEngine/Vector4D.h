#pragma once

#include "ZeldaEngineAPI.h"

#include "Serializable.h"

namespace ZeldaEngine
{
	namespace ZeldaMath
	{
		class Matrix4x1;

		class ZELDAENGINE_API Vector4D : public Serializable
		{
		public:
			float x;
			float y;
			float z;
			float w;

			Vector4D(float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 0.0f);
			Vector4D(const Vector4D& vector4d);
			Vector4D(const Matrix4x1& matrix4x1);

			// 단위벡터를 리턴
			Vector4D Normalize();

			// 벡터의 크기를 리턴
			float Size();

			Vector4D operator+=(const Vector4D& right);
			Vector4D operator-=(const Vector4D& right);
			Vector4D operator*=(const float& right);
			Vector4D operator/=(const float& right);

			Vector4D operator+(const Vector4D& right) const;
			Vector4D operator-(const Vector4D& right) const;
			Vector4D operator-() const;
			Vector4D operator*(const float& right) const;
			Vector4D operator/(const float& right) const;
			Vector4D operator=(const Vector4D& right);

			Vector4D operator+(const float& right) = delete;
			Vector4D operator-(const float& right) = delete;

			bool operator==(const Vector4D& right) const;
			bool operator!=(const Vector4D& right) const;
			bool operator<(const Vector4D& right) const;

			// Serializable을(를) 통해 상속됨
			virtual void PreSerialize(json& jsonData) const override;
			virtual void PreDeserialize(const json& jsonData) override;
			virtual void PostSerialize(json& jsonData) const override;
			virtual void PostDeserialize(const json& jsonData) override;

			const static Vector4D zero;
		};
	}
}