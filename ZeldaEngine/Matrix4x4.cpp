#include "pch.h"
#include "Matrix4x4.h"

#include "Vector3D.h"
#include "Matrix4x1.h"

#include <cmath>

namespace ZeldaEngine
{
	namespace ZeldaMath
	{
		const Matrix4x4 Matrix4x4::zero = Matrix4x4(
			0, 0, 0, 0,
			0, 0, 0, 0,
			0, 0, 0, 0,
			0, 0, 0, 0
		);

		const Matrix4x4 Matrix4x4::identity = Matrix4x4(
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		);

		Matrix4x4::Matrix4x4() :
			m1_1(1), m1_2(0), m1_3(0), m1_4(0),
			m2_1(0), m2_2(1), m2_3(0), m2_4(0),
			m3_1(0), m3_2(0), m3_3(1), m3_4(0),
			m4_1(0), m4_2(0), m4_3(0), m4_4(1)
		{

		}

		Matrix4x4::Matrix4x4(
			float m1_1, float m1_2, float m1_3, float m1_4,
			float m2_1, float m2_2, float m2_3, float m2_4,
			float m3_1, float m3_2, float m3_3, float m3_4,
			float m4_1, float m4_2, float m4_3, float m4_4) :
			m1_1(m1_1), m1_2(m1_2), m1_3(m1_3), m1_4(m1_4),
			m2_1(m2_1), m2_2(m2_2), m2_3(m2_3), m2_4(m2_4),
			m3_1(m3_1), m3_2(m3_2), m3_3(m3_3), m3_4(m3_4),
			m4_1(m4_1), m4_2(m4_2), m4_3(m4_3), m4_4(m4_4)
		{

		}

		Matrix4x4 Matrix4x4::Inverse() const
		{
			Matrix4x4 inv;

			float det =
				m1_1 * (m2_2 * (m3_3 * m4_4 - m3_4 * m4_3) - m2_3 * (m3_2 * m4_4 - m3_4 * m4_2) + m2_4 * (m3_2 * m4_3 - m3_3 * m4_2)) -
				m1_2 * (m2_1 * (m3_3 * m4_4 - m3_4 * m4_3) - m2_3 * (m3_1 * m4_4 - m3_4 * m4_1) + m2_4 * (m3_1 * m4_3 - m3_3 * m4_1)) +
				m1_3 * (m2_1 * (m3_2 * m4_4 - m3_4 * m4_2) - m2_2 * (m3_1 * m4_4 - m3_4 * m4_1) + m2_4 * (m3_1 * m4_2 - m3_2 * m4_1)) -
				m1_4 * (m2_1 * (m3_2 * m4_3 - m3_3 * m4_2) - m2_2 * (m3_1 * m4_3 - m3_3 * m4_1) + m2_3 * (m3_1 * m4_2 - m3_2 * m4_1));

			if (std::abs(det) < 1e-8)
			{
				// 행렬식이 0에 가까우면 역행렬이 존재하지 않습니다.
				// 여기에서는 단순히 영행렬을 반환합니다.
				return zero;
			}

			float invDet = 1.0f / det;

			inv.m1_1 = (m2_2 * (m3_3 * m4_4 - m3_4 * m4_3) - m2_3 * (m3_2 * m4_4 - m3_4 * m4_2) + m2_4 * (m3_2 * m4_3 - m3_3 * m4_2)) * invDet;
			inv.m1_2 = -(m1_2 * (m3_3 * m4_4 - m3_4 * m4_3) - m1_3 * (m3_2 * m4_4 - m3_4 * m4_2) + m1_4 * (m3_2 * m4_3 - m3_3 * m4_2)) * invDet;
			inv.m1_3 = (m1_2 * (m2_3 * m4_4 - m2_4 * m4_3) - m1_3 * (m2_2 * m4_4 - m2_4 * m4_2) + m1_4 * (m2_2 * m4_3 - m2_3 * m4_2)) * invDet;
			inv.m1_4 = -(m1_2 * (m2_3 * m3_4 - m2_4 * m3_3) - m1_3 * (m2_2 * m3_4 - m2_4 * m3_2) + m1_4 * (m2_2 * m3_3 - m2_3 * m3_2)) * invDet;

			inv.m2_1 = -(m2_1 * (m3_3 * m4_4 - m3_4 * m4_3) - m2_3 * (m3_1 * m4_4 - m3_4 * m4_1) + m2_4 * (m3_1 * m4_3 - m3_3 * m4_1)) * invDet;
			inv.m2_2 = (m1_1 * (m3_3 * m4_4 - m3_4 * m4_3) - m1_3 * (m3_1 * m4_4 - m3_4 * m4_1) + m1_4 * (m3_1 * m4_3 - m3_3 * m4_1)) * invDet;
			inv.m2_3 = -(m1_1 * (m2_3 * m4_4 - m2_4 * m4_3) - m1_3 * (m2_1 * m4_4 - m2_4 * m4_1) + m1_4 * (m2_1 * m4_3 - m2_3 * m4_1)) * invDet;
			inv.m2_4 = (m1_1 * (m2_3 * m3_4 - m2_4 * m3_3) - m1_3 * (m2_1 * m3_4 - m2_4 * m3_1) + m1_4 * (m2_1 * m3_3 - m2_3 * m3_1)) * invDet;

			inv.m3_1 = (m2_1 * (m3_2 * m4_4 - m3_4 * m4_2) - m2_2 * (m3_1 * m4_4 - m3_4 * m4_1) + m2_4 * (m3_1 * m4_2 - m3_2 * m4_1)) * invDet;
			inv.m3_2 = -(m1_1 * (m3_2 * m4_4 - m3_4 * m4_2) - m1_2 * (m3_1 * m4_4 - m3_4 * m4_1) + m1_4 * (m3_1 * m4_2 - m3_2 * m4_1)) * invDet;
			inv.m3_3 = (m1_1 * (m2_2 * m4_4 - m2_4 * m4_2) - m1_2 * (m2_1 * m4_4 - m2_4 * m4_1) + m1_4 * (m2_1 * m4_2 - m2_2 * m4_1)) * invDet;
			inv.m3_4 = -(m1_1 * (m2_2 * m3_4 - m2_4 * m3_2) - m1_2 * (m2_1 * m3_4 - m2_4 * m3_1) + m1_4 * (m2_1 * m3_2 - m2_2 * m3_1)) * invDet;

			inv.m4_1 = -(m2_1 * (m3_2 * m4_3 - m3_3 * m4_2) - m2_2 * (m3_1 * m4_3 - m3_3 * m4_1) + m2_3 * (m3_1 * m4_2 - m3_2 * m4_1)) * invDet;
			inv.m4_2 = (m1_1 * (m3_2 * m4_3 - m3_3 * m4_2) - m1_2 * (m3_1 * m4_3 - m3_3 * m4_1) + m1_3 * (m3_1 * m4_2 - m3_2 * m4_1)) * invDet;
			inv.m4_3 = -(m1_1 * (m2_2 * m4_3 - m2_3 * m4_2) - m1_2 * (m2_1 * m4_3 - m2_3 * m4_1) + m1_3 * (m2_1 * m4_2 - m2_2 * m4_1)) * invDet;
			inv.m4_4 = (m1_1 * (m2_2 * m3_3 - m2_3 * m3_2) - m1_2 * (m2_1 * m3_3 - m2_3 * m3_1) + m1_3 * (m2_1 * m3_2 - m2_2 * m3_1)) * invDet;

			return inv;
		}

