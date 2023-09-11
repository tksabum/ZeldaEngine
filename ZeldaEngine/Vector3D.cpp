#include "pch.h"
#include "Vector3D.h"

#include "Vector2D.h"
#include "Matrix4x1.h"
#include <cmath>

namespace ZeldaEngine
{
	namespace ZeldaMath
	{
		const Vector3D Vector3D::zero = Vector3D(0, 0, 0);
		const Vector3D Vector3D::front = Vector3D(0, 0, 1);
		const Vector3D Vector3D::right = Vector3D(1, 0, 0);
		const Vector3D Vector3D::up = Vector3D(0, 1, 0);

		Vector3D::Vector3D(float x, float y, float z) : x(x), y(y), z(z)
		{

		}

		Vector3D::Vector3D(const Vector3D& vector3d) : x(vector3d.x), y(vector3d.y), z(vector3d.z)
		{

		}

		Vector3D::Vector3D(const Matrix4x1& matrix4x1) : x(matrix4x1.m1_1), y(matrix4x1.m2_1), z(matrix4x1.m3_1)
		{

		}

		Vector3D Vector3D::Normalize() const
		{
			float length = sqrt((x * x) + (y * y) + (z * z));

			if (length == 0.0f)
			{
				return 0.0f;
			}

			return Vector3D(x / length, y / length, z / length);
		}

		float Vector3D::Size() const
		{
			return sqrt((x * x) + (y * y) + (z * z));
		}

		Vector3D Vector3D::operator+=(const Vector3D& right)
		{
			this->x += right.x;
			this->y += right.y;
			this->z += right.z;

			return *this;
		}

		Vector3D Vector3D::operator-=(const Vector3D& right)
		{
			this->x -= right.x;
			this->y -= right.y;
			this->z -= right.z;

			return *this;
		}

		Vector3D Vector3D::operator*=(const float& right)
		{
			this->x *= right;
			this->y *= right;
			this->z *= right;

			return *this;
		}

		Vector3D Vector3D::operator/=(const float& right)
		{
			this->x /= right;
			this->y /= right;
			this->z /= right;

			return *this;
		}

		Vector3D Vector3D::operator+(const Vector3D& right) const
		{
			return Vector3D(x + right.x, y + right.y, z + right.z);
		}

		Vector3D Vector3D::operator-(const Vector3D& right) const
		{
			return Vector3D(x - right.x, y - right.y, z - right.z);
		}

		Vector3D Vector3D::operator-() const
		{
			return Vector3D(-x, -y, -z);
		}

		Vector3D Vector3D::operator*(const float& right) const
		{
			return Vector3D(x * right, y * right, z * right);
		}

		Vector3D Vector3D::operator/(const float& right) const
		{
			return Vector3D(x / right, y / right, z / right);
		}

		Vector3D Vector3D::operator=(const Vector3D& right)
		{
			x = right.x;
			y = right.y;
			z = right.z;

			return *this;
		}

		bool Vector3D::operator==(const Vector3D& right) const
		{
			return (x == right.x) && (y == right.y) && (z == right.z);
		}

		bool Vector3D::operator!=(const Vector3D& right) const
		{
			return (x != right.x) || (y != right.y) || (z != right.z);
		}

		bool Vector3D::operator<(const Vector3D& right) const
		{
			if (z < right.z)
			{
				return true;
			}
			else if (z > right.z)
			{
				return false;
			}
			else
			{
				if (y < right.y)
				{
					return true;
				}
				else if (y > right.y)
				{
					return false;
				}
				else
				{
					if (x < right.x)
					{
						return true;
					}
					else
					{
						return false;
					}
				}
			}
		}

		void Vector3D::PreSerialize(json& jsonData) const
		{
			jsonData["Vector3D"]["x"] = x;
			jsonData["Vector3D"]["y"] = y;
			jsonData["Vector3D"]["z"] = z;
		}

		void Vector3D::PreDeserialize(const json& jsonData)
		{
			x = jsonData["Vector3D"]["x"];
			y = jsonData["Vector3D"]["y"];
			z = jsonData["Vector3D"]["z"];
		}

		void Vector3D::PostSerialize(json& jsonData) const
		{

		}

		void Vector3D::PostDeserialize(const json& jsonData)
		{

		}
	}
}