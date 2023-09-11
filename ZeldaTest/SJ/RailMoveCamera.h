#pragma once

#include "ZeldaEngine.h"

// 편하게 쓰려면 using namespace 2개는 고정으로 해주는게 좋을것임
using namespace ZeldaEngine;
using namespace ZeldaEngine::ZeldaMath;

class CameraRailPoint;
class RailTriggerPoint;
class Dotween;

class RailMoveCamera : public Component
{
public:
	virtual void Awake() override;
	virtual void Start() override;
	virtual void Update() override;

	// RailPoint 등록
	void AddRailPoint(CameraRailPoint& railPoint);

	// Index 넣기에 실패하면 false
	bool SetRailPointIndex(CameraRailPoint& railPoint, int index);

	// 컨테이너에 없으면 -1
	int GetRailPointIndex(CameraRailPoint& railPoint) const;

	// RailTriggerPoint 등록
	void AddTriggerPoint(RailTriggerPoint& triggerPoint);

	// Index 넣기에 실패하면 false
	bool SetTriggerPointIndex(RailTriggerPoint& triggerPoint, int index);

	// 컨테이너에 없으면 -1
	int GetTriggerPointIndex(RailTriggerPoint& triggerPoint) const;

	// 구간별 Dotween Time, 실패 시 false
	bool SetRangePlayTime(int section, float playTime);

	// 구간별 Dotween Time, 실패 시 false
	bool SetRangeDelayTime(int section, float delayTime);

private:
	// 성공 시 true, 실패 시 false
	bool PlaySectionDotweenWithDelay();

	void PlaySectionAnimation();
	void PlaySectionTranslateWithDelay();
	void PlaySectionRotateWithDelay();

private:
	Transform* cameraTransform;
	Dotween* cameraDotween;
	std::vector<Transform*> railPoints;
	std::vector<Transform*> railTriggerPoints;

	// 구간별 Dotween Play Time(항상 railPoints 보다 size 가 1 적어야 함)
	// Serialize 를 vector 로 하기 위해 vector로 사용
	std::vector<float> sectionPlayTime;

	// 구간별 Dotween Delay Time(항상 railPoints 보다 size 가 1 적어야 함)
	std::vector<float> sectionDelayTime;

	int currentSection = 0;
	bool isDotweenPlay = false;

	float transTimer = 0;
	float rotTimer = 0;

	// Component을(를) 통해 상속됨
	virtual void PreSerialize(json& jsonData) const override;
	virtual void PreDeserialize(const json& jsonData) override;
	virtual void PostSerialize(json& jsonData) const override;
	virtual void PostDeserialize(const json& jsonData) override;
};