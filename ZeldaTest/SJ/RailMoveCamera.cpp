#include "RailMoveCamera.h"

#include "../Dotween.h"
#include "CameraRailPoint.h"
#include "RailTriggerPoint.h"

#include <algorithm>

void RailMoveCamera::Awake()
{
	if (!cameraTransform)
	{
		cameraTransform = GetGameObject()->GetTransform();
	}

	if (!cameraDotween)
	{
		cameraDotween = GetGameObject()->GetComponent<Dotween>();
	}
}

void RailMoveCamera::Start()
{
	if (railPoints.size() != 0)
	{
		cameraTransform->SetLocalPosition(railPoints[0]->GetLocalPosition());
		cameraTransform->SetLocalRotation(railPoints[0]->GetLocalRotation());
		cameraTransform->SetLocalScale(railPoints[0]->GetLocalScale());

		PlaySectionAnimation();
	}
}

void RailMoveCamera::Update()
{
	if (currentSection <= railPoints.size() - 2)
	{
		PlaySectionAnimation();
	}		
}

void RailMoveCamera::AddRailPoint(CameraRailPoint& railPoint)
{
	railPoints.emplace_back(railPoint.GetGameObject()->GetTransform());
	railPoint.SetIndex(railPoints.size() - 1);
	sectionPlayTime.push_back(0);
	sectionDelayTime.push_back(0);
}

bool RailMoveCamera::SetRailPointIndex(CameraRailPoint& railPoint, int index)
{
	if (railPoints.size() >= index)
	{
		return false;
	}

	if (GetRailPointIndex(railPoint) == index)
	{
		return true;
	}
	else
	{
		auto itr = railPoints.begin();

		for (int i = 0; i < GetRailPointIndex(railPoint); i++)
		{
			itr++;
		}

		railPoints.erase(itr);

		itr = railPoints.begin();
		for (int i = 0; i < index; i++)
		{
			itr++;
		}

		railPoints.insert(itr, railPoint.GetGameObject()->GetTransform());

		for (auto itr_range = itr; itr_range != railPoints.end(); itr_range++)
		{
			auto each = (*itr_range)->GetGameObject()->GetComponent<CameraRailPoint>();
			each->SetIndex(each->GetIndex() + 1);
		}

		railPoint.SetIndex(index);
		return true;
	}
}

int RailMoveCamera::GetRailPointIndex(CameraRailPoint& railPoint) const
{
	auto itr = std::find(railPoints.begin(), railPoints.end(), railPoint.GetGameObject()->GetTransform());
	if (itr != railPoints.end())
	{
		return itr - railPoints.begin();
	}
	else
	{
		return -1;
	}
}

void RailMoveCamera::AddTriggerPoint(RailTriggerPoint& triggerPoint)
{
	railTriggerPoints.emplace_back(triggerPoint.GetGameObject()->GetTransform());
	triggerPoint.SetIndex(railTriggerPoints.size() - 1);
}

bool RailMoveCamera::SetTriggerPointIndex(RailTriggerPoint& triggerPoint, int index)
{
	if (railTriggerPoints.size() >= index)
	{
		return false;
	}

	if (GetTriggerPointIndex(triggerPoint) == index)
	{
		return true;
	}
	else
	{
		auto itr = railTriggerPoints.begin();

		for (int i = 0; i < GetTriggerPointIndex(triggerPoint); i++)
		{
			itr++;
		}

		railTriggerPoints.erase(itr);

		itr = railTriggerPoints.begin();
		for (int i = 0; i < index; i++)
		{
			itr++;
		}

		railTriggerPoints.insert(itr, triggerPoint.GetGameObject()->GetTransform());

		for (auto itr_range = itr; itr_range != railPoints.end(); itr_range++)
		{
			auto each = (*itr_range)->GetGameObject()->GetComponent<RailTriggerPoint>();
			each->SetIndex(each->GetIndex() + 1);
		}

		triggerPoint.SetIndex(index);

		return true;
	}
}

int RailMoveCamera::GetTriggerPointIndex(RailTriggerPoint& triggerPoint) const
{
	auto itr = std::find(railTriggerPoints.begin(), railTriggerPoints.end(), triggerPoint.GetGameObject()->GetTransform());
	if (itr != railTriggerPoints.end())
	{
		return itr - railTriggerPoints.begin();
	}
	else
	{
		return -1;
	}
}

bool RailMoveCamera::SetRangePlayTime(int section, float playTime)
{
	if (section < 0 || section > railPoints.size() - 2)
	{
		return false;
	}

	if (sectionPlayTime.size() - 1 < section)
	{
		while (sectionPlayTime.size() - 1 != section)
		{
			// 기본 0초로 넣음
			sectionPlayTime.emplace_back(0.0f);
		}
		sectionPlayTime[section] = playTime;
	}
	else
	{
		sectionPlayTime[section] = playTime;
	}

	return true;
}


