#include "pch.h"
#include "TimeController.h"

#include "EngineSetting.h"

namespace ZeldaEngine
{
	const float TimeController::TIMESCALE_DEFAULT = 1.0f;
	const float TimeController::TIMESCALE_STOP = 0.0f;
	const float TimeController::PHYSICS_LOOP_TIME_DEFAULT = 0.002f;
	const int TimeController::FPS_DEFAULT = 60;

	float TimeController::GetDeltaTime()
	{

#ifdef _ZELDA_ENGINE_TIMER_DEBUG_MODE
		if (EngineSetting::GetInstance().GetShowDebugInfo())
		{
			return timeScale * 1.0f / FPS_DEFAULT;
		}
#endif

		return GetDeltaTime(mainTime);
	}

	float TimeController::GetPhysicsDeltaTime()
	{

#ifdef _ZELDA_ENGINE_TIMER_DEBUG_MODE
		if (EngineSetting::GetInstance().GetShowDebugInfo())
		{
			return timeScale * 1.0f / FPS_DEFAULT;
		}
#endif

		return GetDeltaTime(physicsTime);
	}

	float TimeController::GetEventDeltaTime()
	{

#ifdef _ZELDA_ENGINE_TIMER_DEBUG_MODE
		return 1.0f / FPS_DEFAULT;
#endif

		return GetEventDeltaTime(mainTime);
	}

	void TimeController::Initialize(std::string maintime, std::string physicstime)
	{
		timeScale = TIMESCALE_DEFAULT;
		framePerSecond = FPS_DEFAULT;
		physicsLoopTime = PHYSICS_LOOP_TIME_DEFAULT;
		mainTime = maintime;
		physicsTime = physicstime;
		frameNumber = 0;
	}

	float TimeController::GetDeltaTime(std::string timename)
	{
		if (deltaTime.count(timename) == 0)
		{
			return 0.0f;
		}

		return deltaTime[timename];
	}

	float TimeController::GetEventDeltaTime(std::string timename)
	{
		if (eventDeltaTime.count(timename) == 0)
		{
			return 0.0f;
		}

		return eventDeltaTime[timename];
	}

	void TimeController::SetTimeScale(float timescale)
	{
		timeScale = timescale;
	}

	float TimeController::GetTimeScale()
	{
		return timeScale;
	}

	unsigned long long TimeController::GetFrameNumber()
	{
		return frameNumber;
	}

	void TimeController::Update(std::string timename)
	{
		std::chrono::system_clock::time_point currentTime = std::chrono::system_clock::now();

		if (timetable.count(timename) == 0)
		{
			timetable[timename] = currentTime;
		}

		std::chrono::duration<float>sec = currentTime - timetable[timename];
		deltaTime[timename] = sec.count() * timeScale;
		eventDeltaTime[timename] = sec.count();

		timetable[timename] = currentTime;
	}

	float TimeController::GetTimeDif(std::string timename)
	{
		std::chrono::system_clock::time_point currentTime = std::chrono::system_clock::now();

		if (timetable.count(timename) == 0)
		{
			return 0.0f;
		}

		std::chrono::duration<float>sec = currentTime - timetable[timename];

		return sec.count();
	}

	float TimeController::GetFPS()
	{
		return static_cast<float>(framePerSecond);
	}

	float TimeController::GetPhysicsLoopTime()
	{
		return physicsLoopTime;
	}

	void TimeController::CountFrameNumber()
	{
		frameNumber++;
	}

	void TimeController::MoveWindow()
	{
		for (auto iter = timetable.begin(); iter != timetable.end(); iter++)
		{
			Update((*iter).first);
		}
	}

	TimeController& TimeController::GetInstance()
	{
		static TimeController instance;
		return instance;
	}

	TimeController::TimeController()
	{

	}

	TimeController::~TimeController()
	{

	}
}