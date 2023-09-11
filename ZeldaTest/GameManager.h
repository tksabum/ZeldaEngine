#pragma once

#include "ZeldaEngine.h"

#include <list>

// 편하게 쓰려면 using namespace 2개는 고정으로 해주는게 좋을것임
using namespace ZeldaEngine;
using namespace ZeldaEngine::ZeldaMath;

class GameManager : public Component
{
public:
	void Awake();
	void Start();
	void Update();

	void SetMainAnimator(SpriteAnimator* spriteAnimator);
	void SetSubAnimator(SpriteAnimator* spriteAnimator);
	void SetThirdAnimator(SpriteAnimator* spriteAnimator);

	void ShowPhase2CutScene1();
	void ShowPhase2CutScene2();
	void ShowPhase3CutScene();

	void ShowPhase2EndEvent();
	void ShowPhase3StartEvent();

	void ShowEndingCutSceneEndEvent();

	void ShowGameOverScene(float gameoverDelay = 2.0f);

private:
	enum EventType
	{
		Animation,
		Scene,
		Pause,
		Resume
	};

	struct EventInfo
	{
		EventType type;
		std::wstring sceneName;
		SpriteAnimator* animator;
		float preDelay;
		float fadeinTime;
		float playTime;
		float PostDelay;
		float fadeoutTime;
		std::vector<std::wstring> sprites;
		int layer;
	};

	std::list<std::pair<float, EventInfo>> eventList;

	void PushAnimationEvent(float startTime, SpriteAnimator* animator, float preDelay, float fadeinTime, float playTime, float postDelay, float fadeoutTime, std::vector<std::wstring> sprites, int layer = DefaultLayer);
	void PushSceneEvent(float startTime, std::wstring sceneName);
	void PushPauseEvent(float time);
	void PushResumeEvent(float time);

	SpriteAnimator* mainAnimator;
	SpriteAnimator* subAnimator;
	SpriteAnimator* thirdAnimator;

	std::unordered_map<SpriteAnimator*, Image*> imageMap;

	bool isStoppedByMe;

	bool isPushedSceneEvent;

	float eventTime;
	float elapsedTime;
	std::wstring nextSceneName;

	const static int DefaultLayer;

private:
	void LoadMainScene();

private:
	// Component을(를) 통해 상속됨
	virtual void PreSerialize(json& jsonData) const override;
	virtual void PreDeserialize(const json& jsonData) override;
	virtual void PostSerialize(json& jsonData) const override;
	virtual void PostDeserialize(const json& jsonData) override;
};

