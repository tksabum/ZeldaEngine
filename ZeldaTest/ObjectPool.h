#pragma once
#include <stack>
#include "ZeldaEngine.h"
/// <summary>
/// GameObject�� stack�� �׾� ����
/// top()->pop()���� ���, push()�� ��� ����.
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

	GameObject* PopGameObject();			// Stack�� �ִ� GameObject�� �ϳ� ��ȯ���ش�.
	void Expand();
	void ReturnGameObject(GameObject* usedObject);
};

