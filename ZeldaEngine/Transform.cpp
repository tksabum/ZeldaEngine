#include "pch.h"
#include "Transform.h"

#include "CoreSystem.h"
#include "GraphicsSystem.h"
#include "GameObject.h"
#include "Camera.h"
#include "SceneManager.h"

namespace ZeldaEngine
{
	using namespace ZeldaMath;

	Transform::Transform() :
		parent(nullptr),
		children(),
		position(ZeldaMath::Vector3D::zero),
		rotation(ZeldaMath::Quaternion::identity),
		scale(ZeldaMath::Vector3D(1.0f, 1.0f, 1.0f)),
		isMoved(true),
		isChangedParent(false),
		uiRotation(0.0f),
		uiCount(0)
	{

	}

	Transform::~Transform()
	{

	}

	Transform* Transform::GetParent()
	{
		return parent;
	}

	void Transform::SetParent(Transform* parenttransform)
	{
		DoMove();
		DoChangeParent();

		// �̹� �θ� �־��ٸ� ���� �θ��� �ڽĿ��� ����
		if (parent != nullptr)
		{
			parent->children.erase(remove(parent->children.begin(), parent->children.end(), this), parent->children.end());
		}
		// �θ� �����ٸ� SceneManager���� ����
		else
		{
			SceneManager::GetInstance().gameobjectList.erase(remove(SceneManager::GetInstance().gameobjectList.begin(), SceneManager::GetInstance().gameobjectList.end(), this->GetGameObject()), SceneManager::GetInstance().gameobjectList.end());
			SceneManager::GetInstance().dontDestroyObjectList.erase(remove(SceneManager::GetInstance().dontDestroyObjectList.begin(), SceneManager::GetInstance().dontDestroyObjectList.end(), this->GetGameObject()), SceneManager::GetInstance().dontDestroyObjectList.end());
		}

		if (parenttransform == nullptr)
		{
			parent = nullptr;
			SceneManager::GetInstance().gameobjectList.push_back(GetGameObject());
		}
		else
		{
			parenttransform->DoMove();
			parent = parenttransform;
			parenttransform->children.push_back(this);
		}
	}

	size_t Transform::GetChildCount()
	{
		return children.size();
	}

	std::vector<Transform*> Transform::GetChildren()
	{
		return children;
	}

	Transform* Transform::GetChild(std::wstring childname)
	{
		for (int i = 0; i < children.size(); i++)
		{
			if (children[i]->GetGameObject()->GetName() == childname)
			{
				return children[i];
			}
		}

		return nullptr;
	}

	Transform* Transform::GetChild(int childindex)
	{
		return children[childindex];
	}

	void Transform::AddChild(Transform* child)
	{
		if (child == nullptr) return;
		child->DoMove();
		child->DoChangeParent();
		DoMove();

		// child�� �̹� �θ� �־��ٸ� ���� �θ��� �ڽĿ��� ����
		if (child->parent != nullptr)
		{
			child->parent->children.erase(remove(child->parent->children.begin(), child->parent->children.end(), this), child->parent->children.end());
		}
		// child�� SceneManager���� �����ǰ� �־��ٸ� SceneManager���� ����
		else
		{
			SceneManager::GetInstance().gameobjectList.erase(remove(SceneManager::GetInstance().gameobjectList.begin(), SceneManager::GetInstance().gameobjectList.end(), child->GetGameObject()), SceneManager::GetInstance().gameobjectList.end());
			SceneManager::GetInstance().dontDestroyObjectList.erase(remove(SceneManager::GetInstance().dontDestroyObjectList.begin(), SceneManager::GetInstance().dontDestroyObjectList.end(), child->GetGameObject()), SceneManager::GetInstance().dontDestroyObjectList.end());
		}

		child->parent = this;
		children.push_back(child);
	}

	void Transform::AddChild(Transform* child, int index)
	{
		if (child == nullptr) return;
		child->DoMove();
		child->DoChangeParent();
		DoMove();

		// child�� �̹� �θ� �־��ٸ� ���� �θ��� �ڽĿ��� ����
		if (child->parent != nullptr)
		{
			child->parent->children.erase(remove(child->parent->children.begin(), child->parent->children.end(), this), child->parent->children.end());
		}
		// child�� SceneManager���� �����ǰ� �־��ٸ� SceneManager���� ����
		else
		{
			SceneManager::GetInstance().gameobjectList.erase(remove(SceneManager::GetInstance().gameobjectList.begin(), SceneManager::GetInstance().gameobjectList.end(), child->GetGameObject()), SceneManager::GetInstance().gameobjectList.end());
			SceneManager::GetInstance().dontDestroyObjectList.erase(remove(SceneManager::GetInstance().dontDestroyObjectList.begin(), SceneManager::GetInstance().dontDestroyObjectList.end(), child->GetGameObject()), SceneManager::GetInstance().dontDestroyObjectList.end());
		}

		assert(index <= children.size());
		child->parent = this;
		children.insert(children.begin() + index, child);
	}

