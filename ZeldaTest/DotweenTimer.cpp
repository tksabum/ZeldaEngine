#include "DotweenTimer.h"
#include "ZeldaEngine.h"

DotweenTimer::DotweenTimer()
	: m_ease(EaseInSine), elapsed(0), isActive(false), isRepeat(false), duration(0), delay(0), easeTime(0),
	m_xValue(0), m_yValue(0), m_zValue(0), accumulated(0), movingDistanceX(0), isDone(false)
{

}

void DotweenTimer::Init()
{
	m_xValue = 0;
	m_yValue = 0;
	m_zValue = 0;

	easeTime = 0;
	elapsed = 0;
	duration = 0;
	delay = 0;

	movingDistanceX = 0;
	movingDistanceY = 0;
	movingDistanceZ = 0;

	speed = 0;

	normalizedDistanceX = 0;
	normalizedDistanceY = 0;
	normalizedDistanceZ = 0;

	finalXvalue = 0;
	finalYvalue = 0;
	finalZvalue = 0;

	accumulated = 0;		// ���� ȸ����

	isActive = false;
	isRepeat = false;
	isDone = false;

	m_ease = EaseInSine;
	onUpdate = []() {};
	onCompleteFunction = []() {};
	onExpiration = []() {};
}


// Start�� �θ��� Ÿ�̸� on
void DotweenTimer::Start()
{
	elapsed = 0;
	isActive = true;
}

void DotweenTimer::Update()
{
	if (isActive)
	{
		elapsed += ZeldaEngine::TimeController::GetInstance().GetDeltaTime();

		if (elapsed > duration)
			elapsed = duration;

		if (elapsed > delay)
		{
			onUpdate();
		}

		if (elapsed == duration)
		{
			if (isRepeat == false)
			{
				onCompleteFunction();
				onExpiration();
				isActive = false;
				elapsed = 0;
				isDone = true;
			}
			else
			{
				onCompleteFunction();
				elapsed = 0;
			}

		}
	}
}

