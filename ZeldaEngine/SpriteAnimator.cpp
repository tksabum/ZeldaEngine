#include "pch.h"
#include "SpriteAnimator.h"

#include "GameObject.h"

#include "SpriteRenderer.h"
#include "Image.h"

#include "ResourceManager.h"

#include "AnimationSystem.h"

#include "TimeController.h"

namespace ZeldaEngine
{
	SpriteAnimator::SpriteAnimator() :
		sprite(nullptr),
		isEventAnimation(false),
		isLoop(false),
		isRetain(true),
		playWithAwake(false),
		playSpeed(1.0f),
		elapsedTime(0.0f),
		preDelay(0.0f),
		postDelay(0.0f),
		fadeOutTime(0.0f),
		fadeInTime(0.0f)
	{

	}

	void SpriteAnimator::PostInitialize_()
	{
		AnimationSystem::GetInstance().AddAnimator(GetGameObject(), this);

		Clear();
	}

	SpriteAnimator::~SpriteAnimator()
	{
		AnimationSystem::GetInstance().RemoveAnimator(GetGameObject(), this);
	}


	void SpriteAnimator::SetTexture(const std::vector<std::wstring>& textureNameList)
	{
		textureList.clear();

		for (auto textureName : textureNameList)
		{
			textureList.push_back(textureName);
		}

		Clear();
	}

	void SpriteAnimator::Awake()
	{
		if (sprite == nullptr)
		{
			FindSprite();
		}

		if (playWithAwake)
		{
			Play();
		}
	}

	void SpriteAnimator::Play()
	{
		if (sprite == nullptr)
		{
			FindSprite();
			elapsedTime = 0.0f;
		}
		else
		{
			elapsedTime = 0.0f;
		}
	}

	void SpriteAnimator::SetPlaySpeed(float speed)
	{
		playSpeed = speed;
	}

	float SpriteAnimator::GetPlaySpeed()
	{
		return playSpeed;
	}

	void SpriteAnimator::SetPlayWithAwake(bool playwithawake)
	{
		playWithAwake = playwithawake;
	}

	bool SpriteAnimator::GetPlayWithAwake()
	{
		return playWithAwake;
	}

	void SpriteAnimator::SetLoop(bool loop)
	{
		isLoop = loop;
	}

	bool SpriteAnimator::GetLoop()
	{
		return isLoop;
	}

	void SpriteAnimator::SetRetainAfterPlayEnds(bool value)
	{
		isRetain = value;
		Clear();
	}

	void SpriteAnimator::SetClearAfterPlayEnds(bool value)
	{
		if (value)
		{
			isRetain = false;
			fadeOutTime = 0.0f;
		}
		Clear();
	}

	void SpriteAnimator::SetFadeInBeforePlayBegins(bool value, float fadeintime)
	{
		if (value)
		{
			fadeInTime = fadeintime;
		}
		else
		{
			fadeInTime = 0.0f;
		}
		Clear();
	}

	void SpriteAnimator::SetFadeOutAfterPlayEnds(bool value, float fadeouttime)
	{
		if (value)
		{
			isRetain = false;
			fadeOutTime = fadeouttime;
		}
		else
		{
			fadeOutTime = 0.0f;
		}

		Clear();
	}

	void SpriteAnimator::SetPreDelay(float delay)
	{
		preDelay = max(0.0f, delay);
		Clear();
	}

	void SpriteAnimator::SetPostDelay(float delay)
	{
		postDelay = max(0.0f, delay);
		Clear();
	}

	void SpriteAnimator::SetEventAnimation(bool value)
	{
		isEventAnimation = value;
	}

	void SpriteAnimator::Clear()
	{
		elapsedTime = fadeInTime + preDelay + (textureList.size() / playSpeed) + postDelay + fadeOutTime;
	}


	void SpriteAnimator::Execute()
	{
		if (sprite != nullptr)
		{
			// 텍스쳐가 없다면 종료
			if (textureList.size() == 0) return;

			float deltaTime = TimeController::GetInstance().GetDeltaTime();

			if (isEventAnimation)
			{
				deltaTime = TimeController::GetInstance().GetEventDeltaTime();
			}

			elapsedTime += deltaTime;

			// 애니메이션만의 재생시간
			float playTime = textureList.size() / playSpeed;

			if (isLoop && elapsedTime > fadeInTime + preDelay + playTime + postDelay + fadeOutTime)
			{
				elapsedTime -= fadeInTime + preDelay + playTime + postDelay + fadeOutTime;
			}

			if (elapsedTime < preDelay)
			{
				ExecutePreDelay();
			}
			else if (elapsedTime < preDelay + fadeInTime)
			{
				ExecuteFadeIn();
			}
			else if (elapsedTime < preDelay + fadeInTime + playTime)
			{
				ExecuteAnimation();
			}
			else if (elapsedTime < preDelay + fadeInTime + playTime + postDelay)
			{
				ExecutePostDelay();
			}
			else if (elapsedTime < preDelay + fadeInTime + playTime + postDelay + fadeOutTime)
			{
				ExecuteFadeOut();
			}
			else
			{
				ExecuteTimeOut();
			}
		}
		else
		{
			FindSprite();
		}
	}

