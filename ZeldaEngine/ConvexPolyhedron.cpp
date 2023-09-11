#include "pch.h"
#include "ConvexPolyhedron.h"

#include "ZeldaMath.h"
#include "Tetrahedron.h"

#include <string>

namespace ZeldaEngine
{
	using namespace ZeldaMath;

	ConvexPolyhedron::ConvexPolyhedron(const std::vector<ZeldaMath::Vector3D>& vtx, const std::vector<int>& idx) : isCenterOfMassUpdated(false), isInertiaTensorUpdated(false), vertices(vtx), indices(idx)
	{
	}

	ConvexPolyhedron::~ConvexPolyhedron()
	{

	}

	ZeldaMath::Vector3D ZeldaEngine::ConvexPolyhedron::GetCenterOfMass()
	{
		if (isCenterOfMassUpdated)
		{
			return centerOfMass;
		}

		isCenterOfMassUpdated = true;

		centerOfMass = ZeldaMath::Vector3D(0, 0, 0);

		if (vertices.size() != 0)
		{
			for (size_t i = 0; i < vertices.size(); i++)
			{
				centerOfMass += vertices[i];
			}

			centerOfMass /= static_cast<float>(vertices.size());
		}

		return centerOfMass;
	}

	ZeldaMath::Matrix3x3 ZeldaEngine::ConvexPolyhedron::GetInertiaTensor(ZeldaMath::Matrix4x4 scaleMatrix, float mass, ZeldaMath::Vector3D centerOfMass)
	{
		if (isInertiaTensorUpdated)
		{
			return inertiaTensor;
		}

		isInertiaTensorUpdated = true;

		inertiaTensor = ZeldaMath::Matrix3x3::zero;

		float vol = 0.0f;

		for (size_t i = 0; i + 2 < indices.size(); i += 3)
		{
			Vector3D v0 = scaleMatrix * vertices[indices[i]];
			Vector3D v1 = scaleMatrix * vertices[indices[i + 1]];
			Vector3D v2 = scaleMatrix * vertices[indices[i + 2]];
			Vector3D c = centerOfMass;

			// 면에 수직이며 다면체의 내부로 향하는 벡터
			ZeldaMath::Vector3D internalNormal = ZeldaMath::CrossProduct(v1 - v0, v2 - v1);

			// 질량중심에서 면위의 한점으로 향하는 벡터
			ZeldaMath::Vector3D centerToVertex = v0 - c;

			// 질량중심이 내부에 있는 경우
			if (ZeldaMath::DotProduct(internalNormal, centerToVertex) > 0)
			{
				float currentVolume = ZeldaMath::Abs(ZeldaMath::ScalarTripleProduct(v0 - c, v1 - c, v2 - c)) * (1.0f / 6.0f);
				vol += currentVolume;
				Tetrahedron t(c, v0, v1, v2);
				inertiaTensor += t.GetInertiaTensorCenteredAtA();
			}
			// 두 벡터가 이루는 각이 90도 이하인 경우 질량중심이 외부에 있으며 두 벡터의 내적이 0보다 크다.
			else
			{
				float currentVolume = ZeldaMath::Abs(ZeldaMath::ScalarTripleProduct(v0 - c, v1 - c, v2 - c)) * (1.0f / 6.0f);
				vol -= currentVolume;
				Tetrahedron t(c, v0, v1, v2);
				inertiaTensor -= t.GetInertiaTensorCenteredAtA();
			}
		}

		if (vol != 0.0f)
		{
			inertiaTensor *= mass / vol;
		}

		return inertiaTensor;
	}

	ZeldaMath::Matrix3x3 ZeldaEngine::ConvexPolyhedron::GetInertiaTensor(ZeldaMath::Matrix4x4 scaleMatrix, float mass)
	{
		return GetInertiaTensor(scaleMatrix, mass, GetCenterOfMass());
	}

