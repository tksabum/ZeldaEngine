#include "pch.h"
#include "ParticleRenderer.h"

#include "GameObject.h"
#include "Transform.h"
#include "TimeController.h"

void ZeldaEngine::ParticleRenderer::Update()
{
	_accTime += TimeController::GetInstance().GetDeltaTime();
	if (_accTime >= 1.f)
	{
		_accTime -= 1.f;
	}
}

void ZeldaEngine::ParticleRenderer::Render(IGraphics* graphics)
{
	ZeldaMath::Matrix4x4 worldTM = gameObject->GetTransform()->GetWorldMatrix();
	float deltaTime = TimeController::GetInstance().GetDeltaTime();
	graphics->DrawParticle(worldTM, deltaTime, _maxParticle, _createInterval, _accTime, _minLifeTime, _maxLifeTime, _minSpeed, _maxSpeed, _startScale, _endScale);
}

void ZeldaEngine::ParticleRenderer::PreSerialize(json& jsonData) const
{
	PRESERIALIZE_BASE(ParticleRenderer);
}

void ZeldaEngine::ParticleRenderer::PreDeserialize(const json& jsonData)
{
	PREDESERIALIZE_BASE(ParticleRenderer);
}

void ZeldaEngine::ParticleRenderer::PostSerialize(json& jsonData) const
{
	POSTSERIALIZE_BASE();
}

void ZeldaEngine::ParticleRenderer::PostDeserialize(const json& jsonData)
{
	POSTDESERIALIZE_BASE();
}
