#include "pch.h"
#include "Quaternion.h"

namespace ZeldaEngine
{
	namespace ZeldaMath
	{
		const Quaternion Quaternion::identity = Quaternion(1.0f, 0.0f, 0.0f, 0.0f);

		Quaternion::Quaternion(float newW, float newX, float newY, float newZ) : w(newW), x(newX), y(newY), z(newZ)
		{

		}

		ZeldaMath::Matrix4x4 Quaternion::GetTransformMatrix() const
		{
			ZeldaMath::Matrix4x4 matrix;

			// 쿼터니언의 요소를 추출합니다.
			float qx = this->x;
			float qy = this->y;
			float qz = this->z;
			float qw = this->w;

			// 변환 행렬을 계산합니다.
			matrix.m[0][0] = 1.0f - 2.0f * qy * qy - 2.0f * qz * qz;
			matrix.m[0][1] = 2.0f * qx * qy - 2.0f * qz * qw;
			matrix.m[0][2] = 2.0f * qx * qz + 2.0f * qy * qw;
			matrix.m[0][3] = 0.0f;

			matrix.m[1][0] = 2.0f * qx * qy + 2.0f * qz * qw;
			matrix.m[1][1] = 1.0f - 2.0f * qx * qx - 2.0f * qz * qz;
			matrix.m[1][2] = 2.0f * qy * qz - 2.0f * qx * qw;
			matrix.m[1][3] = 0.0f;

			matrix.m[2][0] = 2.0f * qx * qz - 2.0f * qy * qw;
			matrix.m[2][1] = 2.0f * qy * qz + 2.0f * qx * qw;
			matrix.m[2][2] = 1.0f - 2.0f * qx * qx - 2.0f * qy * qy;
			matrix.m[2][3] = 0.0f;

			matrix.m[3][0] = 0.0f;
			matrix.m[3][1] = 0.0f;
			matrix.m[3][2] = 0.0f;
			matrix.m[3][3] = 1.0f;

			return matrix;
		}

		Quaternion Quaternion::AxisAngleQuaternion(Vector3D axis, float angle)
		{
			// 축 벡터를 정규화합니다.
			Vector3D normalizedAxis = axis.Normalize();

			// 회전 각도의 반을 구합니다.
			float halfAngle = angle * 0.5f;

			// 쿼터니언의 요소를 계산합니다.
			float w = cos(halfAngle);
			float sinHalfAngle = sin(halfAngle);
			float x = normalizedAxis.x * sinHalfAngle;
			float y = normalizedAxis.y * sinHalfAngle;
			float z = normalizedAxis.z * sinHalfAngle;

			return Quaternion(w, x, y, z);
		}

