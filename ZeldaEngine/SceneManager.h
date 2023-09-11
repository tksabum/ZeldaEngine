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
		// ���忡 transform���� ���� ���ӿ�����Ʈ ����
		GameObject* CreateGameObject(std::wstring gameobjectname);
		//
		GameObject* CreateGameObject(GameObject* parentobject, std::wstring gameobjectname);

		// �������� �̿��Ͽ� ���ӿ�����Ʈ ���� (int�� ���߿� ������ Ÿ������ ������ ����)
		//void CreateGameObject(int);

		// Don't Destroy�� ������ ������Ʈ�� ������ ��� ������Ʈ�� �����Ѵ�.
		void DeleteAllGameObject();

		// Don't Destory�� ������ ��� ������Ʈ�� �����Ѵ�.
		void DeleteAllDontDestGameObject();

		Camera* GetMainCamera();
		void SetMainCamera(Camera* camera);
		void SetName(std::wstring name);
		std::wstring GetName();

		// Serializable��(��) ���� ��ӵ�
		virtual void PreSerialize(json& jsonData) const override;
		virtual void PreDeserialize(const json& jsonData) override;
		virtual void PostSerialize(json& jsonData) const override;
		virtual void PostDeserialize(const json& jsonData) override;

	private:
		// ���ӿ�����Ʈ ����
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
		// subObjectList�� Deserialize��� �ӽ÷θ� ����Ѵ�. ��Ȯ�� List�� ��� ���ؼ��� objectList�� ������ Ÿ�� ã�ƾ��Ѵ�.
		std::vector<GameObject*> subObjectList;

		Camera* mainCamera; // ���� ȭ���� ��� �� ����ϴ� ī�޶�

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