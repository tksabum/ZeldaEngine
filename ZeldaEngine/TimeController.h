#pragma once

#include "ZeldaEngineAPI.h"

#include <chrono>
#include <string>
#include <unordered_map>

namespace ZeldaEngine
{
	class CoreSystem;

	class ZELDAENGINE_API TimeController
	{
	public:
		static const float TIMESCALE_DEFAULT;
		static const float TIMESCALE_STOP;
		static const float PHYSICS_LOOP_TIME_DEFAULT;
		static const int FPS_DEFAULT;

		float GetDeltaTime();
		float GetPhysicsDeltaTime();

		float GetEventDeltaTime();

		void SetTimeScale(float timescale);
		float GetTimeScale();

		unsigned long long GetFrameNumber();

	private:
		float timeScale; // deltaTime의 크기를 조정하여 게임에서 흐르는 시간을 조정할 수 있다.
		std::unordered_map<std::string, std::chrono::system_clock::time_point> timetable;
		std::unordered_map<std::string, float> deltaTime;
		std::unordered_map<std::string, float> eventDeltaTime;
		int framePerSecond; // 목표 초당 프레임 수
		float physicsLoopTime;
		unsigned long long frameNumber;

		std::string mainTime;
		std::string physicsTime;

		void Initialize(std::string maintime, std::string physicstime);

		float GetDeltaTime(std::string timename);
		float GetEventDeltaTime(std::string timename);

		// 현재시간을 기록하고 이전시간과의 차이를 deltaTime에 저장해둔다.
		void Update(std::string timename);
		// 현재시간과 이전시간과의 시간차이(timeScale과 관계없음)를 반환받음
		float GetTimeDif(std::string timename);

		float GetFPS();
		float GetPhysicsLoopTime();

		void CountFrameNumber();
		void MoveWindow();

		friend CoreSystem;
		friend class GraphicsSystem;

		//singleton
	public:
		static TimeController& GetInstance();

	private:
		TimeController();
		~TimeController();
		TimeController(const TimeController& ref);
		TimeController& operator=(const TimeController& ref) = delete;
	};
}