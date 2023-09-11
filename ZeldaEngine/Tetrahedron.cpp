#include "pch.h"
#include "Tetrahedron.h"

#include "ZeldaMath.h"

namespace ZeldaEngine
{
	using namespace ZeldaMath;

	Tetrahedron::Tetrahedron(ZeldaMath::Vector3D a, ZeldaMath::Vector3D b, ZeldaMath::Vector3D c, ZeldaMath::Vector3D d) : isCenterOfMassUpdated(false)
	{
		vertices[0] = a;
		vertices[1] = b;
		vertices[2] = c;
		vertices[3] = d;

		// a				// c				// b
		indices[0] = 0;		indices[1] = 2;		indices[2] = 1;
		// a				// d				// c
		indices[3] = 0;		indices[4] = 3;		indices[5] = 2;
		// a				// b				// d
		indices[6] = 0;		indices[7] = 1;		indices[8] = 3;
		// b				// c				// d
		indices[9] = 1;		indices[10] = 2;	indices[11] = 3;
	}

	Tetrahedron::~Tetrahedron()
	{

	}

	ZeldaMath::Vector3D Tetrahedron::GetCenterOfMass()
	{
		if (isCenterOfMassUpdated)
		{
			return centerOfMass;
		}

		isCenterOfMassUpdated = true;

		centerOfMass = ZeldaMath::Vector3D(0, 0, 0);

		for (size_t i = 0; i < 4; i++)
		{
			centerOfMass += vertices[i];
		}

		centerOfMass /= 4;
		
		return centerOfMass;
	}

	ZeldaMath::Matrix3x3 Tetrahedron::GetInertiaTensor(ZeldaMath::Matrix4x4 scaleMatrix, float mass, ZeldaMath::Vector3D centerOfMass)
	{
		if (isInertiaTensorUpdated)
		{
			return inertiaTensor;
		}
		
		isInertiaTensorUpdated = true;

		inertiaTensor = ZeldaMath::Matrix3x3::zero;

		ZeldaMath::Vector3D c = centerOfMass;

		for (size_t i = 0; i < 4; i++)
		{
			ZeldaMath::Vector3D v0 = scaleMatrix * vertices[indices[3 * i]];
			ZeldaMath::Vector3D v1 = scaleMatrix * vertices[indices[3 * i + 1]];
			ZeldaMath::Vector3D v2 = scaleMatrix * vertices[indices[3 * i + 2]];

			// 면에 수직이며 다면체의 내부로 향하는 벡터
			ZeldaMath::Vector3D internalNormal = ZeldaMath::CrossProduct(v1 - v0, v2 - v1);

			// 질량중심에서 면위의 한점으로 향하는 벡터
			ZeldaMath::Vector3D centerToVertex = v0 - c;

			// 질량중심이 내부에 있는 경우
			if (ZeldaMath::DotProduct(internalNormal, centerToVertex) > 0)
			{
				float vol = ZeldaMath::Abs(ZeldaMath::ScalarTripleProduct(v0 - c, v1 - c, v2 - c)) * (1.0f / 6.0f);
				Tetrahedron t(c, v0, v1, v2);
				if (vol != 0.0f)
				{
					inertiaTensor += t.GetInertiaTensorCenteredAtA() * vol;
				}
			}
			// 두 벡터가 이루는 각이 90도 이하인 경우 질량중심이 외부에 있으며 두 벡터의 내적이 0보다 크다.
			else
			{
				float vol = ZeldaMath::Abs(ZeldaMath::ScalarTripleProduct(v0 - c, v1 - c, v2 - c)) * (1.0f / 6.0f);
				Tetrahedron t(c, v0, v1, v2);
				if (vol != 0.0f)
				{
					inertiaTensor -= t.GetInertiaTensorCenteredAtA() * vol;
				}
			}
		}

		inertiaTensor *= mass / (ZeldaMath::Abs(ZeldaMath::ScalarTripleProduct(vertices[1] - vertices[0], vertices[2] - vertices[0], vertices[3] - vertices[0])) * (1.0f / 6.0f));

		return inertiaTensor;
	}
	