		Matrix4x4 Matrix4x4::Transpose() const
		{
			return Matrix4x4(
				m1_1, m2_1, m3_1, m4_1,
				m1_2, m2_2, m3_2, m4_2,
				m1_3, m2_3, m3_3, m4_3,
				m1_4, m2_4, m3_4, m4_4
			);
		}

		Matrix4x4 Matrix4x4::Translation(float x, float y, float z)
		{
			return Matrix4x4
			(
				1.0f, 0.0f, 0.0f, x,
				0.0f, 1.0f, 0.0f, y,
				0.0f, 0.0f, 1.0f, z,
				0.0f, 0.0f, 0.0f, 1.0f
			);
		}

		Matrix4x4 Matrix4x4::Translation(Vector3D movevector)
		{
			return Translation(movevector.x, movevector.y, movevector.z);
		}

		Matrix4x4 Matrix4x4::Rotation(Vector3D axis, float radianangle)
		{
			float cosTheta = cos(radianangle);
			float sinTheta = sin(radianangle);
			float oneMinusCosTheta = 1.0f - cosTheta;
			float x = axis.x;
			float y = axis.y;
			float z = axis.z;

			return Matrix4x4(
				cosTheta + (1 - cosTheta) * x * x, (1 - cosTheta) * x * y - sinTheta * z, (1 - cosTheta) * x * z + sinTheta * y, 0.0f,
				(1 - cosTheta) * x * y + sinTheta * z, cosTheta + (1 - cosTheta) * y * y, (1 - cosTheta) * y * z - sinTheta * x, 0.0f,
				(1 - cosTheta) * x * z - sinTheta * y, (1 - cosTheta) * y * z + sinTheta * x, cosTheta + (1 - cosTheta) * z * z, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f
			);
		}

