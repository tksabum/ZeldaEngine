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
        float radius = 0.5f; // ĸ���� ������
        float height = 1.0f; // ĸ���� ����
        int stackCount = 5; // ���� ���� - �ݱ� �κ��� �����ϴ� ��
        int sliceCount = 20; // ���� ����

        std::vector<ZeldaMath::Vector3D> v;
        std::vector<int> idx;

        // ��� �ݱ� ����
        v.push_back(Vector3D(0.0f, radius + height * 0.5f, 0.0f));

        for (int i = 1; i <= stackCount; i++)
        {
            // ������ ���Ϳ��� ����
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

        // �ϴ� �ݱ� ����
        for (int i = stackCount; i >= 1; i--)
        {
            // ������ ���Ϳ��� ����
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

        // ��� �ݱ� �ε���
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

        // �Ǹ��� �κ� �ε���
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

        // �ϴ� �ݱ� �ε���
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