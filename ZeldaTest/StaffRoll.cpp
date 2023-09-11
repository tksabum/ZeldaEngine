#include "StaffRoll.h"

void StaffRoll::Update()
{
	if (GetGameObject()->GetTransform()->GetLocalPosition().y >= firstY + 1920)
	{
		return;
	}

	Vector3D nextPos = GetGameObject()->GetTransform()->GetLocalPosition();

	nextPos.y += (1920 / time) * TimeController::GetInstance().GetDeltaTime();

	GetGameObject()->GetTransform()->SetLocalPosition(nextPos);

}

void StaffRoll::Awake()
{
	firstY = GetGameObject()->GetTransform()->GetLocalPosition().y;
}

void StaffRoll::PreSerialize(json& jsonData) const
{
	PRESERIALIZE_BASE(StaffRoll);
}

void StaffRoll::PreDeserialize(const json& jsonData)
{
	PREDESERIALIZE_BASE(StaffRoll);
}

void StaffRoll::PostSerialize(json& jsonData) const
{
	POSTSERIALIZE_BASE();
}

void StaffRoll::PostDeserialize(const json& jsonData)
{
	POSTDESERIALIZE_BASE();
}
