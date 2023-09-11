#include "pch.h"
#include "SceneManager.h"

#include "GameObject.h"
#include "Transform.h"
#include "Camera.h"

#include "EngineSetting.h"

using namespace std;

namespace ZeldaEngine
{
	GameObject* SceneManager::CreateGameObject(std::wstring gameobjectname)
	{
		GameObject* object = new GameObject(gameobjectname);
		gameobjectList.push_back(object);
		return object;
	}

	GameObject* SceneManager::CreateGameObject(GameObject* parentobject, std::wstring gameobjectname)
	{
		GameObject* result = nullptr;

		for (int i = 0; i < gameobjectList.size(); i++)
		{
			result = gameobjectList[i]->AddGameObject(parentobject, gameobjectname);
			if (result != nullptr)
			{
				// parentobject에 object를 추가하는데 성공
				break;
			}
		}

		return result;
	}

	void SceneManager::DeleteGameObject(GameObject* gameobject)
	{
		size_t gameobjectNumber = find(gameobjectList.begin(), gameobjectList.end(), gameobject) - gameobjectList.begin();
		if (gameobjectNumber < gameobjectList.size())
		{
			delete gameobjectList[gameobjectNumber];
			gameobjectList.erase(gameobjectList.begin() + gameobjectNumber);
			
			return;
		}

		size_t dontdestobjectNumber = find(dontDestroyObjectList.begin(), dontDestroyObjectList.end(), gameobject) - dontDestroyObjectList.begin();
		if (dontdestobjectNumber < dontDestroyObjectList.size())
		{
			delete dontDestroyObjectList[dontdestobjectNumber];
			dontDestroyObjectList.erase(dontDestroyObjectList.begin() + dontdestobjectNumber);

			return;
		}

		for (size_t i = 0; i < gameobjectList.size(); i++)
		{
			gameobjectList[i]->DeleteChild(gameobject);
		}

		for (size_t i = 0; i < dontDestroyObjectList.size(); i++)
		{
			dontDestroyObjectList[i]->DeleteChild(gameobject);
		}
	}

	void SceneManager::DeleteAllGameObject()
	{
		for (size_t i = 0; i < gameobjectList.size(); i++)
		{
			delete gameobjectList[i];
		}
		gameobjectList.clear();
	}

	void SceneManager::DeleteAllDontDestGameObject()
	{
		for (size_t i = 0; i < dontDestroyObjectList.size(); i++)
		{
			delete dontDestroyObjectList[i];
		}
		dontDestroyObjectList.clear();
	}

	Camera* SceneManager::GetMainCamera()
	{
		return mainCamera;
	}

	void SceneManager::SetMainCamera(Camera* camera)
	{
		mainCamera = camera;
	}

	void SceneManager::SetName(std::wstring name)
	{
		sceneName = name;
	}

	std::wstring SceneManager::GetName()
	{
		return sceneName;
	}

	void SceneManager::PreSerialize(json& jsonData) const
	{
		jsonData["SceneManager"]["sceneName"] = wstring_to_string(sceneName);

		jsonData["SceneManager"]["gameobjectList"]["size"] = gameobjectList.size();
		for (size_t i = 0; i < gameobjectList.size(); i++)
		{
			gameobjectList[i]->PreSerialize(jsonData["SceneManager"]["gameobjectList"][to_string(i)]);
		}

		jsonData["SceneManager"]["dontDestroyObjectList"]["size"] = dontDestroyObjectList.size();
		for (size_t i = 0; i < dontDestroyObjectList.size(); i++)
		{
			dontDestroyObjectList[i]->PreSerialize(jsonData["SceneManager"]["dontDestroyObjectList"][to_string(i)]);
		}

		// SceneManager가 직접적으로 가지고 있지 않은 오브젝트 처리
		queue<GameObject*> objectqueue;
		for (size_t i = 0; i < gameobjectList.size(); i++)
		{
			objectqueue.push(gameobjectList[i]);
		}

		for (size_t i = 0; i < dontDestroyObjectList.size(); i++)
		{
			objectqueue.push(dontDestroyObjectList[i]);
		}

		int subObjectListCounter = 0;
		while (!objectqueue.empty())
		{
			GameObject* now = objectqueue.front();
			objectqueue.pop();
			vector<Transform*> nowchildren = now->transform->GetChildren();
			for (size_t i = 0; i < nowchildren.size(); i++)
			{
				objectqueue.push(nowchildren[i]->GetGameObject());

				// 여기에 각각의 오브젝트인 nowchildren[i]가 할 동작 구현
				nowchildren[i]->GetGameObject()->PreSerialize(jsonData["SceneManager"]["subObjectList"][to_string(subObjectListCounter)]);
				subObjectListCounter++;
			}
		}

		jsonData["SceneManager"]["subObjectList"]["size"] = subObjectListCounter;
	}