	ZeldaMath::Vector3D ZeldaEngine::ConvexPolyhedron::GetFarthestPoint(ZeldaMath::Vector3D dirVec)
	{
		if (vertices.size() > 0)
		{
			float dotresult = ZeldaMath::DotProduct(vertices[0], dirVec);
			size_t maxindex = 0;

			for (size_t i = 1; i < vertices.size(); i++)
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

		assert(vertices.size() > 0);

		return ZeldaMath::Vector3D::zero;
	}

	std::vector<ZeldaMath::Vector3D> ConvexPolyhedron::GetFarthestPoint(ZeldaMath::Vector3D dirVec, float epsilon)
	{
		assert(vertices.size() > 0);

		std::vector<ZeldaMath::Vector3D> result;

		float dotresult = ZeldaMath::DotProduct(vertices[0], dirVec);
		size_t maxindex = 0;

		for (size_t i = 1; i < vertices.size(); i++)
		{
			float tmp = ZeldaMath::DotProduct(vertices[i], dirVec);
			if (dotresult < tmp)
			{
				dotresult = tmp;
				maxindex = i;
			}
		}

		for (size_t i = 0; i < vertices.size(); i++)
		{
			float tmp = ZeldaMath::DotProduct(vertices[i], dirVec);
			if (dotresult - epsilon < tmp)
			{
				result.push_back(vertices[i]);
			}
		}

		return result;
	}

	float ConvexPolyhedron::GetVolume()
	{
		float v = 0.0f;

		Vector3D center = GetCenterOfMass();

		for (size_t i = 0; i < indices.size(); i += 3)
		{
			Vector3D v1 = vertices[indices[i]];
			Vector3D v2 = vertices[indices[i + 1]];
			Vector3D v3 = vertices[indices[i + 2]];

			v += ScalarTripleProduct(v1 - center, v2 - center, v3 - center);
		}

		return v;
	}

	float ConvexPolyhedron::GetMaxRadius(ZeldaMath::Matrix4x4 transform)
	{
		float maxRadius = 0.0f;
		for (size_t i = 0; i < vertices.size(); i++)
		{
			Vector3D v = transform * vertices[i];
			float radius = v.Size();
			if (radius > maxRadius)
			{
				maxRadius = radius;
			}
		}

		return maxRadius;
	}

	void ConvexPolyhedron::PreSerialize(json& jsonData) const
	{
		// type
		jsonData["Shape"]["type"] = "ConvexPolyhedron";

		// vertices
		jsonData["Shape"]["vertices"]["size"] = vertices.size();
		for (size_t i = 0; i < vertices.size(); i++)
		{
			vertices[i].PreSerialize(jsonData["Shape"]["vertices"][std::to_string(i)]);
		}

		// indices
		jsonData["Shape"]["indices"]["size"] = indices.size();
		for (size_t i = 0; i < indices.size(); i++)
		{
			jsonData["Shape"]["indices"][std::to_string(i)] = indices[i];
		}
	}

	void ConvexPolyhedron::PreDeserialize(const json& jsonData)
	{
		// vertices
		size_t verticesSize = jsonData["Shape"]["vertices"]["size"];
		vertices.clear();
		for (size_t i = 0; i < verticesSize; i++)
		{
			ZeldaMath::Vector3D v;
			v.PreDeserialize(jsonData["Shape"]["vertices"][std::to_string(i)]);
			vertices.push_back(v);
		}

		// indices
		size_t indicesSize = jsonData["Shape"]["indices"]["size"];
		indices.clear();
		for (size_t i = 0; i < indicesSize; i++)
		{
			indices.push_back(jsonData["Shape"]["indices"][std::to_string(i)]);
		}
	}

	void ConvexPolyhedron::PostSerialize(json& jsonData) const
	{
		// vertices
		for (size_t i = 0; i < vertices.size(); i++)
		{
			vertices[i].PostSerialize(jsonData["Shape"]["vertices"][std::to_string(i)]);
		}
	}

	void ConvexPolyhedron::PostDeserialize(const json& jsonData)
	{
		// vertices
		for (size_t i = 0; i < vertices.size(); i++)
		{
			vertices[i].PostDeserialize(jsonData["Shape"]["vertices"][std::to_string(i)]);
		}
	}
}