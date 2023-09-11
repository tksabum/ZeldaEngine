#pragma once

#include "ZeldaEngineAPI.h"

#include "Matrix4x4.h"
#include "Vector3D.h"

#include "Serializable.h"

namespace ZeldaEngine
{
    namespace ZeldaMath
    {
        class ZELDAENGINE_API Quaternion : public Serializable
        {
        public:
            float w, x, y, z;

            // ������: �־��� ����� ���ʹϾ��� �ʱ�ȭ�մϴ�.
            Quaternion(float newW, float newX, float newY, float newZ);

            Matrix4x4 GetTransformMatrix() const;

            Quaternion operator+(const Quaternion& other) const;
            Quaternion operator-(const Quaternion& other) const;
            Quaternion operator*(const Quaternion& other) const;

            Quaternion operator*(float scalar) const;
            Quaternion operator/(float scalar) const;

            // axis, angle�� ���ʹϾ��� ����
            static Quaternion AxisAngleQuaternion(Vector3D axis, float angle);
            static Quaternion RotationMatrix(const Matrix4x4& matrix);

            Quaternion Normalize() const;

            const static Quaternion identity;

        public:
            // Serializable��(��) ���� ��ӵ�
            virtual void PreSerialize(json& jsonData) const override;
            virtual void PreDeserialize(const json& jsonData) override;
            virtual void PostSerialize(json& jsonData) const override;
            virtual void PostDeserialize(const json& jsonData) override;
        };
    }
}