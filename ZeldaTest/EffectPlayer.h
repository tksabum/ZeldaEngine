#pragma once

#include "ZeldaEngine.h"

using namespace ZeldaEngine;

class ZeldaEngine::MeshRenderer;


class EffectPlayer : public ZeldaEngine::Component
{
public:
	virtual void Update() override;

	virtual void OnEnable() override;


	virtual void PreSerialize(json& jsonData) const override;
	virtual void PreDeserialize(const json& jsonData) override;
	virtual void PostSerialize(json& jsonData) const override;
	virtual void PostDeserialize(const json& jsonData) override;
	
public:
	void SetMeshRenderer(ZeldaEngine::MeshRenderer* renderer);
	void SetPlaySpeed(float speed);
	void SetMaxValue(float maxvalue);
	void SetDirection(ZeldaEngine::ZeldaMath::Vector2D direction);
	void SetRemainTime(float remaintime);
	void Reset();
	bool IsFInish();
	void InverseDirection();

private:
	ZeldaEngine::MeshRenderer* renderer;
	float speed;
	float maxValue;
	float currentValue;
	float currentTime;
	float remainTime;
	ZeldaEngine::ZeldaMath::Vector2D direction;
	ZeldaEngine::ZeldaMath::Vector2D uvPosition;
	bool isFinish = false;

	static float accTime;
};

