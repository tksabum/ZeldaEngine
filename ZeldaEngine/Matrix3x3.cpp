#include "pch.h"
#include "Matrix3x3.h"

#include "Vector3D.h"

namespace ZeldaEngine
{
	namespace ZeldaMath
	{
		const Matrix3x3 Matrix3x3::zero = Matrix3x3(
			0, 0, 0,
			0, 0, 0,
			0, 0, 0
		);

		const Matrix3x3 Matrix3x3::identity = Matrix3x3(
			1, 0, 0,
			0, 1, 0,
			0, 0, 1
		);

		Matrix3x3::Matrix3x3() :
			m1_1(1), m1_2(0), m1_3(0),
			m2_1(0), m2_2(1), m2_3(0),
			m3_1(0), m3_2(0), m3_3(1)
		{

		}

		Matrix3x3::Matrix3x3(
			float m1_1, float m1_2, float m1_3,
			float m2_1, float m2_2, float m2_3,
			float m3_1, float m3_2, float m3_3) :
			m1_1(m1_1), m1_2(m1_2), m1_3(m1_3),
			m2_1(m2_1), m2_2(m2_2), m2_3(m2_3),
			m3_1(m3_1), m3_2(m3_2), m3_3(m3_3)
		{

		}

		Matrix3x3 Matrix3x3::Inverse() const
		{
			Matrix3x3 inv;

			float det =
				m1_1 * (m2_2 * m3_3 - m2_3 * m3_2) -
				m1_2 * (m2_1 * m3_3 - m2_3 * m3_1) +
				m1_3 * (m2_1 * m3_2 - m2_2 * m3_1);

			if (std::abs(det) < 1e-8)
			{
				return zero;
			}

			float invDet = 1.0f / det;

			inv.m1_1 = (m2_2 * m3_3 - m2_3 * m3_2) * invDet;
			inv.m1_2 = -(m1_2 * m3_3 - m1_3 * m3_2) * invDet;
			inv.m1_3 = (m1_2 * m2_3 - m1_3 * m2_2) * invDet;

			inv.m2_1 = -(m2_1 * m3_3 - m2_3 * m3_1) * invDet;
			inv.m2_2 = (m1_1 * m3_3 - m1_3 * m3_1) * invDet;
			inv.m2_3 = -(m1_1 * m2_3 - m1_3 * m2_1) * invDet;

			inv.m3_1 = (m2_1 * m3_2 - m2_2 * m3_1) * invDet;
			inv.m3_2 = -(m1_1 * m3_2 - m1_2 * m3_1) * invDet;
			inv.m3_3 = (m1_1 * m2_2 - m1_2 * m2_1) * invDet;

			return inv;
		}

		Matrix3x3 Matrix3x3::operator+=(const Matrix3x3& r)
		{
			this->m1_1 += r.m1_1;	this->m1_2 += r.m1_2;	this->m1_3 += r.m1_3;
			this->m2_1 += r.m2_1;	this->m2_2 += r.m2_2;	this->m2_3 += r.m2_3;
			this->m3_1 += r.m3_1;	this->m3_2 += r.m3_2;	this->m3_3 += r.m3_3;

			return *this;
		}

		Matrix3x3 Matrix3x3::operator-=(const Matrix3x3& r)
		{
			this->m1_1 -= r.m1_1;	this->m1_2 -= r.m1_2;	this->m1_3 -= r.m1_3;
			this->m2_1 -= r.m2_1;	this->m2_2 -= r.m2_2;	this->m2_3 -= r.m2_3;
			this->m3_1 -= r.m3_1;	this->m3_2 -= r.m3_2;	this->m3_3 -= r.m3_3;

			return *this;
		}

		Matrix3x3 Matrix3x3::operator+(const Matrix3x3& r) const
		{
			return Matrix3x3(
				m1_1 + r.m1_1, m1_2 + r.m1_2, m1_3 + r.m1_3,
				m2_1 + r.m2_1, m2_2 + r.m2_2, m2_3 + r.m2_3,
				m3_1 + r.m3_1, m3_2 + r.m3_2, m3_3 + r.m3_3
			);
		}