	ZeldaMath::Matrix3x3 Tetrahedron::GetInertiaTensor(ZeldaMath::Matrix4x4 scaleMatrix, float mass)
	{
		return GetInertiaTensor(scaleMatrix, mass, GetCenterOfMass());
	}
	
	ZeldaMath::Vector3D Tetrahedron::GetFarthestPoint(ZeldaMath::Vector3D dirVec)
	{
		float dotresult = ZeldaMath::DotProduct(vertices[0], dirVec);
		size_t maxindex = 0;

		for (size_t i = 1; i < 4; i++)
		{
			float tmp = ZeldaMath::DotProduct(vertices[i], dirVec);
			if (dotresult < tmp)
			{
				dotresult = tmp;
				maxindex = i;
			}
		}

		return vertices[maxindex];
	}

	std::vector<ZeldaMath::Vector3D> Tetrahedron::GetFarthestPoint(ZeldaMath::Vector3D dirVec, float epsilon)
	{
		std::vector<ZeldaMath::Vector3D> result;

		float dotresult = ZeldaMath::DotProduct(vertices[0], dirVec);
		size_t maxindex = 0;

		for (size_t i = 1; i < 4; i++)
		{
			float tmp = ZeldaMath::DotProduct(vertices[i], dirVec);
			if (dotresult < tmp)
			{
				dotresult = tmp;
				maxindex = i;
			}
		}

		for (size_t i = 0; i < 4; i++)
		{
			float tmp = ZeldaMath::DotProduct(vertices[i], dirVec);
			if (dotresult - epsilon < tmp)
			{
				result.push_back(vertices[i]);
			}
		}

		return result;
	}

	float Tetrahedron::GetVolume()
	{
		return ZeldaMath::ScalarTripleProduct(vertices[1] - vertices[0], vertices[2] - vertices[0], vertices[3] - vertices[0]);
	}

	float Tetrahedron::GetMaxRadius(ZeldaMath::Matrix4x4 transform)
	{
		Vector3D v0 = transform * vertices[0];
		Vector3D v1 = transform * vertices[1];
		Vector3D v2 = transform * vertices[2];
		Vector3D v3 = transform * vertices[3];

		float max01 = Max(v0.Size(), v1.Size());
		float max23 = Max(v2.Size(), v3.Size());

		return Max(max01, max23);
	}

