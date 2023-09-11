#include "pch.h"
#include "GameObject.h"

#include <string>
#include <cmath>
#include <typeinfo>

#include "ZeldaMath.h"
#include "Transform.h"
#include "Camera.h"
#include "EngineSetting.h"
#include "SceneManager.h"
#include "ComponentFactory.h"


#include <fstream>
#include <iostream>
using namespace std;

namespace ZeldaEngine
{
	void GameObject::OnTriggerEnter()
	{
		for (int i = 0; i < componentList.size(); i++)
		{
			componentList[i]->OnTriggerEnter();
		}
	}

	void GameObject::OnTriggerStay()
	{
		for (int i = 0; i < componentList.size(); i++)
		{
			componentList[i]->OnTriggerStay();
		}
	}

	void GameObject::OnTriggerExit()
	{
		for (int i = 0; i < componentList.size(); i++)
		{
			componentList[i]->OnTriggerExit();
		}
	}

	void GameObject::OnTriggerEnter(Collider* other)
	{
		for (int i = 0; i < componentList.size(); i++)
		{
			componentList[i]->OnTriggerEnter(other);
		}
	}

	void GameObject::OnTriggerStay(Collider* other)
	{
		for (int i = 0; i < componentList.size(); i++)
		{
			componentList[i]->OnTriggerStay(other);
		}
	}

	void GameObject::OnTriggerExit(Collider* other)
	{
		for (int i = 0; i < componentList.size(); i++)
		{
			componentList[i]->OnTriggerExit(other);
		}
	}

	void GameObject::OnCollisionEnter()
	{
		for (int i = 0; i < componentList.size(); i++)
		{
			componentList[i]->OnCollisionEnter();
		}
	}

	void GameObject::OnCollisionStay()
	{
		for (int i = 0; i < componentList.size(); i++)
		{
			componentList[i]->OnCollisionStay();
		}
	}

	void GameObject::OnCollisionExit()
	{
		for (int i = 0; i < componentList.size(); i++)
		{
			componentList[i]->OnCollisionExit();
		}
	}

	void GameObject::OnCollisionEnter(Collider* other)
	{
		for (int i = 0; i < componentList.size(); i++)
		{
			componentList[i]->OnCollisionEnter(other);
		}
	}

	void GameObject::OnCollisionStay(Collider* other)
	{
		for (int i = 0; i < componentList.size(); i++)
		{
			componentList[i]->OnCollisionStay(other);
		}
	}

	void GameObject::OnCollisionExit(Collider* other)
	{
		for (int i = 0; i < componentList.size(); i++)
		{
			componentList[i]->OnCollisionExit(other);
		}
	}

	void GameObject::OnCollisionEnter2D()
	{
		for (int i = 0; i < componentList.size(); i++)
		{
			componentList[i]->OnCollisionEnter2D();
		}
	}

	void GameObject::OnCollisionStay2D()
	{
		for (int i = 0; i < componentList.size(); i++)
		{
			componentList[i]->OnCollisionStay2D();
		}
	}

	void GameObject::OnCollisionExit2D()
	{
		for (int i = 0; i < componentList.size(); i++)
		{
			componentList[i]->OnCollisionExit2D();
		}
	}

	void GameObject::OnCollisionEnter2D(Collider2D* other)
	{
		for (int i = 0; i < componentList.size(); i++)
		{
			componentList[i]->OnCollisionEnter2D(other);
		}
	}

	void GameObject::OnCollisionStay2D(Collider2D* other)
	{
		for (int i = 0; i < componentList.size(); i++)
		{
			componentList[i]->OnCollisionStay2D(other);
		}
	}

	void GameObject::OnCollisionExit2D(Collider2D* other)
	{
		for (int i = 0; i < componentList.size(); i++)
		{
			componentList[i]->OnCollisionExit2D(other);
		}
	}

	void GameObject::OnMouseLeftDownEvent()
	{
		for (int i = 0; i < componentList.size(); i++)
		{
			componentList[i]->OnMouseLeftDown();
		}
	}

	void GameObject::OnMouseRightDownEvent()
	{
		for (int i = 0; i < componentList.size(); i++)
		{
			componentList[i]->OnMouseRightDown();
		}
	}

	void GameObject::OnMouseLeftUpEvent()
	{
		for (int i = 0; i < componentList.size(); i++)
		{
			componentList[i]->OnMouseLeftUp();
		}
	}

	void GameObject::OnMouseRightUpEvent()
	{
		for (int i = 0; i < componentList.size(); i++)
		{
			componentList[i]->OnMouseRightUp();
		}
	}

	void GameObject::OnMouseEnterEvent()
	{
		for (int i = 0; i < componentList.size(); i++)
		{
			componentList[i]->OnMouseEnter();
		}
	}

