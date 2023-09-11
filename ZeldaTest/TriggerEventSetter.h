#pragma once

#include "ZeldaEngine.h"

using namespace ZeldaEngine;
using namespace ZeldaEngine::ZeldaMath;

#include <string>

class TriggerEventSetter : public Component
{
public:
	TriggerEventSetter();

	void OnCollisionEnter();
	void OnTriggerEnter();

	void SetTriggerEnterEventLoadScene(const std::wstring& sceneName);
	void SetCollisionEnterEventLoadScene(const std::wstring& sceneName);
private:
	std::wstring triggerEnterScene;
	std::wstring collisionEnterScene;

	// Component을(를) 통해 상속됨
	virtual void PreSerialize(json& jsonData) const override;
	virtual void PreDeserialize(const json& jsonData) override;
	virtual void PostSerialize(json& jsonData) const override;
	virtual void PostDeserialize(const json& jsonData) override;
};

