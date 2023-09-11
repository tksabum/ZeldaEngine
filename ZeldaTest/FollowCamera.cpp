#include "FollowCamera.h"
#include "Dotween.h"
#include <algorithm>
#include <cmath>

FollowCamera::FollowCamera()
	: dist(207.0f), angle(40.0f), minX(-5.0f), maxX(5.0f), minZ(-5.0f), maxZ(5.0f), moveElapse(0.0f), durationTime(0.3f),
	isFixedCamera(false), isFinish(true)
{

}

void FollowCamera::Awake()
{

}

void FollowCamera::Start()
{
	objectDotween = GetGameObject()->AddComponent<Dotween>();
}

void FollowCamera::Update()
{
	Camera* mainCamera = SceneManager::GetInstance().GetMainCamera();
	GameObject* cameraObject = mainCamera->GetGameObject();

	if (isFixedCamera)
	{
		HoldingCamera(cameraObject);
	}
	else
	{
		MovingCamera(cameraObject);
	}
}

void FollowCamera::SetPlayerTransform(Transform* t)
{
	playerTransform = t;
}

void FollowCamera::MovingCamera(GameObject* cameraObject)
{
	float deltaTime = TimeController::GetInstance().GetDeltaTime();
	float PlayerX = std::clamp(playerTransform->GetLocalPosition().x, minX, maxX);
	float PlayerY = playerTransform->GetLocalPosition().y;
	float PlayerZ = std::clamp(playerTransform->GetLocalPosition().z, minZ, maxZ);

	float sinBeforeElapsed = easeInOutSine((moveElapse - deltaTime) / durationTime);
	float sinElapsed = easeInOutSine(moveElapse / durationTime);

	Vector3D clampCamera = Vector3D(PlayerX, PlayerY + dist * std::sin(DegreeToRadian(angle)), PlayerZ + dist * std::cos(DegreeToRadian(angle)));
	Vector3D distance = clampCamera - cameraObject->GetTransform()->GetLocalPosition();

	Vector3D DistancePerFrame = distance * sinElapsed - distance * sinBeforeElapsed;
	
	if (isFinish == false)
	{
		if (moveElapse <= durationTime)
		{
			moveElapse += deltaTime;
			cameraObject->GetTransform()->SetLocalPosition(cameraObject->GetTransform()->GetLocalPosition() + DistancePerFrame);
		}
		else
		{
			moveElapse = 0.0f;
			isFinish = true;
		}
	}
	else
	{
		cameraObject->GetTransform()->SetLocalPosition(clampCamera);
		cameraObject->GetTransform()->SetLocalRotation(Vector3D(angle, 180, 0));
	}
}

void FollowCamera::HoldingCamera(GameObject* cameraObject)
{
	float deltaTime = TimeController::GetInstance().GetDeltaTime();
	float sinBeforeElapsed = easeInOutSine((moveElapse - deltaTime) / durationTime);
	float sinElapsed = easeInOutSine(moveElapse / durationTime);

	Vector3D distance = Vector3D(0, dist * std::sin(DegreeToRadian(angle)), dist * std::cos(DegreeToRadian(angle))) - cameraObject->GetTransform()->GetLocalPosition();

	Vector3D DistancePerFrame = distance * sinElapsed - distance * sinBeforeElapsed;
	if (isFinish == false)
	{
		if (moveElapse <= durationTime)
		{
			moveElapse += deltaTime;
			cameraObject->GetTransform()->SetLocalPosition(cameraObject->GetTransform()->GetLocalPosition() + DistancePerFrame);
		}
		else
		{
			moveElapse = 0.0f;
			isFinish = true;
		}
	}
	else
	{
		cameraObject->GetTransform()->SetLocalPosition(Vector3D(0, dist * std::sin(DegreeToRadian(angle)), dist * std::cos(DegreeToRadian(angle))));
		cameraObject->GetTransform()->SetLocalRotation(Vector3D(angle, 180, 0));
	}
}

void FollowCamera::SetCameraFix(bool isFixed)
{
	isFixedCamera = isFixed;
}

float FollowCamera::easeInOutSine(float t)
{
	return -1 * (cos(PI * t) - 1) / 2;
}

void FollowCamera::FixCamera()
{
	isFixedCamera = true;
	isFinish = false;
}

void FollowCamera::UnFixCamera()
{
	isFixedCamera = false;
	isFinish = false;
}

void FollowCamera::PreSerialize(json& jsonData) const
{
	PRESERIALIZE_BASE(FollowCamera);
	PRESERIALIZE_VALUE(isFixedCamera);
}

void FollowCamera::PreDeserialize(const json& jsonData)
{
	PREDESERIALIZE_BASE(FollowCamera);
	PREDESERIALIZE_VALUE(isFixedCamera);
}

void FollowCamera::PostSerialize(json& jsonData) const
{
	POSTSERIALIZE_BASE();
	POSTSERIALIZE_PTR(playerTransform);
}

void FollowCamera::PostDeserialize(const json& jsonData)
{
	POSTDESERIALIZE_BASE();
	POSTDESERIALIZE_PTR(playerTransform);
}
