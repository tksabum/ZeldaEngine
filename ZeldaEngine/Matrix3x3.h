#pragma once

#include "ZeldaEngineAPI.h"

#include "Serializable.h"

namespace ZeldaEngine
{
	namespace ZeldaMath
	{
		class Matrix3x3;
		class Vector3D;

		class ZELDAENGINE_API Matrix3x3 : public Serializable
		{
		public:
			union
			{
				struct
				{
					float m1_1, m1_2, m1_3;
					float m2_1, m2_2, m2_3;
					float m3_1, m3_2, m3_3;
				};
				float m[3][3];
			};

			Matrix3x3();

			Matrix3x3(
				float m1_1, float m1_2, float m1_3,
				float m2_1, float m2_2, float m2_3,
				float m3_1, float m3_2, float m3_3
			);

			// 역행렬
			Matrix3x3 Inverse() const;

			Matrix3x3 operator+=(const Matrix3x3& r);
			Matrix3x3 operator-=(const Matrix3x3& r);
			Matrix3x3 operator+(const Matrix3x3& r) const;
			Matrix3x3 operator-(const Matrix3x3& r) const;
			Matrix3x3 operator*(const Matrix3x3& r) const;
			Vector3D operator*(const Vector3D& r) const;

			Matrix3x3 operator*(const float& r);
			Matrix3x3 operator*=(const float& r);
			Matrix3x3 operator/(const float& r);

			// Serializable을(를) 통해 상속됨
			virtual void PreSerialize(json& jsonData) const override;
			virtual void PreDeserialize(const json& jsonData) override;
			virtual void PostSerialize(json& jsonData) const override;
			virtual void PostDeserialize(const json& jsonData) override;

			const static Matrix3x3 zero;
			const static Matrix3x3 identity;
		};
	}
}