	void Transform::RemoveChild(Transform* child)
	{
		if (child == nullptr) return;
		child->DoMove();
		child->DoChangeParent();
		DoMove();

		auto it = std::find(children.begin(), children.end(), child);
		if (it != children.end())
		{
			// child�� ��ġ�� �����ϱ� ���� WorldTransform������ ����Ͽ� �� ������ LocalTransform�� ����
			Vector3D orgpos = child->GetWorldPosition();
			Quaternion orgrot = child->GetWorldRotation();
			Vector3D orgscl = child->GetWorldScale();

			child->SetLocalPosition(orgpos);
			child->SetLocalRotation(orgrot);
			child->SetLocalScale(orgscl);

			// child�� �����Ǿ���
			children.erase(it);
			child->parent = nullptr;

			// child�� SceneManager�� �̵���Ų��.
			// root Object�� SceneManager�� gameobjectList�� dontDestroyObjectList�߿��� ��� �����ִ��� �Ǵ��ؼ� �߰��Ѵ�.
			Transform* root = this;
			while (root->parent != nullptr)
			{
				root = root->parent;
			}
			auto iter0 = std::find(SceneManager::GetInstance().gameobjectList.begin(), SceneManager::GetInstance().gameobjectList.end(), root->GetGameObject());
			auto iter1 = std::find(SceneManager::GetInstance().dontDestroyObjectList.begin(), SceneManager::GetInstance().dontDestroyObjectList.end(), root->GetGameObject());

			// �ٸ� �������� �����ϴٸ� �� List���� ��� root�� �����ϰų� �Ѵ� ���� �� ����.
			assert(!(iter0 != SceneManager::GetInstance().gameobjectList.end() && iter1 != SceneManager::GetInstance().dontDestroyObjectList.end()));
			assert(!(iter0 == SceneManager::GetInstance().gameobjectList.end() && iter1 == SceneManager::GetInstance().dontDestroyObjectList.end()));

			// gameobjectList�� ����
			if (iter0 != SceneManager::GetInstance().gameobjectList.end())
			{
				SceneManager::GetInstance().gameobjectList.push_back(child->GetGameObject());
			}
			// dontDestroyObjectList�� ����
			else
			{
				SceneManager::GetInstance().dontDestroyObjectList.push_back(child->GetGameObject());
			}
		}
		else
		{
			// child�� ã�� ������
		}
	}

	Vector3D Transform::GetLocalPosition() const
	{
		return position;
	}

	Quaternion Transform::GetLocalRotation() const
	{
		if (uiCount > 0)
		{
			return Quaternion::identity;
		}
		else
		{
			return rotation;
		}
	}

	Vector3D Transform::GetLocalScale() const
	{
		return scale;
	}

	float Transform::GetLocalUIRotation() const
	{
		if (uiCount > 0)
		{
			return uiRotation;
		}
		else
		{
			return 0.0f;
		}
	}

	Quaternion Transform::GetLocalRotationAsQuaternion() const
	{
		return rotation;
	}

	void Transform::SetLocalPosition(Vector3D vector3d)
	{
		DoMove();

		position = vector3d;
	}

	void Transform::SetLocalRotation(Vector3D vector3d)
	{
		DoMove();

		Vector3D radianVec = Vector3D(DegreeToRadian(vector3d.x), DegreeToRadian(vector3d.y), DegreeToRadian(vector3d.z));

		rotation = EulerToQuaternion(radianVec);
	}

	void Transform::SetLocalRotation(ZeldaMath::Quaternion quaternion)
	{
		DoMove();

		rotation = quaternion;
	}

	void Transform::SetLocalUIRotation(float degreeAngle)
	{
		DoMove();

		uiRotation = degreeAngle;
	}

	void Transform::SetLocalScale(Vector3D vector3d)
	{
		DoMove();

		scale = vector3d;
	}

