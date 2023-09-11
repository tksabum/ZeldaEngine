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
		float timeScale; // deltaTime�� ũ�⸦ �����Ͽ� ���ӿ��� �帣�� �ð��� ������ �� �ִ�.
		std::unordered_map<std::string, std::chrono::system_clock::time_point> timetable;
		std::unordered_map<std::string, float> deltaTime;
		std::unordered_map<std::string, float> eventDeltaTime;
		int framePerSecond; // ��ǥ �ʴ� ������ ��
		float physicsLoopTime;
		unsigned long long frameNumber;

		std::string mainTime;
		std::string physicsTime;

		void Initialize(std::string maintime, std::string physicstime);

		float GetDeltaTime(std::string timename);
		float GetEventDeltaTime(std::string timename);

		// ����ð��� ����ϰ� �����ð����� ���̸� deltaTime�� �����صд�.
		void Update(std::string timename);
		// ����ð��� �����ð����� �ð�����(timeScale�� �������)�� ��ȯ����
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