#pragma once

#include "ZeldaEngine.h"

// 편하게 쓰려면 using namespace 2개는 고정으로 해주는게 좋을것임
using namespace ZeldaEngine;
using namespace ZeldaEngine::ZeldaMath;

class SoundHandle : public Component
{
public:
	virtual void Awake() override;

	virtual void OnMouseLeftDown() override;

	bool IsMouseLeftDown();

private:
	bool nowLeftDown = false;

	ZeldaUI* ui = nullptr;
	Image* image = nullptr;

	// Component을(를) 통해 상속됨
	virtual void PreSerialize(json& jsonData) const override;
	virtual void PreDeserialize(const json& jsonData) override;
	virtual void PostSerialize(json& jsonData) const override;
	virtual void PostDeserialize(const json& jsonData) override;
};