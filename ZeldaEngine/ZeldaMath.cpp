#include "pch.h"
#include "ZeldaMath.h"

#include <cmath>

namespace ZeldaEngine
{
	namespace ZeldaMath
	{
		const float PI = static_cast<float>(std::acos(-1.0));

		void ScalarSinCos (float* pSin, float* pCos, float  Value)
		{
			assert(pSin);
			assert(pCos);

			// Map Value to y in [-pi,pi], x = 2*pi*quotient + remainder.
			float quotient = PI * 0.5f * Value;
			if (Value >= 0.0f)
			{
				quotient = static_cast<float>(static_cast<int>(quotient + 0.5f));
			}
			else
			{
				quotient = static_cast<float>(static_cast<int>(quotient - 0.5f));
			}
			float y = Value - 2.0f * PI * quotient;

			// Map y to [-pi/2,pi/2] with sin(y) = sin(Value).
			float sign;
			if (y > PI * 0.5f)
			{
				y = PI - y;
				sign = -1.0f;
			}
			else if (y < -PI * 0.5f)
			{
				y = -PI - y;
				sign = -1.0f;
			}
			else
			{
				sign = +1.0f;
			}

			float y2 = y * y;

			// 11-degree minimax approximation
			*pSin = (((((-2.3889859e-08f * y2 + 2.7525562e-06f) * y2 - 0.00019840874f) * y2 + 0.0083333310f) * y2 - 0.16666667f) * y2 + 1.0f) * y;

			// 10-degree minimax approximation
			float p = ((((-2.6051615e-07f * y2 + 2.4760495e-05f) * y2 - 0.0013888378f) * y2 + 0.041666638f) * y2 - 0.5f) * y2 + 1.0f;
			*pCos = sign * p;
		}

		bool ScalarNearEqual(float S1, float S2, float Epsilon)
		{
			float Delta = S1 - S2;
			return (fabsf(Delta) <= Epsilon);
		}

		Matrix4x4 MatrixPerspectiveFovLH(float FovAngleY, float AspectRatio, float NearZ, float FarZ)
		{
			assert(NearZ > 0.f && FarZ > 0.f);
			assert(!ScalarNearEqual(FovAngleY, 0.0f, 0.00001f * 2.0f));
			assert(!ScalarNearEqual(AspectRatio, 0.0f, 0.00001f));
			assert(!ScalarNearEqual(FarZ, NearZ, 0.00001f));

			float    SinFov;
			float    CosFov;
			ScalarSinCos(&SinFov, &CosFov, 0.5f * FovAngleY);

			float Height = CosFov / SinFov;
			float Width = Height / AspectRatio;
			float fRange = FarZ / (FarZ - NearZ);

			Matrix4x4 M;
			M.m[0][0] = Width;		M.m[0][1] = 0.0f;		M.m[0][2] = 0.0f;		M.m[0][3] = 0.0f;
			M.m[1][0] = 0.0f;		M.m[1][1] = Height;		M.m[1][2] = 0.0f;		M.m[1][3] = 0.0f;
			M.m[2][0] = 0.0f;		M.m[2][1] = 0.0f;		M.m[2][2] = fRange;		M.m[2][3] = -fRange * NearZ;
			M.m[3][0] = 0.0f;		M.m[3][1] = 0.0f;		M.m[3][2] = 1.0f;		M.m[3][3] = 0.0f;

			return M;
		}

		float DegreeToRadian(float degree)
		{
			return (degree * PI) / 180.0f;
		}

		float RadianToDegree(float radian)
		{
			return (radian * 180.0f) / PI;
		}

		float DotProduct(const Vector2D& v0, const Vector2D& v1)
		{
			return (v0.x * v1.x) + (v0.y * v1.y);
		}

		float DotProduct(const Vector3D& v0, const Vector3D& v1)
		{
			return (v0.x * v1.x) + (v0.y * v1.y) + (v0.z * v1.z);
		}

		Vector3D CrossProduct(const Vector3D& v0, const Vector3D& v1)
		{
			float x = v0.y * v1.z - v0.z * v1.y;
			float y = v0.z * v1.x - v0.x * v1.z;
			float z = v0.x * v1.y - v0.y * v1.x;

			return Vector3D(x, y, z);
		}

