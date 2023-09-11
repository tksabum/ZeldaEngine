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
		// 시계방향 회전값
		float GetLocalUIRotation() const;

		ZeldaMath::Quaternion GetLocalRotationAsQuaternion() const;

		void SetLocalPosition(ZeldaMath::Vector3D vector3d);
		void SetLocalRotation(ZeldaMath::Vector3D vector3d);
		void SetLocalRotation(ZeldaMath::Quaternion quaternion);
		void SetLocalUIRotation(float degreeAngle);
		void SetLocalScale(ZeldaMath::Vector3D vector3d);
		
		ZeldaMath::Vector3D GetFront() const;	// local(0, 0, 1)의 월드변환
		ZeldaMath::Vector3D GetRight() const;	// local(1, 0, 0)의 월드변환
		ZeldaMath::Vector3D GetUp() const;	// local(0, 1, 0)의 월드변환

		// axis를 축으로 angle로 회전 (local기준)
		void Rotate(const ZeldaMath::Vector3D& axis, float angle);
		// center를 회전 중심으로 axis방향을 축으로 하여 angle로 회전
		void Rotate(const ZeldaMath::Vector3D& worldRotationCenter, const ZeldaMath::Vector3D& axis, float angle);

		ZeldaMath::Matrix4x4 GetParentWorldMatrix() const;
		ZeldaMath::Matrix4x4 GetWorldMatrix() const;
		ZeldaMath::Matrix4x4 GetLocalMatrix() const;

		ZeldaMath::Quaternion GetWorldRotation() const;
		float GetWorldUIRotation() const;
		ZeldaMath::Vector3D GetWorldScale() const;
		ZeldaMath::Vector3D GetWorldPosition() const;

		// x, y 성분은 화면의 중심을 (0, 0)으로 하는 픽셀 좌표, z성분은 화면과의 거리
		// z성분은 UI인 경우 0을 반환하며, UI가 아닌 경우 카메라의 near보다 가깝고 far보다 먼 경우 (0, 0, 0)을 반환한다.
		ZeldaMath::Vector3D GetScreenPosition() const;

		// Component을(를) 통해 상속됨
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

		// 현재는 오직 Physics의 Octree를 위해 사용한다. 비슷한게 더 필요한 경우에는 다른 방법을 생각해 봐야할 듯
		bool isMoved;
		bool isChangedParent;

		void DoMove(); // 자신과 모든 자식오브젝트의 isMoved를 true로 변경
		void DoChangeParent(); // 자신과 모든 자식오브젝트의 isChangedParent를 true로 변경

		// 한 프레임 동안 움직였는가
		bool IsMoved();
		void ResetMoved();
		bool IsChangedParent();
		void ResetChangedParent();

		// ZeldaUI에서 사용하는 함수
		void AddUIComponent();
		void RemoveUIComponent();

		friend GameObject;
		friend class PhysicsSystem;
		friend class ZeldaUI;
	};
}