	void SpriteAnimator::ExecutePreDelay()
	{
		sprite->SetAlpha(0.0f);
		sprite->SetTexture(textureList[0]);
	}

	void SpriteAnimator::ExecuteFadeIn()
	{
		float faderate = 1.0f - ((preDelay + fadeInTime - elapsedTime) / fadeInTime);

		sprite->SetAlpha(faderate);
		sprite->SetTexture(textureList[0]);
	}

	void SpriteAnimator::ExecuteAnimation()
	{
		float playTime = textureList.size() / playSpeed;
		size_t currentTexNum = min(static_cast<size_t>((elapsedTime - (preDelay + fadeInTime)) * playSpeed), textureList.size() - 1);
		
		sprite->SetAlpha(1.0f);
		sprite->SetTexture(textureList[currentTexNum]);
	}

	void SpriteAnimator::ExecutePostDelay()
	{
		sprite->SetAlpha(1.0f);
		sprite->SetTexture(textureList[textureList.size() - 1]);
	}

	void SpriteAnimator::ExecuteFadeOut()
	{
		float playTime = textureList.size() / playSpeed;
		float fadeouttotal = fadeInTime + preDelay + playTime + postDelay + fadeOutTime;

		float faderate = (fadeouttotal - elapsedTime) / fadeOutTime;

		sprite->SetAlpha(faderate);
		sprite->SetTexture(textureList[textureList.size() - 1]);
	}

	void SpriteAnimator::ExecuteTimeOut()
	{
		if (isRetain)
		{
			sprite->SetAlpha(1.0f);
			sprite->SetTexture(textureList[textureList.size() - 1]);
		}
		else
		{
			sprite->SetAlpha(0.0f);
		}
	}

	void SpriteAnimator::FindSprite()
	{
		assert(sprite == nullptr);

		sprite = GetGameObject()->GetComponent<SpriteRenderer>();

		if (sprite == nullptr)
		{
			sprite = GetGameObject()->GetComponent<Image>();
		}
	}

	void SpriteAnimator::PreSerialize(json& jsonData) const
	{
		PRESERIALIZE_BASE(SpriteAnimator);

		PRESERIALIZE_VALUE(isEventAnimation);
		PRESERIALIZE_VALUE(isLoop);
		PRESERIALIZE_VALUE(isRetain);
		PRESERIALIZE_VALUE(playWithAwake);
		PRESERIALIZE_VALUE(playSpeed);
		PRESERIALIZE_VALUE(preDelay);
		PRESERIALIZE_VALUE(postDelay);
		PRESERIALIZE_VALUE(fadeOutTime);
		PRESERIALIZE_VALUE(fadeInTime);

		for (size_t i = 0; i < textureList.size(); i++)
		{
			jsonData["Component"]["textureList"][std::to_string(i)] = wstring_to_string(textureList[i]);
		}
		jsonData["Component"]["textureList"]["size"] = textureList.size();
	}

	void SpriteAnimator::PreDeserialize(const json& jsonData)
	{
		PREDESERIALIZE_BASE(SpriteAnimator);

		PREDESERIALIZE_VALUE(isEventAnimation);
		PREDESERIALIZE_VALUE(isLoop);
		PREDESERIALIZE_VALUE(isRetain);
		PREDESERIALIZE_VALUE(playWithAwake);
		PREDESERIALIZE_VALUE(playSpeed);
		PREDESERIALIZE_VALUE(preDelay);
		PREDESERIALIZE_VALUE(postDelay);
		PREDESERIALIZE_VALUE(fadeOutTime);
		PREDESERIALIZE_VALUE(fadeInTime);


		textureList.clear();
		size_t valuesize = jsonData["Component"]["textureList"]["size"];
		for (size_t i = 0; i < valuesize; i++)
		{
			textureList.push_back(string_to_wstring(jsonData["Component"]["textureList"][std::to_string(i)]));
		}
	}

	void SpriteAnimator::PostSerialize(json& jsonData) const
	{
		POSTSERIALIZE_BASE();
	}

	void SpriteAnimator::PostDeserialize(const json& jsonData)
	{
		POSTDESERIALIZE_BASE();
	}
}