#include "FloatingObject.h"
#include "Dotween.h"
#include <algorithm>
#include <random>

FloatingObject::FloatingObject()
	: durationTime(1.5f), downObjectTime(1.0f), isFinish(true), isDown(false), isDone(false), minY(-10.0f), maxY(1.0f),
	floatingHeight(Vector3D(0, 2, 0)), floatElapsed(0.0f), downElapsed(0.0f)
{

}

void FloatingObject::Awake()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dis(1, 10);
	randNum = dis(gen) / 10;
	objectX = objectTransform->GetLocalPosition().x;
	objectY = std::clamp(objectTransform->GetLocalPosition().y + randNum, minY, maxY);
	objectZ = objectTransform->GetLocalPosition().z;
	downHeight = Vector3D(0, -10, 0);
	objectTransform->SetLocalPosition(Vector3D(objectX, objectY, objectZ));
	remainObjectPosition = objectTransform->GetLocalPosition();
	objectHeight = Vector3D(objectX, objectY, objectZ);
	remainDownObjectPosition = remainObjectPosition - downHeight;
}

void FloatingObject::Start()
{
	ObjectDotween = GetGameObject()->AddComponent<Dotween>();
}

void FloatingObject::Update()
{
	if (isDown)
	{
		DownDuration();
	}
	else
	{
		FloatingDuration();
	}
}

void FloatingObject::SetObjectTransform(Transform* transform)
{
	objectTransform = transform;
}

void FloatingObject::DownObject()
{
	isDown = true;
}

void FloatingObject::RevertObject()
{
	floatElapsed = 0.0f;
	isDown = false;
	isFinish = true;
	isDone = true;
	objectTransform->SetLocalPosition(remainDownObjectPosition);
	objectTransform->GetGameObject()->SetActive(true);
}

void FloatingObject::FloatingDuration()
{
	float deltaTime = TimeController::GetInstance().GetDeltaTime();
	float sinBeforeElapsed = easeInOutSine((floatElapsed - deltaTime) / durationTime);
	float sinElapsed = easeInOutSine(floatElapsed / durationTime);

	Vector3D moveDistance = floatingHeight * sinElapsed - floatingHeight * sinBeforeElapsed;
	Vector3D revertDistance = remainObjectPosition - GetGameObject()->GetTransform()->GetLocalPosition();
	Vector3D revertPerFrame = revertDistance * sinElapsed - revertDistance * sinBeforeElapsed;
	if (isDone == false)
	{
		if (isFinish == true)
		{
			if (floatElapsed <= durationTime)
			{
				floatElapsed += deltaTime;
				objectTransform->SetLocalPosition(objectTransform->GetLocalPosition() + moveDistance);
			}
			else
			{
				isFinish = false;
				floatElapsed = 0.0f;
			}
		}
		else
		{
			if (floatElapsed <= durationTime)
			{
				floatElapsed += deltaTime;
				objectTransform->SetLocalPosition(objectTransform->GetLocalPosition() - moveDistance);
			}
			else
			{
				isFinish = true;
				floatElapsed = 0.0f;
			}
		}
	}
	else
	{
		if (floatElapsed <= durationTime)
		{
			floatElapsed += deltaTime;
			objectTransform->SetLocalPosition(objectTransform->GetLocalPosition() + revertPerFrame);
		}
		else
		{
			isDone = false;
			floatElapsed = 0.0f;
		}
	}
}

void FloatingObject::DownDuration()
{
	float deltaTime = TimeController::GetInstance().GetDeltaTime();
	float sinBeforeElapsed = easeInOutSine((downElapsed - deltaTime) / durationTime);
	float sinElapsed = easeInOutSine(downElapsed / durationTime);

	Vector3D downDistance = downHeight * sinElapsed - downHeight * sinBeforeElapsed;
	if (downElapsed <= durationTime)
	{
		downElapsed += deltaTime;
		objectTransform->SetLocalPosition(objectTransform->GetLocalPosition() + downDistance);
	}
	else
	{
		downElapsed = 0.0f;
		GetGameObject()->SetActive(false);
	}
}

float FloatingObject::easeInOutSine(float t)
{
	return -1 * (cos(PI * t) - 1) / 2;
}

void FloatingObject::PreSerialize(json& jsonData) const
{
	PRESERIALIZE_BASE(FloatingObject);
}

void FloatingObject::PreDeserialize(const json& jsonData)
{
	PREDESERIALIZE_BASE(FloatingObject);
}

void FloatingObject::PostSerialize(json& jsonData) const
{
	POSTSERIALIZE_BASE();
	POSTSERIALIZE_PTR(objectTransform);
}

void FloatingObject::PostDeserialize(const json& jsonData)
{
	POSTDESERIALIZE_BASE();
	POSTDESERIALIZE_PTR(objectTransform);
}