	ZeldaMath::Matrix3x3 Tetrahedron::GetInertiaTensorCenteredAtA()
	{
		Matrix3x3 result = Matrix3x3::zero;

		Vector3D v1 = vertices[0] - vertices[0];
		Vector3D v2 = vertices[1] - vertices[0];
		Vector3D v3 = vertices[2] - vertices[0];
		Vector3D v4 = vertices[3] - vertices[0];

		float x1 = v1.x;	float y1 = v1.y;	float z1 = v1.z;
		float x2 = v2.x;	float y2 = v2.y;	float z2 = v2.z;
		float x3 = v3.x;	float y3 = v3.y;	float z3 = v3.z;
		float x4 = v4.x;	float y4 = v4.y;	float z4 = v4.z;

		float a = x2 - x1;	float b = x3 - x1;	float c = x4 - x1;
		float d = y2 - y1;	float e = y3 - y1;	float f = y4 - y1;
		float g = z2 - z1;	float h = z3 - z1;	float i = z4 - z1;

		float determinantJ = (a * e * i) + (b * f * g) + (c * d * h) - (c * e * g) - (b * d * i) - (a * f * h);

		// 물체의 밀도
		float u = 10.0f;

		float A = u * determinantJ * (
			(y1 * y1) + (y1 * y2) + (y2 * y2) + (y1 * y3) + (y2 * y3) + (y3 * y3) + (y1 * y4) + (y2 * y4) + (y3 * y4) + (y4 * y4) +
			(z1 * z1) + (z1 * z2) + (z2 * z2) + (z1 * z3) + (z2 * z3) + (z3 * z3) + (z1 * z4) + (z2 * z4) + (z3 * z4) + (z4 * z4)) / 60.0f;

		float B = u * determinantJ * (
			(x1 * x1) + (x1 * x2) + (x2 * x2) + (x1 * x3) + (x2 * x3) + (x3 * x3) + (x1 * x4) + (x2 * x4) + (x3 * x4) + (x4 * x4) +
			(z1 * z1) + (z1 * z2) + (z2 * z2) + (z1 * z3) + (z2 * z3) + (z3 * z3) + (z1 * z4) + (z2 * z4) + (z3 * z4) + (z4 * z4)) / 60.0f;

		float C = u * determinantJ * (
			(x1 * x1) + (x1 * x2) + (x2 * x2) + (x1 * x3) + (x2 * x3) + (x3 * x3) + (x1 * x4) + (x2 * x4) + (x3 * x4) + (x4 * x4) +
			(y1 * y1) + (y1 * y2) + (y2 * y2) + (y1 * y3) + (y2 * y3) + (y3 * y3) + (y1 * y4) + (y2 * y4) + (y3 * y4) + (y4 * y4)) / 60.0f;

		float A_ = u * determinantJ * (
			(2 * y1 * z1) + (y2 * z1) + (y3 * z1) + (y4 * z1) + (y1 * z2) +
			(2 * y2 * z2) + (y3 * z2) + (y4 * z2) + (y1 * z3) + (y2 * z3) +
			(2 * y3 * z3) + (y4 * z3) + (y1 * z4) + (y2 * z4) + (y3 * z4) +
			(2 * y4 * z4)) / 120.0f;

		float B_ = u * determinantJ * (
			(2 * x1 * z1) + (x2 * z1) + (x3 * z1) + (x4 * z1) + (x1 * z2) +
			(2 * x2 * z2) + (x3 * z2) + (x4 * z2) + (x1 * z3) + (x2 * z3) +
			(2 * x3 * z3) + (x4 * z3) + (x1 * z4) + (x2 * z4) + (x3 * z4) +
			(2 * x4 * z4)) / 120.0f;

		float C_ = u * determinantJ * (
			(2 * x1 * y1) + (x2 * y1) + (x3 * y1) + (x4 * y1) + (x1 * y2) +
			(2 * x2 * y2) + (x3 * y2) + (x4 * y2) + (x1 * y3) + (x2 * y3) +
			(2 * x3 * y3) + (x4 * y3) + (x1 * y4) + (x2 * y4) + (x3 * y4) +
			(2 * x4 * y4)) / 120.0f;

		result.m1_1 = A;
		result.m1_2 = -B_;
		result.m1_3 = -C_;

		result.m2_1 = -B_;
		result.m2_2 = B;
		result.m2_3 = -A_;

		result.m3_1 = -C_;
		result.m3_2 = -A_;
		result.m3_3 = C;

		return result;
	}

	void Tetrahedron::PreSerialize(json& jsonData) const
	{
		// type
		jsonData["Shape"]["type"] = "Tetrahedron";

		// vertices
		for (size_t i = 0; i < 4; i++)
		{
			vertices[i].PreSerialize(jsonData["Shape"]["vertices"][std::to_string(i)]);
		}

		// indices
		for (size_t i = 0; i < 12; i++)
		{
			jsonData["Shape"]["indices"][std::to_string(i)] = indices[i];
		}
	}

	void Tetrahedron::PreDeserialize(const json& jsonData)
	{
		// vertices
		for (size_t i = 0; i < 4; i++)
		{
			vertices[i].PreDeserialize(jsonData["Shape"]["vertices"][std::to_string(i)]);
		}

		// indices
		for (size_t i = 0; i < 12; i++)
		{
			indices[i] = jsonData["Shape"]["indices"][std::to_string(i)];
		}
	}

	void Tetrahedron::PostSerialize(json& jsonData) const
	{
		// vertices
		for (size_t i = 0; i < 4; i++)
		{
			vertices[i].PostSerialize(jsonData["Shape"]["vertices"][std::to_string(i)]);
		}
	}

	void Tetrahedron::PostDeserialize(const json& jsonData)
	{
		// vertices
		for (size_t i = 0; i < 4; i++)
		{
			vertices[i].PostDeserialize(jsonData["Shape"]["vertices"][std::to_string(i)]);
		}
	}
}