		void DecomposeMatrix4x4(const Matrix4x4& matrix4x4, Vector3D* position, Quaternion* quaternion, Vector3D* scale, bool positiveScaleX, bool positiveScaleY, bool positiveScaleZ)
		{
			Matrix4x4 matrix = matrix4x4;

			*position = Vector3D(matrix.m1_4, matrix.m2_4, matrix.m3_4);
			matrix.m1_4 = 0.0f;
			matrix.m2_4 = 0.0f;
			matrix.m3_4 = 0.0f;

			Vector3D basisX = Vector3D(matrix.m1_1, matrix.m2_1, matrix.m3_1);
			Vector3D basisY = Vector3D(matrix.m1_2, matrix.m2_2, matrix.m3_2);
			Vector3D basisZ = Vector3D(matrix.m1_3, matrix.m2_3, matrix.m3_3);

			float sizeX = basisX.Size() * (positiveScaleX) ? (1.0f) : (-1.0f);
			float sizeY = basisY.Size() * (positiveScaleY) ? (1.0f) : (-1.0f);
			float sizeZ = basisZ.Size() * (positiveScaleZ) ? (1.0f) : (-1.0f);

			matrix.m1_1 /= sizeX;
			matrix.m2_1 /= sizeX;
			matrix.m3_1 /= sizeX;

			matrix.m1_2 /= sizeY;
			matrix.m2_2 /= sizeY;
			matrix.m3_2 /= sizeY;

			matrix.m1_3 /= sizeZ;
			matrix.m2_3 /= sizeZ;
			matrix.m3_3 /= sizeZ;
			
			*quaternion = Quaternion::RotationMatrix(matrix);

			*scale = Vector3D(sizeX, sizeY, sizeZ);
		}

		bool IntersectionOfProjetedLineSegment(const Vector3D& A, const Vector3D& B, const Vector3D& C, const Vector3D& D, const Vector3D& dirVec, Vector3D* result)
		{
			Vector3D dirNor = dirVec.Normalize();

			Vector3D projectedA = A;
			Vector3D projectedB = ProjectPointOntoPlane(B, dirNor, A);
			Vector3D projectedC = ProjectPointOntoPlane(C, dirNor, A);
			Vector3D projectedD = ProjectPointOntoPlane(D, dirNor, A);

			float denominator = DotProduct((projectedB - projectedA).Normalize(), (projectedD - projectedC).Normalize());
			if (1.0f - Abs(denominator) < 1e-6f) {
				// 두 직선이 평행하거나 일치하는 경우
				return false;
			}

			// 두 선분이 교차하지 않는 경우
			bool intersect1 = DotProduct(CrossProduct((projectedB - projectedA).Normalize(), (projectedC - projectedA).Normalize()), CrossProduct((projectedB - projectedA).Normalize(), (projectedD - projectedA).Normalize())) <= 0.0f;
			bool intersect2 = DotProduct(CrossProduct((projectedD - projectedC).Normalize(), (projectedA - projectedC).Normalize()), CrossProduct((projectedD - projectedC).Normalize(), (projectedB - projectedC).Normalize())) <= 0.0f;

			if (!(intersect1 && intersect2))
			{
				return false;
			}

			Vector3D AB = projectedA - projectedB;
			Vector3D CD = projectedC - projectedD;
			Vector3D line = CrossProduct(AB, CD);
			Vector3D crossLineAB = CrossProduct(line, AB);
			Vector3D crossLineCD = CrossProduct(line, CD);

			*result = IntersectionOfLineAndPlane(crossLineAB, projectedA, projectedC, projectedD);
			return true;
		}


		Vector3D ProjectPointOntoPlane(const Vector3D& P, const Vector3D& planeNormal, const Vector3D& pointOnPlane)
		{
			Vector3D projVec = P - pointOnPlane;
			float distance = DotProduct(projVec, planeNormal);
			return P - planeNormal * distance;
		}

		Vector3D IntersectionOfLineAndPlane(const Vector3D& normal, const Vector3D& planeDot, const Vector3D& A, const Vector3D& B)
		{
			Vector3D nAB = (B - A).Normalize();

			return A + nAB * (DotProduct(normal, planeDot - A) / DotProduct(normal, nAB));
		}

		float ScalarTripleProduct(const Vector3D& v0, const Vector3D& v1, const Vector3D& v2)
		{
			return DotProduct(v0, CrossProduct(v1, v2));
		}

		Vector3D VectorTripleProduct(const Vector3D& v0, const Vector3D& v1, const Vector3D& v2)
		{
			return CrossProduct(v0, CrossProduct(v1, v2));
		}

