#pragma once

#include "ZeldaEngine.h"

// ���ϰ� ������ using namespace 2���� �������� ���ִ°� ��������
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

	// RailPoint ���
	void AddRailPoint(CameraRailPoint& railPoint);

	// Index �ֱ⿡ �����ϸ� false
	bool SetRailPointIndex(CameraRailPoint& railPoint, int index);

	// �����̳ʿ� ������ -1
	int GetRailPointIndex(CameraRailPoint& railPoint) const;

	// RailTriggerPoint ���
	void AddTriggerPoint(RailTriggerPoint& triggerPoint);

	// Index �ֱ⿡ �����ϸ� false
	bool SetTriggerPointIndex(RailTriggerPoint& triggerPoint, int index);

	// �����̳ʿ� ������ -1
	int GetTriggerPointIndex(RailTriggerPoint& triggerPoint) const;

	// ������ Dotween Time, ���� �� false
	bool SetRangePlayTime(int section, float playTime);

	// ������ Dotween Time, ���� �� false
	bool SetRangeDelayTime(int section, float delayTime);

private:
	// ���� �� true, ���� �� false
	bool PlaySectionDotweenWithDelay();

	void PlaySectionAnimation();
	void PlaySectionTranslateWithDelay();
	void PlaySectionRotateWithDelay();

private:
	Transform* cameraTransform;
	Dotween* cameraDotween;
	std::vector<Transform*> railPoints;
	std::vector<Transform*> railTriggerPoints;

	// ������ Dotween Play Time(�׻� railPoints ���� size �� 1 ����� ��)
	// Serialize �� vector �� �ϱ� ���� vector�� ���
	std::vector<float> sectionPlayTime;

	// ������ Dotween Delay Time(�׻� railPoints ���� size �� 1 ����� ��)
	std::vector<float> sectionDelayTime;

	int currentSection = 0;
	bool isDotweenPlay = false;

	float transTimer = 0;
	float rotTimer = 0;

	// Component��(��) ���� ��ӵ�
	virtual void PreSerialize(json& jsonData) const override;
	virtual void PreDeserialize(const json& jsonData) override;
	virtual void PostSerialize(json& jsonData) const override;
	virtual void PostDeserialize(const json& jsonData) override;
};