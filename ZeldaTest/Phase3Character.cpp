#include "Phase3Character.h"

void Phase3Character::Awake()
{
	if (renderer)
	{
		renderer->GetMaterialInfo(0).vec4Params[0] = 1.f;
	}
}

void Phase3Character::PreSerialize(json& jsonData) const
{
	PRESERIALIZE_BASE(Phase3Character);
}

void Phase3Character::PreDeserialize(const json& jsonData)
{
	PREDESERIALIZE_BASE(Phase3Character);
}

void Phase3Character::PostSerialize(json& jsonData) const
{
	POSTSERIALIZE_BASE();
	POSTSERIALIZE_PTR(renderer);
}

void Phase3Character::PostDeserialize(const json& jsonData)
{
	POSTDESERIALIZE_BASE();
	POSTDESERIALIZE_PTR(renderer);
}

void Phase3Character::SetSkinnedMeshRenderer(ZeldaEngine::SkinnedMeshRenderer* renderer)
{
	this->renderer = renderer;
}