bool RailMoveCamera::SetRangeDelayTime(int section, float delayTime)
{
	if (section < 0 || section > railPoints.size() - 2)
	{
		return false;
	}

	if (sectionDelayTime.size() - 1 < section)
	{
		while (sectionDelayTime.size() - 1 != section)
		{
			// 기본 0초로 넣음
			sectionDelayTime.emplace_back(0.0f);
		}
		sectionDelayTime[section] = delayTime;
	}
	else
	{
		sectionDelayTime[section] = delayTime;
	}

	return true;
}

bool RailMoveCamera::PlaySectionDotweenWithDelay()
{
	if (currentSection > railPoints.size() - 2)
		return false;

	isDotweenPlay = true;

	cameraDotween->DOMove(railPoints[currentSection + 1]->GetLocalPosition(), sectionPlayTime[currentSection]).SetDelay(sectionDelayTime[currentSection]).SetEase(EaseBasic).OnComplete([this]() { currentSection++; isDotweenPlay = false; });

	return true;
}

void RailMoveCamera::PlaySectionAnimation()
{
	PlaySectionRotateWithDelay();
	PlaySectionTranslateWithDelay();
}

void RailMoveCamera::PlaySectionTranslateWithDelay()
{
	transTimer += TimeController::GetInstance().GetDeltaTime();

	if (currentSection > railPoints.size() - 2 || transTimer <= sectionDelayTime[currentSection] || sectionPlayTime[currentSection] == 0)
		return;

	if (transTimer >= sectionPlayTime[currentSection] + sectionDelayTime[currentSection])
	{
		transTimer = 0;

		/// 여기서만 처리함
		currentSection++;
		return;
	}

	GetGameObject()->GetTransform()->SetLocalPosition(ZeldaMath::Lerp(railPoints[currentSection]->GetLocalPosition(), railPoints[currentSection + 1]->GetLocalPosition(), (transTimer - sectionDelayTime[currentSection]) / sectionPlayTime[currentSection]));
}

void RailMoveCamera::PlaySectionRotateWithDelay()
{
	rotTimer += TimeController::GetInstance().GetDeltaTime();

	if (currentSection > railPoints.size() - 2 || rotTimer <= sectionDelayTime[currentSection] || sectionPlayTime[currentSection] == 0)
		return;

	if (rotTimer >= sectionPlayTime[currentSection] + sectionDelayTime[currentSection])
	{
		rotTimer = 0;
		return;
	}

	GetGameObject()->GetTransform()->SetLocalRotation(Slerp(railPoints[currentSection]->GetLocalRotation(), railPoints[currentSection + 1]->GetLocalRotation(), (rotTimer - sectionDelayTime[currentSection]) / sectionPlayTime[currentSection]));
}

void RailMoveCamera::PreSerialize(json& jsonData) const
{
	PRESERIALIZE_BASE(RailMoveCamera);

	PRESERIALIZE_VECTOR_PTR(railPoints);
	PRESERIALIZE_VECTOR_PTR(railTriggerPoints);
	PRESERIALIZE_VECTOR_VALUE(sectionPlayTime);
	PRESERIALIZE_VECTOR_VALUE(sectionDelayTime);
}

void RailMoveCamera::PreDeserialize(const json& jsonData)
{
	PREDESERIALIZE_BASE(RailMoveCamera);

	PREDESERIALIZE_VECTOR_PTR(railPoints);
	PREDESERIALIZE_VECTOR_PTR(railTriggerPoints);
	PREDESERIALIZE_VECTOR_VALUE(sectionPlayTime);
	PREDESERIALIZE_VECTOR_VALUE(sectionDelayTime);
}

void RailMoveCamera::PostSerialize(json& jsonData) const
{
	POSTSERIALIZE_BASE();

	POSTSERIALIZE_VECTOR_PTR(railPoints);
	POSTSERIALIZE_VECTOR_PTR(railTriggerPoints);

	POSTSERIALIZE_PTR(cameraTransform);
	POSTSERIALIZE_PTR(cameraDotween);
}

void RailMoveCamera::PostDeserialize(const json& jsonData)
{
	POSTDESERIALIZE_BASE();

	POSTDESERIALIZE_VECTOR_PTR(railPoints);
	POSTDESERIALIZE_VECTOR_PTR(railTriggerPoints);

	POSTDESERIALIZE_PTR(cameraTransform);
	POSTDESERIALIZE_PTR(cameraDotween);
}
