#pragma once

#include "ZeldaEngineAPI.h"

#include "Component.h"

#include "GraphicsDefine.h"


namespace ZeldaEngine
{
	class ZELDAENGINE_API Animator : public Component
	{
	public:
		Animator();
		virtual ~Animator();

		virtual void Awake() override;
		virtual void Update() override;

	public:
		void PlusAnimationClip(const FF::FFAnimationClip& animationclip);
		void Play(const std::wstring& animationname);

		__int32 GetFrame() { return frame; }
		__int32 GetNextFrame() { return nextFrame; }
		float GetFrameRatio() { return frameRatio; }
		unsigned __int64 GetID() { return id; }
		std::wstring& GetCurrentAnimationName() { return currentAnimation; }

	private:
		std::wstring meshName;
		std::wstring currentAnimation;
		std::map<std::wstring, FF::FFAnimationClip> animationMap;

		float updateTime;
		__int32 frame;
		__int32 nextFrame;
		float frameRatio;

		unsigned __int64 id;

		// Component을(를) 통해 상속됨
		virtual void PreSerialize(json& jsonData) const override;
		virtual void PreDeserialize(const json& jsonData) override;
		virtual void PostSerialize(json& jsonData) const override;
		virtual void PostDeserialize(const json& jsonData) override;
	};
}

