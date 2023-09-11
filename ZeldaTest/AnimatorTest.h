#pragma once

#include "ZeldaEngine.h"

class ZeldaEngine::Animator;

class AnimatorTest : public ZeldaEngine::Component
{
public:
	virtual void Update() override;

	virtual void PreSerialize(json& jsonData) const override;
	virtual void PreDeserialize(const json& jsonData) override;
	virtual void PostSerialize(json& jsonData) const override;
	virtual void PostDeserialize(const json& jsonData) override;

public:
	void SetAnimator(ZeldaEngine::Animator* p_animator) { animator = p_animator; }

private:
	ZeldaEngine::Animator* animator;
};