	void GameObject::OnMouseExitEvent()
	{
		for (int i = 0; i < componentList.size(); i++)
		{
			componentList[i]->OnMouseExit();
		}
	}

	void GameObject::OnMouseOverEvent()
	{
		for (int i = 0; i < componentList.size(); i++)
		{
			componentList[i]->OnMouseOver();
		}
	}

	wstring GameObject::GetName()
	{
		return name;
	}

	void GameObject::SetActive(bool act)
	{
		if (destroy== true && act == true)
		{
			return;
		}

		active = act;
	}

	bool GameObject::GetActive()
	{
		return active;
	}

	bool GameObject::GetRootActive()
	{
		bool rootResult = true;

		if (transform->GetParent() != nullptr)
		{
			rootResult = transform->GetParent()->GetGameObject()->GetRootActive();
		}

		bool ownResult = GetActive();

		return rootResult && ownResult;
	}

	void GameObject::Destroy()
	{
		SetActive(false);
		destroy = true;
		if (state != State::ENABLE)
		{
			state = State::DESTROY;
		}

		vector<Transform*> trchildren = transform->GetChildren();

		for (int i = 0; i < trchildren.size(); i++)
		{
			trchildren[i]->GetGameObject()->Destroy();
		}
	}

	void GameObject::SetLayerID(int layerid)
	{
		layerID = layerid;
	}

	int GameObject::GetLayerID()
	{
		return layerID;
	}

	void GameObject::SetLayerName(std::wstring layername)
	{
		layerID = EngineSetting::GetInstance().GetLayerID(layername);
	}

	std::wstring GameObject::GetLayerName()
	{
		return EngineSetting::GetInstance().GetLayerName(layerID);
	}

	std::vector<Component*> GameObject::GetComponents()
	{
		return componentList;
	}

	void GameObject::RemoveComponent(Component* component)
	{
		componentList.erase(remove(componentList.begin(), componentList.end(), component), componentList.end());
	}

	void GameObject::DeleteComponent(Component* component)
	{
		delete component;
	}

	GameObject::GameObject(std::wstring gameobjectName)
	{
		state = State::CREATED;
		active = true;
		destroy = false;

		layerID = EngineSetting::GetInstance().GetLayerID(L"Default");

		name = gameobjectName;

		transform = CreateComponent<Transform>();
	}

	GameObject::~GameObject()
	{
		vector<Transform*> trchildren = transform->GetChildren();

		for (int i = 0; i < trchildren.size(); i++)
		{
			delete trchildren[i]->GetGameObject();
		}

		DeleteComponent(transform);

		for (int i = 0; i < componentList.size(); i++)
		{
			DeleteComponent(componentList[i]);
		}

		componentList.clear();
	}

	GameObject* GameObject::AddGameObject(GameObject* parentobject, std::wstring gameobjectname)
	{
		// 현재 객체가 parentobject인 경우
		if (this == parentobject)
		{
			GameObject* object = new GameObject(gameobjectname);
			object->transform->SetParent(transform);

			return object;
		}

		GameObject* result = nullptr;


		// 자식오브젝트 들에서 addgameobject 실행
		vector<Transform*> trchildren = transform->GetChildren();

		for (int i = 0; i < trchildren.size(); i++)
		{
			result = trchildren[i]->GetGameObject()->AddGameObject(parentobject, gameobjectname);
			if (result != nullptr)
			{
				// parentobject에 object를 추가하는데 성공
				break;
			}
		}

		return result;
	}

	void GameObject::DeleteChild(GameObject* childobject)
	{
		for (size_t i = 0; i < transform->children.size(); i++)
		{
			if (transform->children[i]->GetGameObject() == childobject)
			{
				delete transform->children[i]->GetGameObject();
				transform->children.erase(transform->children.begin() + i);
				break;
			}

			transform->children[i]->GetGameObject()->DeleteChild(childobject);
		}
	}

	void GameObject::DisableEvent(std::queue<std::pair<Component*, std::function<void(Component&)>>>& funcQueue, bool parentdisable)
	{
		if (state == State::CREATED)
		{
			if (parentdisable == false && active == true)
			{
				for (int i = 0; i < transform->children.size(); i++)
				{
					transform->children[i]->GetGameObject()->DisableEvent(funcQueue, false);
				}
			}
		}
		else if (state == State::ENABLE)
		{
			if (destroy == true)
			{
				for (int i = 0; i < componentList.size(); i++)
				{
					funcQueue.push({ componentList[i], (&Component::OnDisable) });
				}

				for (int i = 0; i < transform->children.size(); i++)
				{
					transform->children[i]->GetGameObject()->DisableEvent(funcQueue, true);
				}
			}
			else if (parentdisable == false && active == true)
			{
				for (int i = 0; i < transform->children.size(); i++)
				{
					transform->children[i]->GetGameObject()->DisableEvent(funcQueue, false);
				}
			}
			else
			{
				for (int i = 0; i < componentList.size(); i++)
				{
					funcQueue.push({ componentList[i], (&Component::OnDisable) });
				}

				for (int i = 0; i < transform->children.size(); i++)
				{
					transform->children[i]->GetGameObject()->DisableEvent(funcQueue, true);
				}
			}
		}
		else if (state == State::DISABLE)
		{
			if (active == true)
			{
				for (int i = 0; i < transform->children.size(); i++)
				{
					transform->children[i]->GetGameObject()->DisableEvent(funcQueue, false);
				}
			}
		}
	}

