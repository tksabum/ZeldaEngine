#include "pch.h"
#include "Animator.h"

#include "GameObject.h"
#include "transform.h"
#include "SkinnedMeshRenderer.h"
#include "TimeController.h"

#include <string>

namespace ZeldaEngine 
{
	Animator::Animator()
	{
		static unsigned __int64 ID = 1;
		id = ID;
		ID++;
	}

	Animator::~Animator()
	{

	}

	void Animator::Awake()
	{
		for (const auto& child : gameObject->GetTransform()->GetChildren())
		{
			SkinnedMeshRenderer* renderer = child->GetGameObject()->GetComponent<SkinnedMeshRenderer>();

			if (renderer)
			{
				meshName = renderer->GetMeshName();
			}
		}
	}

	void Animator::Update()
	{
		updateTime += TimeController::GetInstance().GetDeltaTime();

		const FF::FFAnimationClip& animClip = animationMap[currentAnimation];

		if (updateTime >= animClip.duration)
		{
			if (animClip.isLoop)
			{
				updateTime = 0.f;
			}
		}

		__int32 ratio = static_cast<__int32>(animClip.frameCount / animClip.duration);
		frame = static_cast<__int32>(updateTime * ratio);
		frame = min(frame, animClip.frameCount - 1);
		nextFrame = min(frame + 1, animClip.frameCount - 1);
		frameRatio = static_cast<float>(updateTime - static_cast<float>(frame) / ratio);
	}

	void Animator::PlusAnimationClip(const FF::FFAnimationClip& animationclip)
	{
		animationMap.insert({ animationclip.animName, animationclip });
	}

	void Animator::Play(const std::wstring& animationname)
	{
		currentAnimation = animationname;
		updateTime = 0.f;

		const FF::FFAnimationClip& animClip = animationMap[currentAnimation];

		__int32 ratio = static_cast<__int32>(animClip.frameCount / animClip.duration);
		frame = static_cast<__int32>(updateTime * ratio);
		frame = min(frame, animClip.frameCount - 1);
		nextFrame = min(frame + 1, animClip.frameCount - 1);
		frameRatio = static_cast<float>(updateTime - static_cast<float>(frame) / ratio);
	}

	void Animator::PreSerialize(json& jsonData) const
	{
		// TODO
		PRESERIALIZE_BASE(Animator);

		PRESERIALIZE_WSTRVALUE(meshName);
		PRESERIALIZE_WSTRVALUE(currentAnimation);

		// animationclip
		size_t clipCounter = 0;
		for (auto iter = animationMap.begin(); iter != animationMap.end(); iter++)
		{
			jsonData["Component"]["animationMap"][std::to_string(clipCounter)]["animationName"] = wstring_to_string((*iter).first);
			jsonData["Component"]["animationMap"][std::to_string(clipCounter)]["animationClipName"] = wstring_to_string((*iter).second.animName);
			jsonData["Component"]["animationMap"][std::to_string(clipCounter)]["animationClipDuration"] = ((*iter).second.duration);
			jsonData["Component"]["animationMap"][std::to_string(clipCounter)]["animationClipFrameCount"] = ((*iter).second.frameCount);
			jsonData["Component"]["animationMap"][std::to_string(clipCounter)]["animationClipLoop"] = ((*iter).second.isLoop);
			clipCounter++;
		}
		jsonData["Component"]["animationMap"]["size"] = clipCounter;
	}

	void Animator::PreDeserialize(const json& jsonData)
	{
		// TODO
		PREDESERIALIZE_BASE(Animator);

		PREDESERIALIZE_WSTRVALUE(meshName);
		PREDESERIALIZE_WSTRVALUE(currentAnimation);

		// texture
		size_t clipCount = jsonData["Component"]["animationMap"]["size"];
		for (size_t i = 0; i < clipCount; i++)
		{
			FF::FFAnimationClip value;

			std::wstring key = string_to_wstring(jsonData["Component"]["animationMap"][std::to_string(i)]["animationName"]);

			value.animName = string_to_wstring(jsonData["Component"]["animationMap"][std::to_string(i)]["animationClipName"]);
			value.duration = (jsonData["Component"]["animationMap"][std::to_string(i)]["animationClipDuration"]);
			value.frameCount = (jsonData["Component"]["animationMap"][std::to_string(i)]["animationClipFrameCount"]);
			value.isLoop = (jsonData["Component"]["animationMap"][std::to_string(i)]["animationClipLoop"]);

			animationMap.insert({ key,value });
		}
	}

	void Animator::PostSerialize(json& jsonData) const
	{
		// TODO
		POSTSERIALIZE_BASE();
	}

	void Animator::PostDeserialize(const json& jsonData)
	{
		// TODO
		POSTDESERIALIZE_BASE();
	}
}