	Vector3D Transform::GetFront() const
	{
		Vector3D frontdir = GetWorldRotation().GetTransformMatrix() * Vector3D::front;

		return frontdir.Normalize();
	}

	Vector3D Transform::GetRight() const
	{
		Vector3D rightdir = GetWorldRotation().GetTransformMatrix() * Vector3D::right;

		return rightdir.Normalize();
	}

	Vector3D Transform::GetUp() const
	{
		Vector3D updir = GetWorldRotation().GetTransformMatrix() * Vector3D::up;

		return updir.Normalize();
	}

	void Transform::Rotate(const Vector3D& axis, float angle)
	{
		if (uiCount > 0) return;

		DoMove();

		rotation = Quaternion::AxisAngleQuaternion(axis.Normalize(), angle) * rotation;
	}

	void Transform::Rotate(const ZeldaMath::Vector3D& worldRotationCenter, const ZeldaMath::Vector3D& axis, float angle)
	{
		if (uiCount > 0) return;

		DoMove();

		// ���� ��ǥ�迡�� ���� ��ǥ��� ��ȯ�ϱ� ���� ���� ��ǥ���� ȸ�� �߽��� ���� ��ǥ��� ��ȯ�մϴ�.
		Vector3D localRotationCenter = GetParentWorldMatrix().Inverse() * worldRotationCenter;

		position = static_cast<Vector3D>(Matrix4x4::Rotation(axis.Normalize(), angle) * Matrix4x4::Translation(position - localRotationCenter) * Vector3D::zero) + localRotationCenter;
		rotation = Quaternion::AxisAngleQuaternion(axis.Normalize(), angle) * rotation;
	}

	ZeldaMath::Matrix4x4 Transform::GetParentWorldMatrix() const
	{
		if (parent == nullptr)
		{
			return ZeldaMath::Matrix4x4::identity;
		}

		return ZeldaMath::Matrix4x4::identity * parent->GetWorldMatrix();
	}

	Matrix4x4 Transform::GetWorldMatrix() const
	{
		if (uiCount > 0)
		{
			return Matrix4x4::Translation(GetWorldPosition()) * Matrix4x4::Rotation(-Vector3D::front, DegreeToRadian(GetWorldUIRotation())) * Matrix4x4::Scale(GetLocalScale());
		}

		if (parent == nullptr)
		{
			return GetLocalMatrix();
		}

		return parent->GetWorldMatrix() * GetLocalMatrix();
	}

	Matrix4x4 Transform::GetLocalMatrix() const
	{
		if (uiCount > 0)
		{
			return Matrix4x4::Translation(position) * Matrix4x4::Rotation(-Vector3D::front, DegreeToRadian(uiRotation)) * Matrix4x4::Scale(scale);
		}

		return Matrix4x4::Translation(position) * rotation.GetTransformMatrix() * Matrix4x4::Scale(scale);
	}

	Quaternion Transform::GetWorldRotation() const
	{
		if (parent)
		{
			// �θ��� ȸ������ ������
			Quaternion parentRotation = parent->GetWorldRotation();

			// �θ��� ȸ������ ������ ȸ������ ���Ͽ� ���ο� ȸ���� ��ȯ
			return parentRotation * GetLocalRotationAsQuaternion();
		}
		else
		{
			return GetLocalRotationAsQuaternion();
		}
	}

	float Transform::GetWorldUIRotation() const
	{
		if (parent)
		{
			// �θ��� ȸ������ ������
			float parentRotation = parent->GetWorldUIRotation();

			// �θ��� ȸ������ ������ ȸ������ ���Ͽ� ���ο� ȸ���� ��ȯ
			return parentRotation + GetLocalUIRotation();
		}
		else
		{
			return GetLocalUIRotation();
		}
	}

	ZeldaMath::Vector3D Transform::GetWorldScale() const
	{
		if (uiCount > 0)
		{
			return GetLocalScale();
		}

		if (parent)
		{
			// �θ��� Scale���� ������
			ZeldaMath::Vector3D pScale = parent->GetWorldScale();

			// �θ��� Scale���� ������ Scale���� ���Ͽ� ���ο� ȸ���� ��ȯ
			return ZeldaMath::Vector3D(pScale.x * scale.x, pScale.y * scale.y, pScale.z * scale.z);
		}
		else
		{
			return scale;
		}
	}

