#pragma once

#include "ZeldaEngine.h"

using namespace ZeldaEngine;
using namespace ZeldaEngine::ZeldaMath;

class StaffRoll : public ZeldaEngine::Component
{
public:
	virtual void Update() override;
	virtual void Awake() override;

	virtual void PreSerialize(json& jsonData) const override;
	virtual void PreDeserialize(const json& jsonData) override;
	virtual void PostSerialize(json& jsonData) const override;
	virtual void PostDeserialize(const json& jsonData) override;

private:
	float time = 20.f;
	float distance = 1920;
	float firstY;
};

