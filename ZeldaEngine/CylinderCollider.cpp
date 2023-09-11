#include "pch.h"
#include "CylinderCollider.h"

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

    CylinderCollider::CylinderCollider() {
        float radius = 0.5f; // �Ǹ����� ������
        float height = 1.0f; // �Ǹ����� ����
        int sliceCount = 20; // ���� ����

        std::vector<ZeldaMath::Vector3D> v;
        std::vector<int> idx;

        // ���� ����
        v.push_back(ZeldaMath::Vector3D(0.0f, height * 0.5f, 0.0f));

        // �ٴ� ����
        v.push_back(ZeldaMath::Vector3D(0.0f, -height * 0.5f, 0.0f));

        // ����
        for (int i = 0; i < sliceCount; ++i)
        {
            float zTheta = PI * 2.0f * (i / static_cast<float>(sliceCount));

            float x = radius * sinf(zTheta);
            float y = height * 0.5f;
            float z = radius * cosf(zTheta);

            v.push_back(Vector3D(x, y, z));
        }

        // �ظ�
        for (int i = 0; i < sliceCount; ++i)
        {
            float zTheta = PI * 2.0f * (i / static_cast<float>(sliceCount));

            float x = radius * sinf(zTheta);
            float y = height * 0.5f;
            float z = radius * cosf(zTheta);

            v.push_back(Vector3D(x, -y, z));
        }



        // ���� �ε���
        for (int i = 0; i < sliceCount; ++i)
        {
            int a = 0;
            int b = 2 + i;
            int c = 2 + ((i + 1) % sliceCount);

            idx.push_back(a);
            idx.push_back(b);
            idx.push_back(c);
        }

        // ���� �ε���
        for (int i = 0; i < sliceCount; ++i)
        {
            int a = 2 + i;
            int b = 2 + ((i + 1) % sliceCount);
            int c = 2 + sliceCount + i;
            int d = 2 + sliceCount + ((i + 1) % sliceCount);

            idx.push_back(a);
            idx.push_back(c);
            idx.push_back(d);

            idx.push_back(a);
            idx.push_back(d);
            idx.push_back(b);
        }

        // �ظ� �ε���
        for (int i = 0; i < sliceCount; ++i)
        {
            int a = 1;
            int b = 2 + sliceCount + i;
            int c = 2 + sliceCount + ((i + 1) % sliceCount);

            idx.push_back(b);
            idx.push_back(a);
            idx.push_back(c);
        }

        colliderShape = new ConvexPolyhedron(v, idx);
    }

	CylinderCollider::~CylinderCollider()
	{
        delete colliderShape;
	}

    void CylinderCollider::Render(IGraphics* graphics)
    {
        static MaterialInfo matrerialInfo = *ResourceManager::GetInstance().GetMaterial(L"BoundaryDebug");

        ZeldaMath::Matrix4x4 worldTM = GetGameObject()->GetTransform()->GetWorldMatrix();
        std::wstring meshPath = ResourceManager::GetInstance().GetMesh(L"Cylinder");

        graphics->DrawMesh(worldTM, meshPath, matrerialInfo, matrerialInfo.materialName, 0);
    }

	void CylinderCollider::PreSerialize(json& jsonData) const
	{
        PRESERIALIZE_BASE(CylinderCollider);
        PRESERIALIZE_SERPTR(colliderShape);
        PRESERIALIZE_VALUE(isTrigger);
	}

	void CylinderCollider::PreDeserialize(const json& jsonData)
	{
        PREDESERIALIZE_BASE(CylinderCollider);
        PREDESERIALIZE_SERPTR(colliderShape);
        PREDESERIALIZE_VALUE(isTrigger);
	}

	void CylinderCollider::PostSerialize(json& jsonData) const
	{
        POSTSERIALIZE_BASE();
        POSTSERIALIZE_SERPTR(colliderShape);
	}

	void CylinderCollider::PostDeserialize(const json& jsonData)
	{
        POSTDESERIALIZE_BASE();
        POSTDESERIALIZE_SERPTR(colliderShape);
	}
}