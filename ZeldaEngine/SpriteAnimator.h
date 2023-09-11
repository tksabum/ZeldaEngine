#pragma once

#include "ZeldaEngineAPI.h"

#include "IAnimator.h"
#include "Component.h"
#include "ISprite.h"

namespace ZeldaEngine
{
	class ZELDAENGINE_API SpriteAnimator : public Component, public IAnimator
	{
	public:
		SpriteAnimator();
		void PostInitialize_() override;
		virtual ~SpriteAnimator();

		void SetTexture(const std::vector<std::wstring>& textureNameList);

		void Awake();

		// 애니메이션 재생
		void Play();

		// 초당 재생될 애니메이션의 프레임 수를 정한다.
		void SetPlaySpeed(float speed);
		float GetPlaySpeed();

		// awake될 때 애니메이션이 재생된다.
		void SetPlayWithAwake(bool playwithawake);
		bool GetPlayWithAwake();

		// 애니메이션 재생이 끝나면 다시 반복한다.
		void SetLoop(bool loop);
		bool GetLoop();

		// 재생이 끝난 후 마지막 프레임이 유지된다.
		void SetRetainAfterPlayEnds(bool value);
		// 재생이 끝난 후 보이지 이미지를 지운다.
		void SetClearAfterPlayEnds(bool value);
		
		/// <summary>
		/// 애니메이션 재생 전 첫 프레임이 투명하다가 서서히 나타난 후 재생된다
		/// </summary>
		/// <param name="value"> FadeIn기능을 사용할지 여부 </param>
		/// <param name="fadeintime"> FadeIn되는데 걸리는 시간 </param>
		void SetFadeInBeforePlayBegins(bool value, float fadeintime = 1.0f);

		/// <summary>
		/// 애니메이션 재생 후 마지막 프레임이 서서히 투명해진다.
		/// </summary>
		/// <param name="value"> FadeOut기능을 사용할지 여부 </param>
		/// <param name="fadeouttime"> FadeOut되는데 걸리는 시간 </param>
		void SetFadeOutAfterPlayEnds(bool value, float fadeouttime = 1.0f);

		// FadeIn 이전의 지연시간
		void SetPreDelay(float delay);

		// FadeOut 이전의 지연시간
		void SetPostDelay(float delay);

		// 이미지의 재생에 EventDeltaTime을 이용하도록 설정한다.
		void SetEventAnimation(bool value);

		// 재생이 끝난 상태로 만든다.
		void Clear();

	private:
		bool isEventAnimation;
		bool isLoop; // 반복 설정
		bool isRetain; // 마지막 이미지 유지

		bool playWithAwake;
		float playSpeed; // 재생 속도
		float elapsedTime;
		float preDelay;
		float postDelay;
		float fadeOutTime;
		float fadeInTime;

		ISprite* sprite;
		std::vector<std::wstring> textureList;

		// IAnimator을(를) 통해 상속됨
		virtual void Execute() override;

		void ExecutePreDelay();
		void ExecuteFadeIn();
		void ExecuteAnimation();
		void ExecutePostDelay();
		void ExecuteFadeOut();
		void ExecuteTimeOut();

		void FindSprite();

	private:
		// Component을(를) 통해 상속됨
		virtual void PreSerialize(json& jsonData) const override;
		virtual void PreDeserialize(const json& jsonData) override;
		virtual void PostSerialize(json& jsonData) const override;
		virtual void PostDeserialize(const json& jsonData) override;

	};
}