		Vector3D QuaternionToEuler(const Quaternion& quaternion)
		{
			float w = quaternion.w;
			float x = quaternion.x;
			float y = quaternion.y;
			float z = quaternion.z;

			float ysqr = y * y;

			// roll (x-axis rotation)
			float t0 = +2.0f * (w * x + y * z);
			float t1 = +1.0f - 2.0f * (x * x + ysqr);
			float roll = atan2(t0, t1);

			// pitch (y-axis rotation)
			float t2 = +2.0f * (w * y - z * x);
			t2 = t2 > 1.0f ? 1.0f : t2;
			t2 = t2 < -1.0f ? -1.0f : t2;
			float pitch = asin(t2);

			// yaw (z-axis rotation)
			float t3 = +2.0f * (w * z + x * y);
			float t4 = +1.0f - 2.0f * (ysqr + z * z);
			float yaw = atan2(t3, t4);

			return Vector3D(roll, pitch, yaw);
		}

		Quaternion EulerToQuaternion(const Vector3D& euler)
		{
			float cy = cos(euler.z * 0.5f);
			float sy = sin(euler.z * 0.5f);
			float cp = cos(euler.y * 0.5f);
			float sp = sin(euler.y * 0.5f);
			float cr = cos(euler.x * 0.5f);
			float sr = sin(euler.x * 0.5f);

			float qw = cr * cp * cy + sr * sp * sy;
			float qx = sr * cp * cy - cr * sp * sy;
			float qy = cr * sp * cy + sr * cp * sy;
			float qz = cr * cp * sy - sr * sp * cy;

			return Quaternion(qw, qx, qy, qz);
		}

		Vector3D Lerp(const Vector3D& v1, const Vector3D& v2, float t)
		{
			Vector3D v1v2 = v2 - v1;
			Vector3D v1v2Normal = v1v2.Normalize();
			float size = v1v2.Size();

			return (v1 + (v1v2Normal * size * t));
		}

		Quaternion Slerp(const Quaternion& q1, const Quaternion& q2, float t)
		{
			// Ensure input quaternions are normalized
			Quaternion q1_normalized = q1.Normalize();
			Quaternion q2_normalized = q2.Normalize();

			// Calculate the dot product between the quaternions
			float dot = Min(1.0f, Max(-1.0f, q1_normalized.w * q2_normalized.w + q1_normalized.x * q2_normalized.x + q1_normalized.y * q2_normalized.y + q1_normalized.z * q2_normalized.z));

			// If the dot product is negative, negate one of the quaternions
			if (dot < 0.0f) {
				q2_normalized.w = -q2_normalized.w;
				q2_normalized.x = -q2_normalized.x;
				q2_normalized.y = -q2_normalized.y;
				q2_normalized.z = -q2_normalized.z;
				dot = -dot;
			}

			// Calculate the interpolation factor
			float theta = std::acos(dot);
			float sin_theta = std::sqrt(1.0f - dot * dot);

			if (std::fabs(sin_theta) < 0.001f) {
				// Linear interpolation if quaternions are nearly parallel
				return Quaternion(
					q1_normalized.w + t * (q2_normalized.w - q1_normalized.w),
					q1_normalized.x + t * (q2_normalized.x - q1_normalized.x),
					q1_normalized.y + t * (q2_normalized.y - q1_normalized.y),
					q1_normalized.z + t * (q2_normalized.z - q1_normalized.z)
				).Normalize();
			}
			else {
				// Spherical interpolation
				float coeff1 = std::sin((1.0f - t) * theta) / sin_theta;
				float coeff2 = std::sin(t * theta) / sin_theta;

				return Quaternion(
					coeff1 * q1_normalized.w + coeff2 * q2_normalized.w,
					coeff1 * q1_normalized.x + coeff2 * q2_normalized.x,
					coeff1 * q1_normalized.y + coeff2 * q2_normalized.y,
					coeff1 * q1_normalized.z + coeff2 * q2_normalized.z
				).Normalize();
			}
		}

		double Abs(double value)
		{
			if (value < 0) return -value;
			return value;
		}

		float Abs(float value)
		{
			if (value < 0) return -value;
			return value;
		}

		int Abs(int value)
		{
			if (value < 0) return -value;
			return value;
		}

		double Max(double valueA, double valueB)
		{
			return valueA < valueB ? (valueB) : (valueA);
		}

		float Max(float valueA, float valueB)
		{
			return valueA < valueB ? (valueB) : (valueA);
		}

		int Max(int valueA, int valueB)
		{
			return valueA < valueB ? (valueB) : (valueA);
		}

		double Min(double valueA, double valueB)
		{
			return valueA > valueB ? (valueB) : (valueA);
		}

		float Min(float valueA, float valueB)
		{
			return valueA > valueB ? (valueB) : (valueA);
		}

		int Min(int valueA, int valueB)
		{
			return valueA > valueB ? (valueB) : (valueA);
		}
	}
}