#pragma once

#include "ZeldaEngineAPI.h"

#include "Serializable.h"

#include <string>
#include <vector>
#include <unordered_map>
#include <queue>
#include <functional>

namespace ZeldaEngine
{
	class Scene;
	class CoreSystem;
	class TimeController;
	class GameObject;
	class Component;
	class Camera;
	class Transform;

	class ZELDAENGINE_API SceneManager : public Serializable
	{
	public:
		// 월드에 transform만을 가진 게임오브젝트 생성
		GameObject* CreateGameObject(std::wstring gameobjectname);
		//
		GameObject* CreateGameObject(GameObject* parentobject, std::wstring gameobjectname);

		// 프리펩을 이용하여 게임오브젝트 생성 (int는 나중에 적절한 타입으로 변경할 예정)
		//void CreateGameObject(int);

		// Don't Destroy가 설정된 오브젝트를 제외한 모든 오브젝트를 제거한다.
		void DeleteAllGameObject();

		// Don't Destory가 설정된 모든 오브젝트를 제거한다.
		void DeleteAllDontDestGameObject();

		Camera* GetMainCamera();
		void SetMainCamera(Camera* camera);
		void SetName(std::wstring name);
		std::wstring GetName();

		// Serializable을(를) 통해 상속됨
		virtual void PreSerialize(json& jsonData) const override;
		virtual void PreDeserialize(const json& jsonData) override;
		virtual void PostSerialize(json& jsonData) const override;
		virtual void PostDeserialize(const json& jsonData) override;

	private:
		// 게임오브젝트 제거
		void DeleteGameObject(GameObject* gameobject);

		void Initialize();

		void InitializeEvent();
		void DestroyEvent();
		void PhysicsEvent();
		void EarlyUpdateEvent();
		void UpdateEvent();
		void LateUpdateEvent();
		void RenderEvent();

		std::wstring sceneName;
		std::vector<GameObject*> gameobjectList;
		std::vector<GameObject*> dontDestroyObjectList;
		// subObjectList는 Deserialize등에서 임시로만 사용한다. 정확한 List를 얻기 위해서는 objectList의 계층을 타고 찾아야한다.
		std::vector<GameObject*> subObjectList;

		Camera* mainCamera; // 현재 화면을 띄울 때 사용하는 카메라

		void ExecuteEventQueue();
		std::queue<std::pair<Component*, std::function<void(Component&)>>> eventQueue;

		void ExcuteDestroyQueue();
		std::queue<GameObject*> destroyedGameObject;

		friend CoreSystem;
		friend GameObject;
		friend Transform;

		//singleton
	public:
		static SceneManager& GetInstance();

	private:
		SceneManager();
		~SceneManager();
		SceneManager(const SceneManager& ref);
		SceneManager& operator=(const SceneManager& ref) = delete;
	};
}