#include "EffectPlayer.h"

#include "TimeController.h"

float EffectPlayer::accTime = 0.f;

void EffectPlayer::Update()
{
	currentTime += ZeldaEngine::TimeController::GetInstance().GetDeltaTime();

	if ((direction.x + direction.y) > 0)
	{
		if (currentValue >= maxValue)
		{
			isFinish = true;

			if (currentTime >= remainTime)
			{
				isFinish = false;
				GetGameObject()->SetActive(false);
			}
			else
			{
				renderer->GetMaterialInfo(0).intParams[0] = 1;
			}
		}
		else
		{
			currentValue += speed * ZeldaEngine::TimeController::GetInstance().GetDeltaTime();

			uvPosition.x = direction.x * currentValue;
			uvPosition.y = direction.y * currentValue;
		}
	}
	else
	{
		if (currentValue < maxValue)
		{
			isFinish = true;

			if (currentTime >= remainTime)
			{
				isFinish = false;
				GetGameObject()->SetActive(false);
			}
			else
			{
				renderer->GetMaterialInfo(0).intParams[0] = 1;
			}
		}
		else
		{
			currentValue -= speed * ZeldaEngine::TimeController::GetInstance().GetDeltaTime();

			uvPosition.x = currentValue;
			uvPosition.y = direction.y * currentValue;
		}
	}

	renderer->GetMaterialInfo(0).vec2Params[0].x = uvPosition.x;
	renderer->GetMaterialInfo(0).vec2Params[0].y = uvPosition.y;
}

void EffectPlayer::OnEnable()
{
	Reset();
}

void EffectPlayer::PreSerialize(json& jsonData) const
{
	PRESERIALIZE_BASE(EffectPlayer);

	PRESERIALIZE_VALUE(speed);
	PRESERIALIZE_VALUE(maxValue);
	PRESERIALIZE_VALUE(remainTime);
	PRESERIALIZE_SERVALUE(direction);
}

void EffectPlayer::PreDeserialize(const json& jsonData)
{
	PREDESERIALIZE_BASE(EffectPlayer);

	PREDESERIALIZE_VALUE(speed);
	PREDESERIALIZE_VALUE(maxValue);
	PREDESERIALIZE_VALUE(remainTime);
	PREDESERIALIZE_SERVALUE(direction);
}

void EffectPlayer::PostSerialize(json& jsonData) const
{
	POSTSERIALIZE_BASE();

	POSTSERIALIZE_PTR(renderer);
	POSTSERIALIZE_SERVALUE(direction);
}

void EffectPlayer::PostDeserialize(const json& jsonData)
{
	POSTDESERIALIZE_BASE();

	POSTDESERIALIZE_PTR(renderer);
	POSTDESERIALIZE_SERVALUE(direction);
}

void EffectPlayer::SetMeshRenderer(ZeldaEngine::MeshRenderer* renderer)
{
	this->renderer = renderer;
}

void EffectPlayer::SetPlaySpeed(float speed)
{
	this->speed = speed;
}

void EffectPlayer::SetMaxValue(float maxvalue)
{
	this->maxValue = maxvalue;
}

void EffectPlayer::SetDirection(ZeldaEngine::ZeldaMath::Vector2D direction)
{
	this->direction.x = direction.x;
	this->direction.y = direction.y;
}

void EffectPlayer::SetRemainTime(float remaintime)
{
	this->remainTime = remaintime;
}

void EffectPlayer::Reset()
{
	isFinish = false;
	currentTime = 0.f;
	currentValue = 0.f;
	uvPosition.x = 0.f;
	uvPosition.y = 0.f;
	renderer->GetMaterialInfo(0).intParams[0] = 0;
}

bool EffectPlayer::IsFInish()
{
	return isFinish;
}

void EffectPlayer::InverseDirection()
{
	direction.x *= -1;
	direction.y *= -1;
}

