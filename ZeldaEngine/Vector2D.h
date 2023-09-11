#pragma once

#include "ZeldaEngineAPI.h"

#include "Serializable.h"

namespace ZeldaEngine
{
	namespace ZeldaMath
	{
		class Vector3D;

		class ZELDAENGINE_API Vector2D : public Serializable
		{
		public:
			float x;
			float y;

			Vector2D(float x = 0.0f, float y = 0.0f);
			Vector2D(const Vector2D& vector2d);
			Vector2D(const Vector3D& vector3d);

			// �������͸� ����
			Vector2D Normalize();

			// ������ ũ�⸦ ����
			float Size();

			Vector2D operator+=(const Vector2D& right);
			Vector2D operator-=(const Vector2D& right);

			Vector2D operator+(const Vector2D& right);
			Vector2D operator-(const Vector2D& right);
			Vector2D operator-();
			Vector2D operator*(const float& right);
			Vector2D operator/(const float& right);
			Vector2D operator=(const Vector2D& right);

			bool operator==(const Vector2D& right) const;
			bool operator!=(const Vector2D& right) const;
			bool operator<(const Vector2D& right) const;


			// Serializable��(��) ���� ��ӵ�
			virtual void PreSerialize(json& jsonData) const override;

			virtual void PreDeserialize(const json& jsonData) override;

			virtual void PostSerialize(json& jsonData) const override;

			virtual void PostDeserialize(const json& jsonData) override;

			const static Vector2D zero;
		};
	}
}