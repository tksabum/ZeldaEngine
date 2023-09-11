#pragma once

#include <vector>

namespace ZeldaEngine
{
	class IAnimator;
	class GameObject;

	class AnimationSystem
	{
	public:
		void AddAnimator(GameObject* gameobject, IAnimator* animator);
		void RemoveAnimator(GameObject* gameobject, IAnimator* animator);

		void ExecuteAllAnimator();

	private:
		std::vector<std::pair<GameObject*, IAnimator*>> animatorList;


		//singleton
	public:
		static AnimationSystem& GetInstance();

	private:
		AnimationSystem();
		~AnimationSystem();
		AnimationSystem(const AnimationSystem& ref) = delete;
		AnimationSystem& operator=(const AnimationSystem& ref) = delete;
	};
}