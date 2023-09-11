#pragma once
#include <stack>
#include "ZeldaEngine.h"

/// <summary>
/// DO�Լ��� �� �� ���� new ���ְ�,
/// �� ���� ���� delete ���ִ� ���� ȿ�������� �ʴ�.
/// �̸� DotweenTimer�� 50�� ���� �������ְ� ������ �Ẹ��.
/// </summary>

class DotweenTimer;

class DotweenTimerPool
{
private:
	std::stack<DotweenTimer*> dotweenTimerStack;
	int maxSize;
	static DotweenTimerPool* dotweenTimerPool;

	DotweenTimerPool();
	~DotweenTimerPool();
	DotweenTimerPool(const DotweenTimerPool& ref) {}
	DotweenTimerPool& operator=(const DotweenTimerPool& ref) {}

public:
	static DotweenTimerPool* GetInstance();

	DotweenTimer* GetDotweenTimer();
	void Expand();
	void ReturnDotweenTimer(DotweenTimer* usedTimer);
};

