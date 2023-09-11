#include "pch.h"
#include "Matrix4x1.h"

#include "Vector3D.h"

namespace ZeldaEngine
{
	namespace ZeldaMath
	{
		Matrix4x1::Matrix4x1(float x, float y, float z, float w) : m1_1(x), m2_1(y), m3_1(z), m4_1(w)
		{

		}

		Matrix4x1::Matrix4x1(const Vector3D& vector3d) : m1_1(vector3d.x), m2_1(vector3d.y), m3_1(vector3d.z), m4_1(1.0f)
		{

		}

		Matrix4x1::Matrix4x1(const Matrix4x1& matrix4x1) : m1_1(matrix4x1.m1_1), m2_1(matrix4x1.m2_1), m3_1(matrix4x1.m3_1), m4_1(matrix4x1.m4_1)
		{

		}

		Matrix4x1 Matrix4x1::operator=(const Vector3D& right)
		{
			m1_1 = right.x;
			m2_1 = right.y;
			m3_1 = right.z;
			m4_1 = 1.0f;
			return *this;
		}

		void Matrix4x1::PreSerialize(json& jsonData) const
		{
			jsonData["m1_1"] = m1_1;
			jsonData["m2_1"] = m2_1;
			jsonData["m3_1"] = m3_1;
			jsonData["m4_1"] = m4_1;
		}

		void Matrix4x1::PreDeserialize(const json& jsonData)
		{
			m1_1 = jsonData["m1_1"];
			m2_1 = jsonData["m2_1"];
			m3_1 = jsonData["m3_1"];
			m4_1 = jsonData["m4_1"];
		}

		void Matrix4x1::PostSerialize(json& jsonData) const
		{

		}

		void Matrix4x1::PostDeserialize(const json& jsonData)
		{

		}
	}
}