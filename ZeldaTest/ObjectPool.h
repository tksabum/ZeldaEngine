#pragma once
#include <stack>
#include "ZeldaEngine.h"
/// <summary>
/// GameObject를 stack에 쌓아 놓고
/// top()->pop()으로 사용, push()로 사용 종료.
/// </summary>

using namespace ZeldaEngine;

class ObjectPool
{
private:
	std::stack<GameObject*> objectStack;
	int maxSize;
	static ObjectPool* objectPool;

	ObjectPool();
	~ObjectPool();
	ObjectPool(const ObjectPool& ref) {}
	ObjectPool& operator=(const ObjectPool& ref) {}

public:
	static ObjectPool* GetInstance();

	GameObject* PopGameObject();			// Stack에 있는 GameObject를 하나 반환해준다.
	void Expand();
	void ReturnGameObject(GameObject* usedObject);
};