	void GameObject::AwakeEvent(std::queue<std::pair<Component*, std::function<void(Component&)>>>& funcQueue, bool parentdisable)
	{
		if (state == State::CREATED)
		{
			if (parentdisable == false && active == true)
			{
				for (int i = 0; i < componentList.size(); i++)
				{
					funcQueue.push({ componentList[i], (&Component::Awake) });
				}

				for (int i = 0; i < transform->children.size(); i++)
				{
					transform->children[i]->GetGameObject()->AwakeEvent(funcQueue, false);
				}
			}
		}
	}

	void GameObject::EnableEvent(std::queue<std::pair<Component*, std::function<void(Component&)>>>& funcQueue, bool parentdisable)
	{
		if (state == State::CREATED)
		{
			if (parentdisable == false && active == true)
			{
				for (int i = 0; i < componentList.size(); i++)
				{
					funcQueue.push({ componentList[i], (&Component::OnEnable) });
				}

				for (int i = 0; i < transform->children.size(); i++)
				{
					transform->children[i]->GetGameObject()->EnableEvent(funcQueue, false);
				}
			}
		}
		else if (state == State::ENABLE)
		{
			if (destroy == true)
			{
				for (int i = 0; i < transform->children.size(); i++)
				{
					transform->children[i]->GetGameObject()->EnableEvent(funcQueue, true);
				}
			}
			else if (parentdisable == false && active == true)
			{
				for (int i = 0; i < transform->children.size(); i++)
				{
					transform->children[i]->GetGameObject()->EnableEvent(funcQueue, false);
				}
			}
			else
			{
				for (int i = 0; i < transform->children.size(); i++)
				{
					transform->children[i]->GetGameObject()->EnableEvent(funcQueue, true);
				}
			}
		}
		else if (state == State::DISABLE)
		{
			if (active == true)
			{
				for (int i = 0; i < componentList.size(); i++)
				{
					funcQueue.push({ componentList[i], (&Component::OnEnable) });
				}

				for (int i = 0; i < transform->children.size(); i++)
				{
					transform->children[i]->GetGameObject()->EnableEvent(funcQueue, false);
				}
			}
		}
	}

	void GameObject::StartEvent(std::queue<std::pair<Component*, std::function<void(Component&)>>>& funcQueue, bool parentdisable)
	{
		if (state == State::CREATED)
		{
			if (parentdisable == false && active == true)
			{
				for (int i = 0; i < componentList.size(); i++)
				{
					funcQueue.push({ componentList[i], (&Component::Start) });
				}

				for (int i = 0; i < transform->children.size(); i++)
				{
					transform->children[i]->GetGameObject()->StartEvent(funcQueue, false);
				}
			}
		}
	}

	void GameObject::stateChangeEvent(bool parentdisable)
	{
		if (state == State::CREATED)
		{
			if (parentdisable == false && active == true)
			{
				state = State::ENABLE;

				for (int i = 0; i < transform->children.size(); i++)
				{
					transform->children[i]->GetGameObject()->stateChangeEvent(false);
				}
			}
		}
		else if (state == State::ENABLE)
		{
			if (destroy == true)
			{
				state = State::DESTROY;

				for (int i = 0; i < transform->children.size(); i++)
				{
					transform->children[i]->GetGameObject()->stateChangeEvent(true);
				}
			}
			else if (parentdisable == false && active == true)
			{
				for (int i = 0; i < transform->children.size(); i++)
				{
					transform->children[i]->GetGameObject()->stateChangeEvent(false);
				}
			}
			else
			{
				state = State::DISABLE;

				for (int i = 0; i < transform->children.size(); i++)
				{
					transform->children[i]->GetGameObject()->stateChangeEvent(true);
				}
			}
		}
		else if (state == State::DISABLE)
		{
			if (active == true)
			{
				state = State::ENABLE;

				for (int i = 0; i < transform->children.size(); i++)
				{
					transform->children[i]->GetGameObject()->stateChangeEvent(false);
				}
			}
		}
	}