		Matrix4x4 Matrix4x4::EulerRotation(Vector3D eulerdegreeangle)
		{
			return Rotation(Vector3D(0, 0, 1), eulerdegreeangle.z) * Rotation(Vector3D(0, 1, 0), eulerdegreeangle.y) * Rotation(Vector3D(1, 0, 0), eulerdegreeangle.x);
		}

		Matrix4x4 Matrix4x4::Scale(Vector3D scale)
		{
			return Matrix4x4(
				scale.x, 0.0f, 0.0f, 0.0f,
				0.0f, scale.y, 0.0f, 0.0f,
				0.0f, 0.0f, scale.z, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f
			);
		}

		Matrix4x4 Matrix4x4::operator+=(const Matrix4x4& r)
		{
			this->m1_1 += r.m1_1;	this->m1_2 += r.m1_2;	this->m1_3 += r.m1_3;	this->m1_4 += r.m1_4;
			this->m2_1 += r.m2_1;	this->m2_2 += r.m2_2;	this->m2_3 += r.m2_3;	this->m2_4 += r.m2_4;
			this->m3_1 += r.m3_1;	this->m3_2 += r.m3_2;	this->m3_3 += r.m3_3;	this->m3_4 += r.m3_4;
			this->m4_1 += r.m4_1;	this->m4_2 += r.m4_2;	this->m4_3 += r.m4_3;	this->m4_4 += r.m4_4;

			return *this;
		}

		Matrix4x4 Matrix4x4::operator-=(const Matrix4x4& r)
		{
			this->m1_1 -= r.m1_1;	this->m1_2 -= r.m1_2;	this->m1_3 -= r.m1_3;	this->m1_4 -= r.m1_4;
			this->m2_1 -= r.m2_1;	this->m2_2 -= r.m2_2;	this->m2_3 -= r.m2_3;	this->m2_4 -= r.m2_4;
			this->m3_1 -= r.m3_1;	this->m3_2 -= r.m3_2;	this->m3_3 -= r.m3_3;	this->m3_4 -= r.m3_4;
			this->m4_1 -= r.m4_1;	this->m4_2 -= r.m4_2;	this->m4_3 -= r.m4_3;	this->m4_4 -= r.m4_4;

			return *this;
		}

		Matrix4x4 Matrix4x4::operator+(const Matrix4x4& r) const
		{
			return Matrix4x4(
				m1_1 + r.m1_1, m1_2 + r.m1_2, m1_3 + r.m1_3, m1_4 + r.m1_4,
				m2_1 + r.m2_1, m2_2 + r.m2_2, m2_3 + r.m2_3, m2_4 + r.m2_4,
				m3_1 + r.m3_1, m3_2 + r.m3_2, m3_3 + r.m3_3, m3_4 + r.m3_4,
				m4_1 + r.m4_1, m4_2 + r.m4_2, m4_3 + r.m4_3, m4_4 + r.m4_4
			);
		}

		Matrix4x4 Matrix4x4::operator-(const Matrix4x4& r) const
		{
			return Matrix4x4(
				m1_1 - r.m1_1, m1_2 - r.m1_2, m1_3 - r.m1_3, m1_4 - r.m1_4,
				m2_1 - r.m2_1, m2_2 - r.m2_2, m2_3 - r.m2_3, m2_4 - r.m2_4,
				m3_1 - r.m3_1, m3_2 - r.m3_2, m3_3 - r.m3_3, m3_4 - r.m3_4,
				m4_1 - r.m4_1, m4_2 - r.m4_2, m4_3 - r.m4_3, m4_4 - r.m4_4
			);
		}

		Matrix4x4 Matrix4x4::operator*(const Matrix4x4& r) const
		{
			return Matrix4x4(
				m1_1 * r.m1_1 + m1_2 * r.m2_1 + m1_3 * r.m3_1 + m1_4 * r.m4_1,
				m1_1 * r.m1_2 + m1_2 * r.m2_2 + m1_3 * r.m3_2 + m1_4 * r.m4_2,
				m1_1 * r.m1_3 + m1_2 * r.m2_3 + m1_3 * r.m3_3 + m1_4 * r.m4_3,
				m1_1 * r.m1_4 + m1_2 * r.m2_4 + m1_3 * r.m3_4 + m1_4 * r.m4_4,

				m2_1 * r.m1_1 + m2_2 * r.m2_1 + m2_3 * r.m3_1 + m2_4 * r.m4_1,
				m2_1 * r.m1_2 + m2_2 * r.m2_2 + m2_3 * r.m3_2 + m2_4 * r.m4_2,
				m2_1 * r.m1_3 + m2_2 * r.m2_3 + m2_3 * r.m3_3 + m2_4 * r.m4_3,
				m2_1 * r.m1_4 + m2_2 * r.m2_4 + m2_3 * r.m3_4 + m2_4 * r.m4_4,

				m3_1 * r.m1_1 + m3_2 * r.m2_1 + m3_3 * r.m3_1 + m3_4 * r.m4_1,
				m3_1 * r.m1_2 + m3_2 * r.m2_2 + m3_3 * r.m3_2 + m3_4 * r.m4_2,
				m3_1 * r.m1_3 + m3_2 * r.m2_3 + m3_3 * r.m3_3 + m3_4 * r.m4_3,
				m3_1 * r.m1_4 + m3_2 * r.m2_4 + m3_3 * r.m3_4 + m3_4 * r.m4_4,

				m4_1 * r.m1_1 + m4_2 * r.m2_1 + m4_3 * r.m3_1 + m4_4 * r.m4_1,
				m4_1 * r.m1_2 + m4_2 * r.m2_2 + m4_3 * r.m3_2 + m4_4 * r.m4_2,
				m4_1 * r.m1_3 + m4_2 * r.m2_3 + m4_3 * r.m3_3 + m4_4 * r.m4_3,
				m4_1 * r.m1_4 + m4_2 * r.m2_4 + m4_3 * r.m3_4 + m4_4 * r.m4_4
			);
		}

