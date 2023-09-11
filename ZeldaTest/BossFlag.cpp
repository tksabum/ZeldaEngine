#include "BossFlag.h"

void BossFlag::Update()
{
	if (isDissolve == true)
	{
		amount += 0.5f * TimeController::GetInstance().GetDeltaTime();
		if (amount >= 1.f)
		{
			renderer->GetMaterialInfo(0).intParams[1] = 1;
			isDissolve = false;
		}
		else
		{
			renderer->GetMaterialInfo(0).floatParams[1] = 1;
			renderer->GetMaterialInfo(0).floatParams[0] = amount;
			renderer->GetMaterialInfo(0).color = ZeldaColor::Orange;
		}
	}
}

void BossFlag::PreSerialize(json& jsonData) const
{
	PRESERIALIZE_BASE(BossFlag);
}

void BossFlag::PreDeserialize(const json& jsonData)
{
	PREDESERIALIZE_BASE(BossFlag);
}

void BossFlag::PostSerialize(json& jsonData) const
{
	POSTSERIALIZE_BASE();

	POSTSERIALIZE_PTR(renderer);
}

void BossFlag::PostDeserialize(const json& jsonData)
{
	POSTDESERIALIZE_BASE();

	POSTDESERIALIZE_PTR(renderer);
}

void BossFlag::SetMeshRenderer(ZeldaEngine::MeshRenderer* renderer)
{
	this->renderer = renderer;
}

void BossFlag::SetPlaySpeed(float speed)
{
	this->speed = speed;
}

void BossFlag::Dissolve()
{
	isDissolve = true;
}

void BossFlag::Revert()
{
	renderer->GetMaterialInfo(0).floatParams[1] = 0;
}
