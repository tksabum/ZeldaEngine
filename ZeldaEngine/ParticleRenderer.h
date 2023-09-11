#pragma once

#include "ZeldaEngineAPI.h"

#include "Renderer.h"

namespace ZeldaEngine
{
	class ZELDAENGINE_API ParticleRenderer : public Renderer
	{
	public:
		virtual void Update() override;
		virtual void Render(IGraphics* graphics) override;

	private:
		virtual void PreSerialize(json& jsonData) const override;
		virtual void PreDeserialize(const json& jsonData) override;
		virtual void PostSerialize(json& jsonData) const override;
		virtual void PostDeserialize(const json& jsonData) override;

	private:
		unsigned __int32	_maxParticle = 1000;
		float				_createInterval = 0.005f;
		float				_accTime = 0.f;

		float				_minLifeTime = 1.f;
		float				_maxLifeTime = 1.2f;
		float				_minSpeed = 1.3f;
		float				_maxSpeed = 1.5f;
		float				_startScale = 1.5f;
		float				_endScale = 1.f;
	};
}