		Matrix3x3 Matrix3x3::operator-(const Matrix3x3& r) const
		{
			return Matrix3x3(
				m1_1 - r.m1_1, m1_2 - r.m1_2, m1_3 - r.m1_3,
				m2_1 - r.m2_1, m2_2 - r.m2_2, m2_3 - r.m2_3,
				m3_1 - r.m3_1, m3_2 - r.m3_2, m3_3 - r.m3_3
			);
		}

		Matrix3x3 Matrix3x3::operator*(const Matrix3x3& r) const
		{
			return Matrix3x3(
				m1_1 * r.m1_1 + m1_2 * r.m2_1 + m1_3 * r.m3_1,
				m1_1 * r.m1_2 + m1_2 * r.m2_2 + m1_3 * r.m3_2,
				m1_1 * r.m1_3 + m1_2 * r.m2_3 + m1_3 * r.m3_3,

				m2_1 * r.m1_1 + m2_2 * r.m2_1 + m2_3 * r.m3_1,
				m2_1 * r.m1_2 + m2_2 * r.m2_2 + m2_3 * r.m3_2,
				m2_1 * r.m1_3 + m2_2 * r.m2_3 + m2_3 * r.m3_3,

				m3_1 * r.m1_1 + m3_2 * r.m2_1 + m3_3 * r.m3_1,
				m3_1 * r.m1_2 + m3_2 * r.m2_2 + m3_3 * r.m3_2,
				m3_1 * r.m1_3 + m3_2 * r.m2_3 + m3_3 * r.m3_3
			);
		}

		Vector3D Matrix3x3::operator*(const Vector3D& r) const
		{
			return Vector3D(
				m1_1 * r.x + m1_2 * r.y + m1_3 * r.z,
				m2_1 * r.x + m2_2 * r.y + m2_3 * r.z,
				m3_1 * r.x + m3_2 * r.y + m3_3 * r.z
			);
		}

		Matrix3x3 Matrix3x3::operator*(const float& r)
		{
			return Matrix3x3(
				m1_1 * r, m1_2 * r, m1_3 * r,
				m2_1 * r, m2_2 * r, m2_3 * r,
				m3_1 * r, m3_2 * r, m3_3 * r
			);
		}

		Matrix3x3 Matrix3x3::operator*=(const float& r)
		{
			this->m1_1 *= r;	this->m1_2 *= r;	this->m1_3 *= r;
			this->m2_1 *= r;	this->m2_2 *= r;	this->m2_3 *= r;
			this->m3_1 *= r;	this->m3_2 *= r;	this->m3_3 *= r;

			return *this;
		}

		Matrix3x3 Matrix3x3::operator/(const float& r)
		{
			return Matrix3x3(
				m1_1 / r, m1_2 / r, m1_3 / r,
				m2_1 / r, m2_2 / r, m2_3 / r,
				m3_1 / r, m3_2 / r, m3_3 / r
			);
		}

		void Matrix3x3::PreSerialize(json& jsonData) const
		{
			jsonData["m1_1"] = m1_1; jsonData["m1_2"] = m1_2; jsonData["m1_3"] = m1_3;
			jsonData["m2_1"] = m2_1; jsonData["m2_2"] = m2_2; jsonData["m2_3"] = m2_3;
			jsonData["m3_1"] = m3_1; jsonData["m3_2"] = m3_2; jsonData["m3_3"] = m3_3;
		}

		void Matrix3x3::PreDeserialize(const json& jsonData)
		{
			m1_1 = jsonData["m1_1"]; m1_2 = jsonData["m1_2"]; m1_3 = jsonData["m1_3"];
			m2_1 = jsonData["m2_1"]; m2_2 = jsonData["m2_2"]; m2_3 = jsonData["m2_3"];
			m3_1 = jsonData["m3_1"]; m3_2 = jsonData["m3_2"]; m3_3 = jsonData["m3_3"];
		}

		void Matrix3x3::PostSerialize(json& jsonData) const
		{

		}

		void Matrix3x3::PostDeserialize(const json& jsonData)
		{

		}
	}
}