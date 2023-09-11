#pragma once
#include <iostream>
#include <functional>
#include "Easing.h"

/// <summary>
/// Dotween�� �ݺ�ȣ���� �������� Ŭ����.
/// elasped? OnExpiriation?
/// Dotween ��ü�� ������ ������ ���� �� ����...
/// </summary>

class DotweenTimer
{
private:


public:
	double m_xValue;
	double m_yValue;
	double m_zValue;
	double m_wValue;

	double easeTime;
	double elapsed;
	// Ÿ�̸Ӱ� ���۵� �� ����� �ð�
	double duration;
	double delay;

	// ���� ���� �Ÿ�
	double movingDistanceX;
	double movingDistanceY;
	double movingDistanceZ;
	double movingDistanceW;
	// �ӵ�
	double speed;

	double normalizedDistanceX;
	double normalizedDistanceY;
	double normalizedDistanceZ;
	double normalizedDistanceW;

	double finalXvalue;
	double finalYvalue;
	double finalZvalue;
	double finalWvalue;

	double accumulated;		// ���� ȸ����

	bool isActive;
	bool isRepeat;
	bool isDone;			// Ÿ�̸� Vector���� ���� �Ǵ����� ���� �޽��� ���޿�

	easing_functions m_ease;

	std::function<void()> onUpdate = []() {};			
	std::function<void()> onCompleteFunction = []() {};		// duration �������� �� �����ų �Լ�.
	std::function<void()> onExpiration = []() {};			// dotween ���ο��� �������� �� �ִ� ���� �Ϸ� �� ȣ�� �Լ�

	DotweenTimer();
	void Start();
	void Update();

	void Init();


};

