#pragma once

#include "ZeldaEngine.h"

using namespace ZeldaEngine;
using namespace ZeldaEngine::ZeldaMath;

#include <list>
#include <random>

class RandomRotate : public Component
{
public:
	void Awake();
	void Update();

private:
	Transform* transform;
	SpriteAnimator* animator;

	std::list<Transform*> cubePool;
	std::list<Transform*> activePool;


	// Component을(를) 통해 상속됨
	virtual void PreSerialize(json& jsonData) const override;
	virtual void PreDeserialize(const json& jsonData) override;
	virtual void PostSerialize(json& jsonData) const override;
	virtual void PostDeserialize(const json& jsonData) override;
};