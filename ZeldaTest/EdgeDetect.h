#pragma once
#include "ZeldaEngine.h"

using namespace ZeldaEngine;

class ZeldaEngine::SkinnedMeshRenderer;

class EdgeDetect : public ZeldaEngine::Component
{
public:
	virtual void PreSerialize(json& jsonData) const override;
	virtual void PreDeserialize(const json& jsonData) override;
	virtual void PostSerialize(json& jsonData) const override;
	virtual void PostDeserialize(const json& jsonData) override;

	virtual void OnCollisionStay2D() override;
	virtual void OnCollisionExit2D() override;

	void Awake();

	void SetMeshRenderer(ZeldaEngine::SkinnedMeshRenderer* renderer);
	void OnEdge();
	void OffEdge();

	void SetUseCollision(bool value);

private:

	bool useCollision;

	ZeldaEngine::SkinnedMeshRenderer* renderer;
};

