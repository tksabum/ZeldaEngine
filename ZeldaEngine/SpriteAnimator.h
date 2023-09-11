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

		// �ִϸ��̼� ���
		void Play();

		// �ʴ� ����� �ִϸ��̼��� ������ ���� ���Ѵ�.
		void SetPlaySpeed(float speed);
		float GetPlaySpeed();

		// awake�� �� �ִϸ��̼��� ����ȴ�.
		void SetPlayWithAwake(bool playwithawake);
		bool GetPlayWithAwake();

		// �ִϸ��̼� ����� ������ �ٽ� �ݺ��Ѵ�.
		void SetLoop(bool loop);
		bool GetLoop();

		// ����� ���� �� ������ �������� �����ȴ�.
		void SetRetainAfterPlayEnds(bool value);
		// ����� ���� �� ������ �̹����� �����.
		void SetClearAfterPlayEnds(bool value);
		
		/// <summary>
		/// �ִϸ��̼� ��� �� ù �������� �����ϴٰ� ������ ��Ÿ�� �� ����ȴ�
		/// </summary>
		/// <param name="value"> FadeIn����� ������� ���� </param>
		/// <param name="fadeintime"> FadeIn�Ǵµ� �ɸ��� �ð� </param>
		void SetFadeInBeforePlayBegins(bool value, float fadeintime = 1.0f);

		/// <summary>
		/// �ִϸ��̼� ��� �� ������ �������� ������ ����������.
		/// </summary>
		/// <param name="value"> FadeOut����� ������� ���� </param>
		/// <param name="fadeouttime"> FadeOut�Ǵµ� �ɸ��� �ð� </param>
		void SetFadeOutAfterPlayEnds(bool value, float fadeouttime = 1.0f);

		// FadeIn ������ �����ð�
		void SetPreDelay(float delay);

		// FadeOut ������ �����ð�
		void SetPostDelay(float delay);

		// �̹����� ����� EventDeltaTime�� �̿��ϵ��� �����Ѵ�.
		void SetEventAnimation(bool value);

		// ����� ���� ���·� �����.
		void Clear();

	private:
		bool isEventAnimation;
		bool isLoop; // �ݺ� ����
		bool isRetain; // ������ �̹��� ����

		bool playWithAwake;
		float playSpeed; // ��� �ӵ�
		float elapsedTime;
		float preDelay;
		float postDelay;
		float fadeOutTime;
		float fadeInTime;

		ISprite* sprite;
		std::vector<std::wstring> textureList;

		// IAnimator��(��) ���� ��ӵ�
		virtual void Execute() override;

		void ExecutePreDelay();
		void ExecuteFadeIn();
		void ExecuteAnimation();
		void ExecutePostDelay();
		void ExecuteFadeOut();
		void ExecuteTimeOut();

		void FindSprite();

	private:
		// Component��(��) ���� ��ӵ�
		virtual void PreSerialize(json& jsonData) const override;
		virtual void PreDeserialize(const json& jsonData) override;
		virtual void PostSerialize(json& jsonData) const override;
		virtual void PostDeserialize(const json& jsonData) override;

	};
}