#pragma once

#include "ZeldaEngine.h"

#include "../SJ/CoolTimeUI.h"

// 편하게 쓰려면 using namespace 2개는 고정으로 해주는게 좋을것임
using namespace ZeldaEngine;
using namespace ZeldaEngine::ZeldaMath;

class DashCoolTimeUI : public CoolTimeUI
{
public:
	virtual void Awake() override;
	virtual void Start() override;
	virtual void Update() override;

public:
	float padding = 4.0f;

	// Component을(를) 통해 상속됨
	virtual void PreSerialize(json& jsonData) const override;
	virtual void PreDeserialize(const json& jsonData) override;
	virtual void PostSerialize(json& jsonData) const override;
	virtual void PostDeserialize(const json& jsonData) override;
};