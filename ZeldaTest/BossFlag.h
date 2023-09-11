#pragma once

#include "ZeldaEngine.h"

using namespace ZeldaEngine;

class ZeldaEngine::MeshRenderer;

class BossFlag : public ZeldaEngine::Component
{
public:
	virtual void Update() override;

	virtual void PreSerialize(json& jsonData) const override;
	virtual void PreDeserialize(const json& jsonData) override;
	virtual void PostSerialize(json& jsonData) const override;
	virtual void PostDeserialize(const json& jsonData) override;

public:
	void SetMeshRenderer(ZeldaEngine::MeshRenderer* renderer);
	void SetPlaySpeed(float speed);
	void Dissolve();
	void Revert();

private:
	ZeldaEngine::MeshRenderer* renderer;
	float amount = 0;
	float speed = 1.f;

	bool isDissolve = false;
};

