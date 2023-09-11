#pragma once

#include "ZeldaEngineAPI.h"

#include "Serializable.h"

namespace ZeldaEngine
{
	namespace ZeldaMath
	{
		class Vector3D;
		class Matrix4x1;

		class ZELDAENGINE_API Matrix4x4 : public Serializable
		{
		public:
			union
			{
				struct
				{
					float m1_1, m1_2, m1_3, m1_4;
					float m2_1, m2_2, m2_3, m2_4;
					float m3_1, m3_2, m3_3, m3_4;
					float m4_1, m4_2, m4_3, m4_4;
				};
				float m[4][4];
			};

			Matrix4x4();

			Matrix4x4(
				float m1_1, float m1_2, float m1_3, float m1_4,
				float m2_1, float m2_2, float m2_3, float m2_4,
				float m3_1, float m3_2, float m3_3, float m3_4,
				float m4_1, float m4_2, float m4_3, float m4_4
			);

			// �����
			Matrix4x4 Inverse() const;

			// ��ġ���
			Matrix4x4 Transpose() const;

			// x, y ��ŭ �̵���Ű�� ��ȯ���
			static Matrix4x4 Translation(float x, float y, float z);
			// MoveVector ��ŭ �̵���Ű�� ��ȯ���
			static Matrix4x4 Translation(Vector3D movevector);

			// ��ǥ��� y-up �޼���ǥ�踦 �������� �Ѵ�.
			static Matrix4x4 Rotation(Vector3D axis, float radianangle);
			// ȸ�� ������ x, y, z ������ �Ѵ�.
			static Matrix4x4 EulerRotation(Vector3D eulerdegreeangle);

			// ������ ��ȯ���
			static Matrix4x4 Scale(Vector3D scale);

			Matrix4x4 operator+=(const Matrix4x4& r);
			Matrix4x4 operator-=(const Matrix4x4& r);
			Matrix4x4 operator+(const Matrix4x4& r) const;
			Matrix4x4 operator-(const Matrix4x4& r) const;
			Matrix4x4 operator*(const Matrix4x4& r) const;
			Matrix4x1 operator*(const Matrix4x1& r) const;

			Matrix4x4 operator*(const float& r);
			Matrix4x4 operator*=(const float& r);

			// Serializable��(��) ���� ��ӵ�
			virtual void PreSerialize(json& jsonData) const override;

			virtual void PreDeserialize(const json& jsonData) override;

			virtual void PostSerialize(json& jsonData) const override;

			virtual void PostDeserialize(const json& jsonData) override;

			const static Matrix4x4 zero;
			const static Matrix4x4 identity;
		};
	}
}