	void GameObject::DestroyEvent(std::queue<GameObject*>& destoryQueue)
	{
		if (state == State::DESTROY)
		{
			for (int i = 0; i < componentList.size(); i++)
			{
				componentList[i]->OnDestroy();
			}
		}

		for (int i = 0; i < transform->children.size(); i++)
		{
			transform->children[i]->GetGameObject()->DestroyEvent(destoryQueue);
		}

		if (state == State::DESTROY)
		{
			destoryQueue.push(this);
		}
	}

	void GameObject::EarlyUpdateEvent()
	{
		if (state == State::ENABLE)
		{
			for (int i = 0; i < componentList.size(); i++)
			{
				componentList[i]->EarlyUpdate();
			}

			for (int i = 0; i < transform->children.size(); i++)
			{
				transform->children[i]->GetGameObject()->EarlyUpdateEvent();
			}
		}
	}

	void GameObject::UpdateEvent()
	{
		if (state == State::ENABLE)
		{
			for (int i = 0; i < componentList.size(); i++)
			{
				componentList[i]->Update();
			}

			for (int i = 0; i < transform->children.size(); i++)
			{
				transform->children[i]->GetGameObject()->UpdateEvent();
			}
		}
	}

	void GameObject::LateUpdateEvent()
	{
		if (state == State::ENABLE)
		{
			for (int i = 0; i < componentList.size(); i++)
			{
				componentList[i]->LateUpdate();
			}

			for (int i = 0; i < transform->children.size(); i++)
			{
				transform->children[i]->GetGameObject()->LateUpdateEvent();
			}
		}
	}

	void GameObject::callComponentPreInitialize_(Component* component)
	{
		component->PreInitialize_();
	}

	void GameObject::callComponentPostInitialize_(Component* component)
	{
		component->PostInitialize_();
	}

	Transform* GameObject::GetTransform()
	{
		return transform;
	}

	void GameObject::PreSerialize(json& jsonData) const
	{
		jsonData["GameObject"]["ID"] = IOManager::GetInstance().GetNewID(static_cast<void*>(const_cast<GameObject*>(this)));
		jsonData["GameObject"]["name"] = wstring_to_string(name);
		jsonData["GameObject"]["active"] = active;
		jsonData["GameObject"]["layerID"] = layerID;

		transform->PreSerialize(jsonData["GameObject"]["transform"]);

		jsonData["GameObject"]["componentList"]["size"] = componentList.size();
		for (size_t i = 0; i < componentList.size(); i++)
		{
			componentList[i]->PreSerialize(jsonData["GameObject"]["componentList"][to_string(i)]);
		}
	}

	void GameObject::PreDeserialize(const json& jsonData)
	{
		IOManager::GetInstance().SetNewAddress(jsonData["GameObject"]["ID"], static_cast<void*>(const_cast<GameObject*>(this)));

		name = string_to_wstring(jsonData["GameObject"]["name"]);
		active = jsonData["GameObject"]["active"];
		layerID = jsonData["GameObject"]["layerID"];

		transform->PreDeserialize(jsonData["GameObject"]["transform"]);

		size_t componentListSize = jsonData["GameObject"]["componentList"]["size"];
		for (size_t i = 0; i < componentListSize; i++)
		{
			Component* newcomponent = ComponentFactory::CreateComponentByName(string_to_wstring(jsonData["GameObject"]["componentList"][to_string(i)]["Component"]["type"]));
			componentList.push_back(newcomponent);

			callComponentPreInitialize_(newcomponent);

			// AddComponent
			if (newcomponent != nullptr)
			{
				newcomponent->PreDeserialize(jsonData["GameObject"]["componentList"][to_string(i)]);
			}
		}
	}

	void GameObject::PostSerialize(json& jsonData) const
	{
		transform->PostSerialize(jsonData["GameObject"]["transform"]);

		for (size_t i = 0; i < componentList.size(); i++)
		{
			componentList[i]->PostSerialize(jsonData["GameObject"]["componentList"][to_string(i)]);
		}
	}

	void GameObject::PostDeserialize(const json& jsonData)
	{
		transform->PostDeserialize(jsonData["GameObject"]["transform"]);

		size_t componentListSize = jsonData["GameObject"]["componentList"]["size"];
		for (size_t i = 0; i < componentListSize; i++)
		{
			if (componentList[i] != nullptr)
			{
				componentList[i]->PostDeserialize(jsonData["GameObject"]["componentList"][to_string(i)]);
				
				// transform.PostDeserialize로 계층정보가 완성된 후, 컴포넌트의 Initialize_를 호출한다.
				callComponentPostInitialize_(componentList[i]);
			}
		}
	}
}