		Matrix4x1 Matrix4x4::operator*(const Matrix4x1& r) const
		{
			return Matrix4x1(
				m1_1 * r.m1_1 + m1_2 * r.m2_1 + m1_3 * r.m3_1 + m1_4 * r.m4_1,
				m2_1 * r.m1_1 + m2_2 * r.m2_1 + m2_3 * r.m3_1 + m2_4 * r.m4_1,
				m3_1 * r.m1_1 + m3_2 * r.m2_1 + m3_3 * r.m3_1 + m3_4 * r.m4_1,
				m4_1 * r.m1_1 + m4_2 * r.m2_1 + m4_3 * r.m3_1 + m4_4 * r.m4_1
			);
		}

		Matrix4x4 Matrix4x4::operator*(const float& r)
		{
			return Matrix4x4(
				m1_1 * r, m1_2 * r, m1_3 * r, m1_4 * r,
				m2_1 * r, m2_2 * r, m2_3 * r, m2_4 * r,
				m3_1 * r, m3_2 * r, m3_3 * r, m3_4 * r,
				m4_1 * r, m4_2 * r, m4_3 * r, m4_4 * r
			);
		}

		Matrix4x4 Matrix4x4::operator*=(const float& r)
		{
			this->m1_1 *= r;	this->m1_2 *= r;	this->m1_3 *= r;	this->m1_4 *= r;
			this->m2_1 *= r;	this->m2_2 *= r;	this->m2_3 *= r;	this->m2_4 *= r;
			this->m3_1 *= r;	this->m3_2 *= r;	this->m3_3 *= r;	this->m3_4 *= r;
			this->m4_1 *= r;	this->m4_2 *= r;	this->m4_3 *= r;	this->m4_4 *= r;

			return *this;
		}

		void Matrix4x4::PreSerialize(json& jsonData) const
		{
			jsonData["m1_1"] = m1_1; jsonData["m1_2"] = m1_2; jsonData["m1_3"] = m1_3; jsonData["m1_4"] = m1_4;
			jsonData["m2_1"] = m2_1; jsonData["m2_2"] = m2_2; jsonData["m2_3"] = m2_3; jsonData["m2_4"] = m2_4;
			jsonData["m3_1"] = m3_1; jsonData["m3_2"] = m3_2; jsonData["m3_3"] = m3_3; jsonData["m3_4"] = m3_4;
			jsonData["m4_1"] = m4_1; jsonData["m4_2"] = m4_2; jsonData["m4_3"] = m4_3; jsonData["m4_4"] = m4_4;
		}

		void Matrix4x4::PreDeserialize(const json& jsonData)
		{
			m1_1 = jsonData["m1_1"]; m1_2 = jsonData["m1_2"]; m1_3 = jsonData["m1_3"]; m1_4 = jsonData["m1_4"];
			m2_1 = jsonData["m2_1"]; m2_2 = jsonData["m2_2"]; m2_3 = jsonData["m2_3"]; m2_4 = jsonData["m2_4"];
			m3_1 = jsonData["m3_1"]; m3_2 = jsonData["m3_2"]; m3_3 = jsonData["m3_3"]; m3_4 = jsonData["m3_4"];
			m4_1 = jsonData["m4_1"]; m4_2 = jsonData["m4_2"]; m4_3 = jsonData["m4_3"]; m4_4 = jsonData["m4_4"];
		}

		void Matrix4x4::PostSerialize(json& jsonData) const
		{

		}

		void Matrix4x4::PostDeserialize(const json& jsonData)
		{

		}
	}
}