	void SceneManager::PreDeserialize(const json& jsonData)
	{
		sceneName = string_to_wstring(jsonData["SceneManager"]["sceneName"]);

		size_t objectListSize = jsonData["SceneManager"]["gameobjectList"]["size"];
		for (size_t i = 0; i < objectListSize; i++)
		{
			GameObject* gameobject = new GameObject(L"");
			gameobjectList.push_back(gameobject);
			gameobject->PreDeserialize(jsonData["SceneManager"]["gameobjectList"][to_string(i)]);
		}

		size_t dontDestObjectListSize = jsonData["SceneManager"]["dontDestroyObjectList"]["size"];
		for (size_t i = 0; i < dontDestObjectListSize; i++)
		{
			GameObject* gameobject = new GameObject(L"");
			dontDestroyObjectList.push_back(gameobject);
			gameobject->PreDeserialize(jsonData["SceneManager"]["dontDestroyObjectList"][to_string(i)]);
		}

		subObjectList.clear();
		size_t subObjectListSize = jsonData["SceneManager"]["subObjectList"]["size"];
		for (size_t i = 0; i < subObjectListSize; i++)
		{
			GameObject* gameobject = new GameObject(L"");
			subObjectList.push_back(gameobject);
			gameobject->PreDeserialize(jsonData["SceneManager"]["subObjectList"][to_string(i)]);
		}
	}

	void SceneManager::PostSerialize(json& jsonData) const
	{
		jsonData["SceneManager"]["mainCameraID"] = IOManager::GetInstance().GetID(static_cast<void*>(mainCamera));

		for (size_t i = 0; i < gameobjectList.size(); i++)
		{
			gameobjectList[i]->PostSerialize(jsonData["SceneManager"]["gameobjectList"][to_string(i)]);
		}

		for (size_t i = 0; i < dontDestroyObjectList.size(); i++)
		{
			dontDestroyObjectList[i]->PostSerialize(jsonData["SceneManager"]["dontDestroyObjectList"][to_string(i)]);
		}

		// SceneManager가 직접적으로 가지고 있지 않은 오브젝트 처리
		queue<GameObject*> objectqueue;
		for (size_t i = 0; i < gameobjectList.size(); i++)
		{
			objectqueue.push(gameobjectList[i]);
		}

		for (size_t i = 0; i < dontDestroyObjectList.size(); i++)
		{
			objectqueue.push(dontDestroyObjectList[i]);
		}

		int subObjectListCounter = 0;
		while (!objectqueue.empty())
		{
			GameObject* now = objectqueue.front();
			objectqueue.pop();
			vector<Transform*> nowchildren = now->transform->GetChildren();
			for (size_t i = 0; i < nowchildren.size(); i++)
			{
				objectqueue.push(nowchildren[i]->GetGameObject());

				// 여기에 각각의 오브젝트인 nowchildren[i]가 할 동작 구현
				nowchildren[i]->GetGameObject()->PostSerialize(jsonData["SceneManager"]["subObjectList"][to_string(subObjectListCounter)]);
				subObjectListCounter++;
			}
		}
	}

	void SceneManager::PostDeserialize(const json& jsonData)
	{
		mainCamera = static_cast<Camera*>(IOManager::GetInstance().GetAddress(jsonData["SceneManager"]["mainCameraID"]));

		for (size_t i = 0; i < gameobjectList.size(); i++)
		{
			gameobjectList[i]->PostDeserialize(jsonData["SceneManager"]["gameobjectList"][to_string(i)]);
		}

		for (size_t i = 0; i < dontDestroyObjectList.size(); i++)
		{
			dontDestroyObjectList[i]->PostDeserialize(jsonData["SceneManager"]["dontDestroyObjectList"][to_string(i)]);
		}

		for (size_t i = 0; i < subObjectList.size(); i++)
		{
			subObjectList[i]->PostDeserialize(jsonData["SceneManager"]["subObjectList"][to_string(i)]);
		}
	}

	void SceneManager::Initialize()
	{
		DeleteAllGameObject();
		if (mainCamera == nullptr)
		{
			GameObject* object = CreateGameObject(L"MainCamera");
			Camera* tmpcamera = object->AddComponent<Camera>();
			mainCamera = tmpcamera;
		}
	}

