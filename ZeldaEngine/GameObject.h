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

		// 오브젝트 위에서 마우스를 누르면 호출
		virtual void OnMouseLeftDownEvent();
		virtual void OnMouseRightDownEvent();
		// 오브젝트 위에서 마우스를 떼면 호출
		virtual void OnMouseLeftUpEvent();
		virtual void OnMouseRightUpEvent();


		// 마우스가 오브젝트 위로 올라갔을 때 호출
		virtual void OnMouseEnterEvent();
		// 마우스가 오브젝트에서 벗어났을 때 호출
		virtual void OnMouseExitEvent();
		// 마우스가 오브젝트 위에 있으면 계속 호출
		virtual void OnMouseOverEvent();

		// Serializable을(를) 통해 상속됨
		virtual void PreSerialize(json& jsonData) const override;
		virtual void PreDeserialize(const json& jsonData) override;
		virtual void PostSerialize(json& jsonData) const override;
		virtual void PostDeserialize(const json& jsonData) override;

		std::wstring GetName();

		void SetActive(bool act);
		bool GetActive();
		// parent를 따라가며 active를 확인하여 모든 active가 true인 경우에만 true를 반환한다.
		bool GetRootActive();

		// 게임오브젝트를 제거하도록 명령한다. (실제로 오브젝트가 제거되는것은 DestroyEvent가 발생할 때 삭제된다.)
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

		// 현재 게임오브젝트의 컴포넌트 목록에서 제거
		void RemoveComponent(Component* component);

		// 컴포넌트 삭제
		void DeleteComponent(Component* component);

		GameObject(std::wstring gameobjectName = L"");
		virtual ~GameObject();

		/// <summary>
		/// 현재 GameObject가 ParentGameObject라면 GameObject를 생성하여 children으로 추가합니다.
		/// </summary>
		/// <returns> 생성된 GameObject의 포인터 </returns>
		GameObject* AddGameObject(GameObject* parentobject, std::wstring gameobjectname);

		// childobject를 탐색해서 제거한다. (자기자신은 확인하지 않는다.)
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
		/// <para>컴포넌트 객체 생성</para>
		/// <para>이 함수는 new와 같은 역할을 하므로 주의</para>
		/// </summary>
		/// <typeparam name="componentT"> 생성할 컴포넌트 타입 </typeparam>
		/// <returns> 생성된 컴포넌트 포인터 </returns>
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