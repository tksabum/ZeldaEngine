#include "pch.h"
#include "CubeCollider.h"

#include "Shape.h"
#include "Tetrahedron.h"
#include "ConvexPolyhedron.h"

#include "ResourceManager.h"
#include "GameObject.h"
#include "Transform.h"
#include "IGraphics.h"

namespace ZeldaEngine
{
	CubeCollider::CubeCollider()
	{
		colliderShape = new ConvexPolyhedron(
			{
				ZeldaMath::Vector3D(0.5f, 0.5f, 0.5f), ZeldaMath::Vector3D(-0.5f, 0.5f, 0.5f), ZeldaMath::Vector3D(-0.5f, 0.5f, -0.5f), ZeldaMath::Vector3D(0.5f, 0.5f, -0.5f),
				ZeldaMath::Vector3D(0.5f, -0.5f, 0.5f), ZeldaMath::Vector3D(-0.5f, -0.5f, 0.5f), ZeldaMath::Vector3D(-0.5f, -0.5f, -0.5f), ZeldaMath::Vector3D(0.5f, -0.5f, -0.5f)
			},
			{
				0, 2, 1, 2, 0, 3,
				4, 6, 7, 6, 4, 5,
				7, 0, 4, 0, 7, 3,
				5, 2, 6, 2, 5, 1,
				3, 6, 2, 6, 3, 7,
				1, 4, 0, 4, 1, 5
			});
	}
	
	CubeCollider::~CubeCollider()
	{
		delete colliderShape;
	}

	void CubeCollider::Render(IGraphics* graphics)
	{
		static MaterialInfo matrerialInfo = *ResourceManager::GetInstance().GetMaterial(L"BoundaryDebug");

		ZeldaMath::Matrix4x4 worldTM = GetGameObject()->GetTransform()->GetWorldMatrix();
		std::wstring meshPath = ResourceManager::GetInstance().GetMesh(L"Cube");

		graphics->DrawMesh(worldTM, meshPath, matrerialInfo, matrerialInfo.materialName, 0);
	}

	void CubeCollider::PreSerialize(json& jsonData) const
	{
		PRESERIALIZE_BASE(CubeCollider);
		PRESERIALIZE_SERPTR(colliderShape);
		PRESERIALIZE_VALUE(isTrigger);
	}

	void CubeCollider::PreDeserialize(const json& jsonData)
	{
		PREDESERIALIZE_BASE(CubeCollider);
		PREDESERIALIZE_SERPTR(colliderShape);
		PREDESERIALIZE_VALUE(isTrigger);
	}
	void CubeCollider::PostSerialize(json& jsonData) const
	{
		POSTSERIALIZE_BASE();
		POSTSERIALIZE_SERPTR(colliderShape);
	}
	void CubeCollider::PostDeserialize(const json& jsonData)
	{
		POSTDESERIALIZE_BASE();
		POSTDESERIALIZE_SERPTR(colliderShape);
	}
}