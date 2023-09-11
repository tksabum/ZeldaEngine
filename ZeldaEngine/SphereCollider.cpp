#include "pch.h"
#include "SphereCollider.h"

#include "Shape.h"
#include "Tetrahedron.h"
#include "ConvexPolyhedron.h"

#include "ZeldaMath.h"

#include "ResourceManager.h"
#include "GameObject.h"
#include "Transform.h"
#include "IGraphics.h"

namespace ZeldaEngine
{
	SphereCollider::SphereCollider()
	{
		float radius = 0.5f; // ±∏¿« π›¡ˆ∏ß
		int stackCount = 20; // ∞°∑Œ ∫–«“
		int sliceCount = 20; // ºº∑Œ ∫–«“

		std::vector<ZeldaMath::Vector3D> v;

		// ∫œ±ÿ
		v.push_back(ZeldaMath::Vector3D(0.0f, radius, 0.0f));

		float stackAngle = ZeldaMath::PI / static_cast<float>(stackCount);
		float sliceAngle = 2 * ZeldaMath::PI / static_cast<float>(sliceCount);

		float deltaU = 1.f / static_cast<float>(sliceCount);
		float deltaV = 1.f / static_cast<float>(stackCount);

		// ∞Ì∏Æ∏∂¥Ÿ µπ∏Èº≠ ¡§¡°¿ª ∞ËªÍ«—¥Ÿ (∫œ±ÿ/≥≤±ÿ ¥‹¿œ¡°¿∫ ∞Ì∏Æ∞° X)
		for (size_t y = 1; y <= stackCount - 1; ++y)
		{
			float phi = y * stackAngle;

			// ∞Ì∏Æø° ¿ßƒ°«— ¡§¡°
			for (size_t x = 0; x <= sliceCount; ++x)
			{
				float theta = x * sliceAngle;

				v.push_back(ZeldaMath::Vector3D(radius * sinf(phi) * cosf(theta), radius * cosf(phi), radius * sinf(phi) * sinf(theta)));
			}
		}

		// ≥≤±ÿ
		v.push_back(ZeldaMath::Vector3D(0.0f, -radius, 0.0f));

		std::vector<int> idx(36);

		// ∫œ±ÿ ¿Œµ¶Ω∫
		for (int i = 0; i <= sliceCount; ++i)
		{
			//  [0]
			//   |  \
			//  [i+1]-[i+2]
			idx.push_back(0);
			idx.push_back(i + 2);
			idx.push_back(i + 1);
		}

		// ∏ˆ≈Î ¿Œµ¶Ω∫
		int ringVertexCount = sliceCount + 1;
		for (int y = 0; y < stackCount - 2; ++y)
		{
			for (int x = 0; x < sliceCount; ++x)
			{
				//  [y, x]-[y, x+1]
				//  |		/
				//  [y+1, x]
				idx.push_back(1 + (y)*ringVertexCount + (x));
				idx.push_back(1 + (y)*ringVertexCount + (x + 1));
				idx.push_back(1 + (y + 1) * ringVertexCount + (x));
				//		 [y, x+1]
				//		 /	  |
				//  [y+1, x]-[y+1, x+1]
				idx.push_back(1 + (y + 1) * ringVertexCount + (x));
				idx.push_back(1 + (y)*ringVertexCount + (x + 1));
				idx.push_back(1 + (y + 1) * ringVertexCount + (x + 1));
			}
		}

		// ≥≤±ÿ ¿Œµ¶Ω∫
		size_t bottomIndex = v.size() - 1;
		size_t lastRingStartIndex = bottomIndex - ringVertexCount;
		for (size_t i = 0; i < sliceCount; ++i)
		{
			//  [last+i]-[last+i+1]
			//  |      /
			//  [bottom]
			idx.push_back(static_cast<int>(bottomIndex));
			idx.push_back(static_cast<int>(lastRingStartIndex + i));
			idx.push_back(static_cast<int>(lastRingStartIndex + i + 1));
		}

		colliderShape = new ConvexPolyhedron(v, idx);
	}

	SphereCollider::~SphereCollider()
	{
		delete colliderShape;
	}

	void SphereCollider::Render(IGraphics* graphics)
	{
		static MaterialInfo matrerialInfo = *ResourceManager::GetInstance().GetMaterial(L"BoundaryDebug");

		ZeldaMath::Matrix4x4 worldTM = GetGameObject()->GetTransform()->GetWorldMatrix();
		std::wstring meshPath = ResourceManager::GetInstance().GetMesh(L"Sphere");

		graphics->DrawMesh(worldTM, meshPath, matrerialInfo, matrerialInfo.materialName, 0);
	}

	void SphereCollider::PreSerialize(json& jsonData) const
	{
		PRESERIALIZE_BASE(SphereCollider);
		PRESERIALIZE_SERPTR(colliderShape);
		PRESERIALIZE_VALUE(isTrigger);
	}

	void SphereCollider::PreDeserialize(const json& jsonData)
	{
		PREDESERIALIZE_BASE(SphereCollider);
		PREDESERIALIZE_SERPTR(colliderShape);
		PREDESERIALIZE_VALUE(isTrigger);
	}

	void SphereCollider::PostSerialize(json& jsonData) const
	{
		POSTSERIALIZE_BASE();
		POSTSERIALIZE_SERPTR(colliderShape);
	}

	void SphereCollider::PostDeserialize(const json& jsonData)
	{
		POSTDESERIALIZE_BASE();
		POSTDESERIALIZE_SERPTR(colliderShape);
	}
}