#pragma once

#include "ZeldaEngineAPI.h"

#include "Matrix3x3.h"
#include "Matrix4x1.h"
#include "Matrix4x4.h"
#include "Vector2D.h"
#include "Vector3D.h"
#include "Quaternion.h"

namespace ZeldaEngine
{
	namespace ZeldaMath
	{
		ZELDAENGINE_API extern const float PI;

		ZELDAENGINE_API void ScalarSinCos(float* pSin, float* pCos, float  Value);
		ZELDAENGINE_API bool ScalarNearEqual(float S1, float S2, float Epsilon);
		ZELDAENGINE_API Matrix4x4 MatrixPerspectiveFovLH(float FovAngleY, float AspectRatio, float NearZ, float FarZ);

		ZELDAENGINE_API float DegreeToRadian(float degree);
		ZELDAENGINE_API float RadianToDegree(float radian);

		ZELDAENGINE_API float DotProduct(const Vector2D& v0, const Vector2D& v1);
		ZELDAENGINE_API float DotProduct(const Vector3D& v0, const Vector3D& v1);
		ZELDAENGINE_API Vector3D CrossProduct(const Vector3D& v0, const Vector3D& v1);

		ZELDAENGINE_API void DecomposeMatrix4x4(const Matrix4x4& matrix4x4, Vector3D* position, Quaternion* quaternion, Vector3D* scale, bool positiveScaleX = true, bool positiveScaleY = true, bool positiveScaleZ = true);

		// ������ ������
		ZELDAENGINE_API bool IntersectionOfProjetedLineSegment(const Vector3D& A, const Vector3D& B, const Vector3D& C, const Vector3D& D, const Vector3D& dirVec, Vector3D* result);

		// P�� ��鿡 ���翵�� ���� ���ϴ� �Լ�
		ZELDAENGINE_API Vector3D ProjectPointOntoPlane(const Vector3D& P, const Vector3D& planeNormal, const Vector3D& pointOnPlane);

		// ������ ����� ������ ���ϱ�
		ZELDAENGINE_API Vector3D IntersectionOfLineAndPlane(const Vector3D& normal, const Vector3D& planeDot, const Vector3D& A, const Vector3D& B);

		// v0 �� (v1 �� v2)
		ZELDAENGINE_API float ScalarTripleProduct(const Vector3D& v0, const Vector3D& v1, const Vector3D& v2);
		// v0 �� (v1 �� v2)
		ZELDAENGINE_API Vector3D VectorTripleProduct(const Vector3D& v0, const Vector3D& v1, const Vector3D& v2);

		ZELDAENGINE_API Vector3D QuaternionToEuler(const Quaternion& quaternion);
		ZELDAENGINE_API Quaternion EulerToQuaternion(const Vector3D& euler);

		// �� ���� ���̸� t ������ �����ϴ� �Լ�
		ZELDAENGINE_API Vector3D Lerp(const Vector3D& v1, const Vector3D& v2, float t);

		// �� ���ʹϾ� ���̸� t ������ �����ϴ� �Լ�
		ZELDAENGINE_API Quaternion Slerp(const Quaternion& q1, const Quaternion& q2, float t);

		ZELDAENGINE_API double Abs(double value);
		ZELDAENGINE_API float Abs(float value);
		ZELDAENGINE_API int Abs(int value);

		ZELDAENGINE_API double Max(double valueA, double valueB);
		ZELDAENGINE_API float Max(float valueA, float valueB);
		ZELDAENGINE_API int Max(int valueA, int valueB);

		ZELDAENGINE_API double Min(double valueA, double valueB);
		ZELDAENGINE_API float Min(float valueA, float valueB);
		ZELDAENGINE_API int Min(int valueA, int valueB);
	}
}