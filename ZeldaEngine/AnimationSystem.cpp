#include "pch.h"
#include "AnimationSystem.h"

#include "IAnimator.h"
#include "GameObject.h"

namespace ZeldaEngine
{
	void AnimationSystem::AddAnimator(GameObject* gameobject, IAnimator* animator)
	{
		animatorList.push_back({ gameobject, animator });
	}

	void AnimationSystem::RemoveAnimator(GameObject* gameobject, IAnimator* animator)
	{
		animatorList.erase(remove(animatorList.begin(), animatorList.end(), std::make_pair(gameobject, animator)), animatorList.end());
	}

	void AnimationSystem::ExecuteAllAnimator()
	{
		for (size_t i = 0; i < animatorList.size(); i++)
		{
			if (animatorList[i].first->GetRootActive() == true)
			{
				animatorList[i].second->Execute();
			}
		}
	}

	AnimationSystem& AnimationSystem::GetInstance()
	{
		static AnimationSystem instance;
		return instance;
	}

	AnimationSystem::AnimationSystem()
	{

	}

	AnimationSystem::~AnimationSystem()
	{

	}
}