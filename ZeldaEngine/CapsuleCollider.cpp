#include "pch.h"
#include "CapsuleCollider.h"

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
    using namespace ZeldaMath;

	CapsuleCollider::CapsuleCollider()
	{
        float radius = 0.5f; // 캡슐의 반지름
        float height = 1.0f; // 캡슐의 높이
        int stackCount = 5; // 수평 분할 - 반구 부분을 분할하는 수
        int sliceCount = 20; // 수직 분할

        std::vector<ZeldaMath::Vector3D> v;
        std::vector<int> idx;

        // 상단 반구 정점
        v.push_back(Vector3D(0.0f, radius + height * 0.5f, 0.0f));

        for (int i = 1; i <= stackCount; i++)
        {
            // 윗방향 벡터와의 각도
            float upTheta = PI * 0.5f * (i / static_cast<float>(stackCount));

            float xzsize = radius * sinf(upTheta);
            float ysize = radius * cosf(upTheta);
            
            for (int j = 0; j < sliceCount; j++)
            {
                float zTheta = PI * 2.0f * (j / static_cast<float>(sliceCount));

                float x = xzsize * sinf(zTheta);
                float y = ysize + height * 0.5f;
                float z = xzsize * cosf(zTheta);

                v.push_back(Vector3D(x, y, z));
            }
        }

        size_t middleIdx = v.size();

        // 하단 반구 정점
        for (int i = stackCount; i >= 1; i--)
        {
            // 윗방향 벡터와의 각도
            float upTheta = PI * 0.5f * (i / static_cast<float>(stackCount));

            float xzsize = radius * sinf(upTheta);
            float ysize = radius * cosf(upTheta);

            for (int j = 0; j < sliceCount; j++)
            {
                float zTheta = PI * 2.0f * (j / static_cast<float>(sliceCount));

                float x = xzsize * sinf(zTheta);
                float y = ysize + height * 0.5f;
                float z = xzsize * cosf(zTheta);

                v.push_back(Vector3D(x, -y, z));
            }
        }

        v.push_back(Vector3D(0.0f, -(radius + height * 0.5f), 0.0f));

        // 상단 반구 인덱스
        for (int i = 0; i < sliceCount; i++) {
            int a = 0;
            int b = 1 + i;
            int c = 1 + ((i + 1) % sliceCount);

            idx.push_back(a);
            idx.push_back(b);
            idx.push_back(c);
        }

        for (int i = 1; i < stackCount; i++) {
            for (int j = 0; j < sliceCount; j++) {
                int a = 1 + (i - 1) * sliceCount + j;
                int b = 1 + (i - 1) * sliceCount + ((j + 1) % sliceCount);
                int c = 1 + i * sliceCount + j;
                int d = 1 + i * sliceCount + ((j + 1) % sliceCount);

                idx.push_back(a);
                idx.push_back(c);
                idx.push_back(d);

                idx.push_back(a);
                idx.push_back(d);
                idx.push_back(b);
            }
        }

        // 실린더 부분 인덱스
        for (int i = 0; i < sliceCount; i++)
        {
            int a = middleIdx - sliceCount + i;
            int b = middleIdx - sliceCount + ((i + 1) % sliceCount);
            int c = middleIdx + i;
            int d = middleIdx + ((i + 1) % sliceCount);

            idx.push_back(a);
            idx.push_back(c);
            idx.push_back(d);

            idx.push_back(a);
            idx.push_back(d);
            idx.push_back(b);
        }

        // 하단 반구 인덱스
        for (int i = 1; i < stackCount; i++) {
            for (int j = 0; j < sliceCount; j++) {
                int a = middleIdx + (i - 1) * sliceCount + j;
                int b = middleIdx + (i - 1) * sliceCount + ((j + 1) % sliceCount);
                int c = middleIdx + i * sliceCount + j;
                int d = middleIdx + i * sliceCount + ((j + 1) % sliceCount);

                idx.push_back(a);
                idx.push_back(c);
                idx.push_back(d);

                idx.push_back(a);
                idx.push_back(d);
                idx.push_back(b);
            }
        }
        
        for (int i = 0; i < sliceCount; i++) {
            int a = v.size() - 1;
            int b = v.size() - 1 - sliceCount + i;
            int c = v.size() - 1 - sliceCount + ((i + 1) % sliceCount);

            idx.push_back(b);
            idx.push_back(a);
            idx.push_back(c);
        }

        colliderShape = new ConvexPolyhedron(v, idx);
	}

	CapsuleCollider::~CapsuleCollider()
	{
        delete colliderShape;
	}

    void CapsuleCollider::Render(IGraphics* graphics)
    {
        static MaterialInfo matrerialInfo = *ResourceManager::GetInstance().GetMaterial(L"BoundaryDebug");

        ZeldaMath::Matrix4x4 worldTM = GetGameObject()->GetTransform()->GetWorldMatrix();
        std::wstring meshPath = ResourceManager::GetInstance().GetMesh(L"Capsule");

        graphics->DrawMesh(worldTM, meshPath, matrerialInfo, matrerialInfo.materialName, 0);
    }

	void CapsuleCollider::PreSerialize(json& jsonData) const
	{
        PRESERIALIZE_BASE(CapsuleCollider);
        PRESERIALIZE_SERPTR(colliderShape);
        PRESERIALIZE_VALUE(isTrigger);
	}

	void CapsuleCollider::PreDeserialize(const json& jsonData)
	{
        PREDESERIALIZE_BASE(CapsuleCollider);
        PREDESERIALIZE_SERPTR(colliderShape);
        PREDESERIALIZE_VALUE(isTrigger);
	}

	void CapsuleCollider::PostSerialize(json& jsonData) const
	{
        POSTSERIALIZE_BASE();
        POSTSERIALIZE_SERPTR(colliderShape);
	}

	void CapsuleCollider::PostDeserialize(const json& jsonData)
	{
        POSTDESERIALIZE_BASE();
        POSTDESERIALIZE_SERPTR(colliderShape);
	}
}