		Quaternion Quaternion::RotationMatrix(const Matrix4x4& matrix)
		{
			ZeldaMath::Quaternion quaternion = Quaternion::identity;

			Matrix4x4 tmatrix = matrix.Transpose();

			// Extract the rotation elements from the transformation matrix.
			float trace = tmatrix.m[0][0] + tmatrix.m[1][1] + tmatrix.m[2][2];

			if (trace > 0.0f)
			{
				float S = sqrt(trace + 1.0f) * 2.0f;
				quaternion.w = 0.25f * S;
				quaternion.x = (tmatrix.m[1][2] - tmatrix.m[2][1]) / S;
				quaternion.y = (tmatrix.m[2][0] - tmatrix.m[0][2]) / S;
				quaternion.z = (tmatrix.m[0][1] - tmatrix.m[1][0]) / S;
			}
			else if ((tmatrix.m[0][0] > tmatrix.m[1][1]) && (tmatrix.m[0][0] > tmatrix.m[2][2]))
			{
				float S = sqrt(1.0f + tmatrix.m[0][0] - tmatrix.m[1][1] - tmatrix.m[2][2]) * 2.0f;
				quaternion.w = (tmatrix.m[1][2] - tmatrix.m[2][1]) / S;
				quaternion.x = 0.25f * S;
				quaternion.y = (tmatrix.m[1][0] + tmatrix.m[0][1]) / S;
				quaternion.z = (tmatrix.m[2][0] + tmatrix.m[0][2]) / S;
			}
			else if (tmatrix.m[1][1] > tmatrix.m[2][2])
			{
				float S = sqrt(1.0f + tmatrix.m[1][1] - tmatrix.m[0][0] - tmatrix.m[2][2]) * 2.0f;
				quaternion.w = (tmatrix.m[2][0] - tmatrix.m[0][2]) / S;
				quaternion.x = (tmatrix.m[1][0] + tmatrix.m[0][1]) / S;
				quaternion.y = 0.25f * S;
				quaternion.z = (tmatrix.m[2][1] + tmatrix.m[1][2]) / S;
			}
			else
			{
				float S = sqrt(1.0f + tmatrix.m[2][2] - tmatrix.m[0][0] - tmatrix.m[1][1]) * 2.0f;
				quaternion.w = (tmatrix.m[0][1] - tmatrix.m[1][0]) / S;
				quaternion.x = (tmatrix.m[2][0] + tmatrix.m[0][2]) / S;
				quaternion.y = (tmatrix.m[2][1] + tmatrix.m[1][2]) / S;
				quaternion.z = 0.25f * S;
			}

			// Normalize the quaternion before returning.
			float length = sqrt(quaternion.x * quaternion.x + quaternion.y * quaternion.y + quaternion.z * quaternion.z + quaternion.w * quaternion.w);
			if (length != 0.0f)
			{
				quaternion.x /= length;
				quaternion.y /= length;
				quaternion.z /= length;
				quaternion.w /= length;
			}

			return quaternion;
		}

		Quaternion Quaternion::Normalize() const
		{
			float size = sqrt(x * x + y * y + z * z + w * w);

			assert(size != 0.0f);
			if (size == 0.0f) return Quaternion::identity;

			return Quaternion(w / size, x / size, y / size, z / size);
		}

		Quaternion Quaternion::operator+(const Quaternion& other) const
		{
			return Quaternion(w + other.w, x + other.x, y + other.y, z + other.z);
		}

		Quaternion Quaternion::operator-(const Quaternion& other) const
		{
			return Quaternion(w - other.w, x - other.x, y - other.y, z - other.z);
		}

		Quaternion Quaternion::operator*(const Quaternion& other) const
		{
			float newW = w * other.w - x * other.x - y * other.y - z * other.z;
			float newX = w * other.x + x * other.w + y * other.z - z * other.y;
			float newY = w * other.y - x * other.z + y * other.w + z * other.x;
			float newZ = w * other.z + x * other.y - y * other.x + z * other.w;

			return Quaternion(newW, newX, newY, newZ);
		}

		Quaternion Quaternion::operator*(float scalar) const
		{
			return Quaternion(w * scalar, x * scalar, y * scalar, z * scalar);
		}

		Quaternion Quaternion::operator/(float scalar) const
		{
			return Quaternion(w / scalar, x / scalar, y / scalar, z / scalar);
		}

		void Quaternion::PreSerialize(json& jsonData) const
		{
			jsonData["Quaternion"]["w"] = w;
			jsonData["Quaternion"]["x"] = x;
			jsonData["Quaternion"]["y"] = y;
			jsonData["Quaternion"]["z"] = z;
		}

		void Quaternion::PreDeserialize(const json& jsonData)
		{
			w = jsonData["Quaternion"]["w"];
			x = jsonData["Quaternion"]["x"];
			y = jsonData["Quaternion"]["y"];
			z = jsonData["Quaternion"]["z"];
		}

		void Quaternion::PostSerialize(json& jsonData) const
		{

		}

		void Quaternion::PostDeserialize(const json& jsonData)
		{

		}
	}
}