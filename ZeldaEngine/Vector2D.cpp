#include "pch.h"
#include "Vector2D.h"

#include "Vector3D.h"

#include <cmath>

namespace ZeldaEngine
{
	namespace ZeldaMath
	{
		const Vector2D Vector2D::zero = Vector2D(0.0f, 0.0f);

		Vector2D::Vector2D(float x, float y) : x(x), y(y)
		{

		}

		Vector2D::Vector2D(const Vector2D& vector2d) : x(vector2d.x), y(vector2d.y)
		{

		}

		Vector2D::Vector2D(const Vector3D& vector3d) : x(vector3d.x), y(vector3d.y)
		{

		}

		Vector2D Vector2D::Normalize()
		{
			float length = sqrt((x * x) + (y * y));

			if (length == 0.0f)
			{
				return 0.0f;
			}

			return Vector2D(x / length, y / length);
		}

		float Vector2D::Size()
		{
			return sqrt((x * x) + (y * y));
		}

		Vector2D Vector2D::operator+=(const Vector2D& right)
		{
			this->x += right.x;
			this->y += right.y;

			return *this;
		}

		Vector2D Vector2D::operator-=(const Vector2D& right)
		{
			this->x -= right.x;
			this->y -= right.y;

			return *this;
		}

		Vector2D Vector2D::operator+(const Vector2D& right)
		{
			return Vector2D(x + right.x, y + right.y);
		}

		Vector2D Vector2D::operator-(const Vector2D& right)
		{
			return Vector2D(x - right.x, y - right.y);
		}

		Vector2D Vector2D::operator-()
		{
			return Vector2D(-x, -y);
		}

		Vector2D Vector2D::operator*(const float& right)
		{
			return Vector2D(x * right, y * right);
		}

		Vector2D Vector2D::operator/(const float& right)
		{
			return Vector2D(x / right, y / right);
		}

		Vector2D Vector2D::operator=(const Vector2D& right)
		{
			x = right.x;
			y = right.y;

			return *this;
		}

		bool Vector2D::operator==(const Vector2D& right) const
		{
			return (x == right.x) && (y == right.y);
		}

		bool Vector2D::operator!=(const Vector2D& right) const
		{
			return (x != right.x) || (y != right.y);
		}

		bool Vector2D::operator<(const Vector2D& right) const
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

		void Vector2D::PreSerialize(json& jsonData) const
		{
			jsonData["Vector2D"]["x"] = x;
			jsonData["Vector2D"]["y"] = y;
		}
		void Vector2D::PreDeserialize(const json& jsonData)
		{
			x = jsonData["Vector2D"]["x"];
			y = jsonData["Vector2D"]["y"];
		}

		void Vector2D::PostSerialize(json& jsonData) const
		{

		}

		void Vector2D::PostDeserialize(const json& jsonData)
		{

		}
	}
}