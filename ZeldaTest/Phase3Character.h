#pragma once

#include "ZeldaEngine.h"

using namespace ZeldaEngine;

class ZeldaEngine::SkinnedMeshRenderer;

class Phase3Character : public ZeldaEngine::Component
{
public:
	virtual void Awake() override;

	virtual void PreSerialize(json& jsonData) const override;
	virtual void PreDeserialize(const json& jsonData) override;
	virtual void PostSerialize(json& jsonData) const override;
	virtual void PostDeserialize(const json& jsonData) override;

	void SetSkinnedMeshRenderer(ZeldaEngine::SkinnedMeshRenderer* renderer);

private:
	ZeldaEngine::SkinnedMeshRenderer* renderer;


};

