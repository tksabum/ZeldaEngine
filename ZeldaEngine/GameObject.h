#pragma once

#include "ZeldaEngineAPI.h"

#include "Serializable.h"

#include <string>
#include <vector>
#include <queue>
#include <functional>

namespace ZeldaEngine
{
	class Component;
	class SceneManager;
	class Transform;
	class TimeController;
	class GraphicsSystem;
	class Collider;
	class Collider2D;
	class Rigidbody;

	class ZELDAENGINE_API GameObject : public Serializable
	{
	public:
		void OnTriggerEnter();
		void OnTriggerStay();
		void OnTriggerExit();

		void OnTriggerEnter(Collider* other);
		void OnTriggerStay(Collider* other);
		void OnTriggerExit(Collider* other);

		void OnCollisionEnter();
		void OnCollisionStay();
		void OnCollisionExit();

		void OnCollisionEnter(Collider* other);
		void OnCollisionStay(Collider* other);
		void OnCollisionExit(Collider* other);

		void OnCollisionEnter2D();
		void OnCollisionStay2D();
		void OnCollisionExit2D();

		void OnCollisionEnter2D(Collider2D* other);
		void OnCollisionStay2D(Collider2D* other);
		void OnCollisionExit2D(Collider2D* other);

		// ������Ʈ ������ ���콺�� ������ ȣ��
		virtual void OnMouseLeftDownEvent();
		virtual void OnMouseRightDownEvent();
		// ������Ʈ ������ ���콺�� ���� ȣ��
		virtual void OnMouseLeftUpEvent();
		virtual void OnMouseRightUpEvent();


		// ���콺�� ������Ʈ ���� �ö��� �� ȣ��
		virtual void OnMouseEnterEvent();
		// ���콺�� ������Ʈ���� ����� �� ȣ��
		virtual void OnMouseExitEvent();
		// ���콺�� ������Ʈ ���� ������ ��� ȣ��
		virtual void OnMouseOverEvent();

		// Serializable��(��) ���� ��ӵ�
		virtual void PreSerialize(json& jsonData) const override;
		virtual void PreDeserialize(const json& jsonData) override;
		virtual void PostSerialize(json& jsonData) const override;
		virtual void PostDeserialize(const json& jsonData) override;

		std::wstring GetName();

		void SetActive(bool act);
		bool GetActive();
		// parent�� ���󰡸� active�� Ȯ���Ͽ� ��� active�� true�� ��쿡�� true�� ��ȯ�Ѵ�.
		bool GetRootActive();

		// ���ӿ�����Ʈ�� �����ϵ��� ����Ѵ�. (������ ������Ʈ�� ���ŵǴ°��� DestroyEvent�� �߻��� �� �����ȴ�.)
		void Destroy();

		void SetLayerID(int layerid);
		int GetLayerID();

		void SetLayerName(std::wstring layername);
		std::wstring GetLayerName();

		template <typename componentT>
		componentT* AddComponent()
		{
			Component* component = CreateComponent<componentT>();
			componentList.push_back(component);
			callComponentPreInitialize_(component);
			callComponentPostInitialize_(component);
			return (componentT*)component;
		}


		template <typename componentT>
		componentT* GetComponent()
		{
			for (int i = 0; i < componentList.size(); i++)
			{
				Component* component = dynamic_cast<componentT*>(componentList[i]);
				if (component != nullptr)
				{
					return (componentT*)component;
				}
			}

			return nullptr;
		}

		std::vector<Component*> GetComponents();

	private:
		enum class State
		{
			CREATED,
			ENABLE,
			DISABLE,
			DESTROY
		};
		State state;
		bool active;
		bool destroy;

		int layerID;

		Transform* transform;
		std::wstring name;

		std::vector<Component*> componentList;

		// ���� ���ӿ�����Ʈ�� ������Ʈ ��Ͽ��� ����
		void RemoveComponent(Component* component);

		// ������Ʈ ����
		void DeleteComponent(Component* component);

		GameObject(std::wstring gameobjectName = L"");
		virtual ~GameObject();

		/// <summary>
		/// ���� GameObject�� ParentGameObject��� GameObject�� �����Ͽ� children���� �߰��մϴ�.
		/// </summary>
		/// <returns> ������ GameObject�� ������ </returns>
		GameObject* AddGameObject(GameObject* parentobject, std::wstring gameobjectname);

		// childobject�� Ž���ؼ� �����Ѵ�. (�ڱ��ڽ��� Ȯ������ �ʴ´�.)
		void DeleteChild(GameObject* childobject);

		void DisableEvent(std::queue<std::pair<Component*, std::function<void(Component&)>>>& funcQueue, bool parentdisable = false);
		void AwakeEvent(std::queue<std::pair<Component*, std::function<void(Component&)>>>& funcQueue, bool parentdisable = false);
		void EnableEvent(std::queue<std::pair<Component*, std::function<void(Component&)>>>& funcQueue, bool parentdisable = false);
		void StartEvent(std::queue<std::pair<Component*, std::function<void(Component&)>>>& funcQueue, bool parentdisable = false);
		void stateChangeEvent(bool parentdisable = false);

		void DestroyEvent(std::queue<GameObject*>& destoryQueue);

		void EarlyUpdateEvent();
		void UpdateEvent();
		void LateUpdateEvent();

		void callComponentPreInitialize_(Component* component);
		void callComponentPostInitialize_(Component* component);

		/// <summary>
		/// <para>������Ʈ ��ü ����</para>
		/// <para>�� �Լ��� new�� ���� ������ �ϹǷ� ����</para>
		/// </summary>
		/// <typeparam name="componentT"> ������ ������Ʈ Ÿ�� </typeparam>
		/// <returns> ������ ������Ʈ ������ </returns>
		template<typename componentT>
		componentT* CreateComponent()
		{
			static_assert(std::is_base_of<Component, componentT>::value, "The class you are trying to create is not Component");

#ifdef _DEBUG
			Component::SetAddComponentFlag(true);
#endif // _DEBUG

			componentT* component = new componentT();

#ifdef _DEBUG
			Component::SetAddComponentFlag(false);
#endif // _DEBUG

			component->gameObject = this;
			return component;
		}

		friend SceneManager;
		friend GraphicsSystem;
		friend Transform;

	public:
		Transform* GetTransform();
};
}