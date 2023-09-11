#pragma once

#include "ZeldaEngineAPI.h"

#include "Component.h"
#include "ISerializable.h"

#include "ZeldaMath.h"

namespace ZeldaEngine
{
	class GameObject;

	class ZELDAENGINE_API Transform : public Component
	{
	public:
		Transform();
		virtual ~Transform();

		Transform* GetParent();
		void SetParent(Transform* parenttransform);
		size_t GetChildCount();
		std::vector<Transform*> GetChildren();
		Transform* GetChild(std::wstring childname);
		Transform* GetChild(int childindex);
		void AddChild(Transform* child);
		void AddChild(Transform* child, int index);
		void RemoveChild(Transform* child);

		ZeldaMath::Vector3D GetLocalPosition() const;
		ZeldaMath::Quaternion GetLocalRotation() const;
		ZeldaMath::Vector3D GetLocalScale() const;
		// �ð���� ȸ����
		float GetLocalUIRotation() const;

		ZeldaMath::Quaternion GetLocalRotationAsQuaternion() const;

		void SetLocalPosition(ZeldaMath::Vector3D vector3d);
		void SetLocalRotation(ZeldaMath::Vector3D vector3d);
		void SetLocalRotation(ZeldaMath::Quaternion quaternion);
		void SetLocalUIRotation(float degreeAngle);
		void SetLocalScale(ZeldaMath::Vector3D vector3d);
		
		ZeldaMath::Vector3D GetFront() const;	// local(0, 0, 1)�� ���庯ȯ
		ZeldaMath::Vector3D GetRight() const;	// local(1, 0, 0)�� ���庯ȯ
		ZeldaMath::Vector3D GetUp() const;	// local(0, 1, 0)�� ���庯ȯ

		// axis�� ������ angle�� ȸ�� (local����)
		void Rotate(const ZeldaMath::Vector3D& axis, float angle);
		// center�� ȸ�� �߽����� axis������ ������ �Ͽ� angle�� ȸ��
		void Rotate(const ZeldaMath::Vector3D& worldRotationCenter, const ZeldaMath::Vector3D& axis, float angle);

		ZeldaMath::Matrix4x4 GetParentWorldMatrix() const;
		ZeldaMath::Matrix4x4 GetWorldMatrix() const;
		ZeldaMath::Matrix4x4 GetLocalMatrix() const;

		ZeldaMath::Quaternion GetWorldRotation() const;
		float GetWorldUIRotation() const;
		ZeldaMath::Vector3D GetWorldScale() const;
		ZeldaMath::Vector3D GetWorldPosition() const;

		// x, y ������ ȭ���� �߽��� (0, 0)���� �ϴ� �ȼ� ��ǥ, z������ ȭ����� �Ÿ�
		// z������ UI�� ��� 0�� ��ȯ�ϸ�, UI�� �ƴ� ��� ī�޶��� near���� ������ far���� �� ��� (0, 0, 0)�� ��ȯ�Ѵ�.
		ZeldaMath::Vector3D GetScreenPosition() const;

		// Component��(��) ���� ��ӵ�
		virtual void PreSerialize(json& jsonData) const override;
		virtual void PreDeserialize(const json& jsonData) override;
		virtual void PostSerialize(json& jsonData) const override;
		virtual void PostDeserialize(const json& jsonData) override;

	private:
		Transform* parent;
		std::vector<Transform*> children;

		ZeldaMath::Vector3D position;
		ZeldaMath::Quaternion rotation;
		ZeldaMath::Vector3D scale;

		float uiRotation;	// degreeAngle
		int uiCount;

		// ����� ���� Physics�� Octree�� ���� ����Ѵ�. ����Ѱ� �� �ʿ��� ��쿡�� �ٸ� ����� ������ ������ ��
		bool isMoved;
		bool isChangedParent;

		void DoMove(); // �ڽŰ� ��� �ڽĿ�����Ʈ�� isMoved�� true�� ����
		void DoChangeParent(); // �ڽŰ� ��� �ڽĿ�����Ʈ�� isChangedParent�� true�� ����

		// �� ������ ���� �������°�
		bool IsMoved();
		void ResetMoved();
		bool IsChangedParent();
		void ResetChangedParent();

		// ZeldaUI���� ����ϴ� �Լ�
		void AddUIComponent();
		void RemoveUIComponent();

		friend GameObject;
		friend class PhysicsSystem;
		friend class ZeldaUI;
	};
}