#include "pch.h"
#include "Vector4D.h"

#include "matrix4x1.h"

namespace ZeldaEngine
{
	namespace ZeldaMath
	{
		const Vector4D Vector4D::zero = Vector4D(0, 0, 0, 0);

		Vector4D::Vector4D(float x, float y, float z, float w) : x(x), y(y), z(z), w(w)
		{

		}

		Vector4D::Vector4D(const Vector4D& vector4d) : x(vector4d.x), y(vector4d.y), z(vector4d.z), w(vector4d.w)
		{

		}

		Vector4D::Vector4D(const Matrix4x1& matrix4x1) : x(matrix4x1.m1_1), y(matrix4x1.m2_1), z(matrix4x1.m3_1), w(matrix4x1.m4_1)
		{

		}

		Vector4D Vector4D::Normalize()
		{
			float length = sqrt((x * x) + (y * y) + (z * z) + (w * w));

			if (length == 0.0f)
			{
				return 0.0f;
			}

			return Vector4D(x / length, y / length, z / length, w / length);
		}

		float Vector4D::Size()
		{
			return sqrt((x * x) + (y * y) + (z * z) + (w * w));
		}

		Vector4D Vector4D::operator+=(const Vector4D& right)
		{
			this->x += right.x;
			this->y += right.y;
			this->z += right.z;
			this->w += right.w;

			return *this;
		}

		Vector4D Vector4D::operator-=(const Vector4D& right)
		{
			this->x -= right.x;
			this->y -= right.y;
			this->z -= right.z;
			this->w -= right.w;

			return *this;
		}

		Vector4D Vector4D::operator*=(const float& right)
		{
			this->x *= right;
			this->y *= right;
			this->z *= right;
			this->w *= right;

			return *this;
		}

		Vector4D Vector4D::operator/=(const float& right)
		{
			this->x /= right;
			this->y /= right;
			this->z /= right;
			this->w /= right;

			return *this;
		}

		Vector4D Vector4D::operator+(const Vector4D& right) const
		{
			return Vector4D(x + right.x, y + right.y, z + right.z, w + right.w);
		}

		Vector4D Vector4D::operator-(const Vector4D& right) const
		{
			return Vector4D(x - right.x, y - right.y, z - right.z, w - right.w);
		}

		Vector4D Vector4D::operator-() const
		{
			return Vector4D(-x, -y, -z, -w);
		}

		Vector4D Vector4D::operator*(const float& right) const
		{
			return Vector4D(x * right, y * right, z * right, w * right);
		}

		Vector4D Vector4D::operator/(const float& right) const
		{
			return Vector4D(x / right, y / right, z / right, w / right);
		}

		Vector4D Vector4D::operator=(const Vector4D& right)
		{
			x = right.x;
			y = right.y;
			z = right.z;
			w = right.w;

			return *this;
		}

		bool Vector4D::operator==(const Vector4D& right) const
		{
			return (x == right.x) && (y == right.y) && (z == right.z) && (w == right.w);
		}

		bool Vector4D::operator!=(const Vector4D& right) const
		{
			return (x != right.x) || (y != right.y) || (z != right.z) || (w != right.w);
		}

		bool Vector4D::operator<(const Vector4D& right) const
		{
			if (w < right.w)
			{
				return true;
			}
			else if (w > right.w)
			{
				return false;
			}
			else
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
		}

		void Vector4D::PreSerialize(json& jsonData) const
		{
			jsonData["Vector4D"]["x"] = x;
			jsonData["Vector4D"]["y"] = y;
			jsonData["Vector4D"]["z"] = z;
			jsonData["Vector4D"]["w"] = w;
		}

		void Vector4D::PreDeserialize(const json& jsonData)
		{
			x = jsonData["Vector4D"]["x"];
			y = jsonData["Vector4D"]["y"];
			z = jsonData["Vector4D"]["z"];
			w = jsonData["Vector4D"]["w"];
		}

		void Vector4D::PostSerialize(json& jsonData) const
		{
		}
		void Vector4D::PostDeserialize(const json& jsonData)
		{
		}
	}
}