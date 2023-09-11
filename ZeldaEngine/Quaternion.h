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

            // 생성자: 주어진 값들로 쿼터니언을 초기화합니다.
            Quaternion(float newW, float newX, float newY, float newZ);

            Matrix4x4 GetTransformMatrix() const;

            Quaternion operator+(const Quaternion& other) const;
            Quaternion operator-(const Quaternion& other) const;
            Quaternion operator*(const Quaternion& other) const;

            Quaternion operator*(float scalar) const;
            Quaternion operator/(float scalar) const;

            // axis, angle로 쿼터니언을 얻음
            static Quaternion AxisAngleQuaternion(Vector3D axis, float angle);
            static Quaternion RotationMatrix(const Matrix4x4& matrix);

            Quaternion Normalize() const;

            const static Quaternion identity;

        public:
            // Serializable을(를) 통해 상속됨
            virtual void PreSerialize(json& jsonData) const override;
            virtual void PreDeserialize(const json& jsonData) override;
            virtual void PostSerialize(json& jsonData) const override;
            virtual void PostDeserialize(const json& jsonData) override;
        };
    }
}