	void SceneManager::InitializeEvent()
	{
		// Disable Event
		for (size_t i = 0; i < gameobjectList.size(); i++)
		{
			gameobjectList[i]->DisableEvent(eventQueue);
		}

		for (size_t i = 0; i < dontDestroyObjectList.size(); i++)
		{
			dontDestroyObjectList[i]->DisableEvent(eventQueue);
		}

		ExecuteEventQueue();

		// Awake Event
		for (size_t i = 0; i < gameobjectList.size(); i++)
		{
			gameobjectList[i]->AwakeEvent(eventQueue);
		}

		for (size_t i = 0; i < dontDestroyObjectList.size(); i++)
		{
			dontDestroyObjectList[i]->AwakeEvent(eventQueue);
		}

		ExecuteEventQueue();

		// Enable Event
		for (size_t i = 0; i < gameobjectList.size(); i++)
		{
			gameobjectList[i]->EnableEvent(eventQueue);
		}

		for (size_t i = 0; i < dontDestroyObjectList.size(); i++)
		{
			dontDestroyObjectList[i]->EnableEvent(eventQueue);
		}

		ExecuteEventQueue();

		// Start Event
		for (size_t i = 0; i < gameobjectList.size(); i++)
		{
			gameobjectList[i]->StartEvent(eventQueue);
		}
			
		for (size_t i = 0; i < dontDestroyObjectList.size(); i++)
		{
			dontDestroyObjectList[i]->StartEvent(eventQueue);
		}

		ExecuteEventQueue();

		// State Change Event
		for (size_t i = 0; i < gameobjectList.size(); i++)
		{
			gameobjectList[i]->stateChangeEvent();
		}

		for (size_t i = 0; i < dontDestroyObjectList.size(); i++)
		{
			dontDestroyObjectList[i]->stateChangeEvent();
		}
	}

	void SceneManager::DestroyEvent()
	{
		for (size_t i = 0; i < gameobjectList.size(); i++)
		{
			gameobjectList[i]->DestroyEvent(destroyedGameObject);
		}

		for (size_t i = 0; i < dontDestroyObjectList.size(); i++)
		{
			dontDestroyObjectList[i]->DestroyEvent(destroyedGameObject);
		}

		ExcuteDestroyQueue();
	}

	void SceneManager::PhysicsEvent()
	{
		for (size_t i = 0; i < gameobjectList.size(); i++)
		{
			//gameobjectList[i]->PhysicsEvent();
		}

		for (size_t i = 0; i < dontDestroyObjectList.size(); i++)
		{
			//dontDestroyObjectList[i]->PhysicsEvent();
		}
	}

	void SceneManager::EarlyUpdateEvent()
	{
		for (size_t i = 0; i < gameobjectList.size(); i++)
		{
			gameobjectList[i]->EarlyUpdateEvent();
		}

		for (size_t i = 0; i < dontDestroyObjectList.size(); i++)
		{
			dontDestroyObjectList[i]->EarlyUpdateEvent();
		}
	}

	void SceneManager::UpdateEvent()
	{
		for (size_t i = 0; i < gameobjectList.size(); i++)
		{
			gameobjectList[i]->UpdateEvent();
		}

		for (size_t i = 0; i < dontDestroyObjectList.size(); i++)
		{
			dontDestroyObjectList[i]->UpdateEvent();
		}
	}

	void SceneManager::LateUpdateEvent()
	{
		for (size_t i = 0; i < gameobjectList.size(); i++)
		{
			gameobjectList[i]->LateUpdateEvent();
		}

		for (size_t i = 0; i < dontDestroyObjectList.size(); i++)
		{
			dontDestroyObjectList[i]->LateUpdateEvent();
		}
	}

	void SceneManager::RenderEvent()
	{
		for (size_t i = 0; i < gameobjectList.size(); i++)
		{
			//gameobjectList[i]->RenderEvent();
		}

		for (size_t i = 0; i < dontDestroyObjectList.size(); i++)
		{
			//dontDestroyObjectList[i]->RenderEvent();
		}
	}

	void SceneManager::ExecuteEventQueue()
	{
		while (!eventQueue.empty())
		{
			Component* component = eventQueue.front().first;
			auto f = eventQueue.front().second;
			eventQueue.pop();

			f(*component);
		}
	}

	void SceneManager::ExcuteDestroyQueue()
	{
		while (!destroyedGameObject.empty())
		{
			GameObject* object = destroyedGameObject.front();
			destroyedGameObject.pop();

			DeleteGameObject(object);
		}
	}

	SceneManager& SceneManager::GetInstance()
	{
		static SceneManager instance;
		return instance;
	}

	SceneManager::SceneManager() :
		sceneName(L"SampleScene"),
		mainCamera(nullptr)
	{

	}

	SceneManager::~SceneManager()
	{

	}
}