	ZeldaMath::Vector3D Transform::GetWorldPosition() const
	{
		if (uiCount > 0)
		{
			if (parent)
			{
				return Matrix4x4::Translation(parent->GetWorldPosition()) * Matrix4x4::Rotation(-Vector3D::front, DegreeToRadian(GetWorldUIRotation())) * GetLocalPosition();
			}
			else
			{
				return Matrix4x4::Rotation(-Vector3D::front, DegreeToRadian(GetWorldUIRotation())) * GetLocalPosition();
			}
		}

		return GetWorldMatrix() * Vector3D::zero;
	}

	ZeldaMath::Vector3D Transform::GetScreenPosition() const
	{
		if (uiCount > 0)
		{
			Vector3D worldPos = GetWorldPosition();

			return Vector3D(worldPos.x, worldPos.y, 0.0f);
		}

		Camera* camera = SceneManager::GetInstance().GetMainCamera();
		Transform* cameraTransform = camera->GetGameObject()->GetTransform();

		Vector2D windowSize = GraphicsSystem::GetInstance().GetWindowSize();

		Vector3D cameraFront = cameraTransform->GetFront();
		Vector3D cameraUp = cameraTransform->GetUp();
		Vector3D cameraRight = cameraTransform->GetRight();
		Vector3D cameraPos = cameraTransform->GetWorldPosition();

		Vector3D worldPos = GetWorldPosition();

		float x = DotProduct(cameraRight, worldPos - cameraPos);
		float y = DotProduct(cameraUp, worldPos - cameraPos);
		float z = DotProduct(cameraFront, worldPos - cameraPos);

		if (z < camera->GetCameraNear() || z > camera->GetCameraFar()) return Vector3D::zero;

		return Vector3D(windowSize.y * 0.5f * x * camera->GetCameraNear() / (z * std::tanf(camera->GetCameraFOV() / 2)), windowSize.y * 0.5f * y * camera->GetCameraNear() / (z * std::tanf(camera->GetCameraFOV() / 2)), z);
	}

	void Transform::PreSerialize(json& jsonData) const
	{
		PRESERIALIZE_BASE(Transform);

		PRESERIALIZE_SERVALUE(position);
		PRESERIALIZE_SERVALUE(rotation);
		PRESERIALIZE_SERVALUE(scale);
		PRESERIALIZE_VALUE(uiRotation);
	}

	void Transform::PreDeserialize(const json& jsonData)
	{
		PREDESERIALIZE_BASE(Transform);

		PREDESERIALIZE_SERVALUE(position);
		PREDESERIALIZE_SERVALUE(rotation);
		PREDESERIALIZE_SERVALUE(scale);
		PREDESERIALIZE_VALUE(uiRotation);
	}

	void Transform::PostSerialize(json& jsonData) const
	{
		POSTSERIALIZE_BASE();

		jsonData["Component"]["ID_parent"] = IOManager::GetInstance().GetID(static_cast<void*>(const_cast<Transform*>(parent)));

		POSTSERIALIZE_SERVALUE(position);
		POSTSERIALIZE_SERVALUE(rotation);
		POSTSERIALIZE_SERVALUE(scale);
	}

	void Transform::PostDeserialize(const json& jsonData)
	{
		POSTDESERIALIZE_BASE();

		Transform* tmpparent = static_cast<Transform*>(IOManager::GetInstance().GetAddress(jsonData["Component"]["ID_parent"]));
		if (tmpparent != nullptr)
		{
			SetParent(tmpparent);
		}

		POSTDESERIALIZE_SERVALUE(position);
		POSTDESERIALIZE_SERVALUE(rotation);
		POSTDESERIALIZE_SERVALUE(scale);
	}

	void Transform::DoMove()
	{
		isMoved = true;
		for (size_t i = 0; i < children.size(); i++)
		{
			children[i]->DoMove();
		}
	}

	void Transform::DoChangeParent()
	{
		isChangedParent = true;
		for (size_t i = 0; i < children.size(); i++)
		{
			children[i]->DoChangeParent();
		}
	}

	bool Transform::IsMoved()
	{
		return isMoved;
	}

	void Transform::ResetMoved()
	{
		isMoved = false;
	}

	bool Transform::IsChangedParent()
	{
		return isChangedParent;
	}

	void Transform::ResetChangedParent()
	{
		isChangedParent = false;
	}

	void Transform::AddUIComponent()
	{
		uiCount += 1;
	}

	void Transform::RemoveUIComponent()
	{
		uiCount -= 1;
	}
}