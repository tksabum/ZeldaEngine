#include "ObjectPool.h"

using namespace ZeldaEngine;

ObjectPool* ObjectPool::objectPool = nullptr;

ObjectPool::ObjectPool()
{
	maxSize = 50;
	Expand();
}

ObjectPool::~ObjectPool()
{
	while (!objectStack.empty())
	{
		GameObject* object = objectStack.top();
		objectStack.pop();
		object->Destroy();
	}
}

ObjectPool* ObjectPool::GetInstance()
{
	if (objectPool == nullptr)
		objectPool = new ObjectPool();

	return objectPool;
}

GameObject* ObjectPool::PopGameObject()
{
	if (objectStack.empty())
	{
		Expand();
	}
	GameObject* tempObject = objectStack.top();
	objectStack.pop();
	return tempObject;
}

void ObjectPool::Expand()
{
	for (int i = 0; i < maxSize; i++)
	{
		GameObject* object = SceneManager::GetInstance().CreateGameObject(L"MadeByPool");
		objectStack.push(object);
	}
	maxSize += maxSize;
}

void ObjectPool::ReturnGameObject(GameObject* usedObject)
{
	// Pool에 넣어주기 전에 disAble? 같은거 해줘야 하지 않나...
	usedObject->SetActive(false);
	objectStack.push(usedObject);
}
