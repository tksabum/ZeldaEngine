#pragma once

#include "ZeldaEngine.h"

using namespace ZeldaEngine;
using namespace ZeldaEngine::ZeldaMath;

/// <summary>
/// rotation, scale�� �������� ���� ������Ʈ�� ���̴°� ������ �������...
/// </summary>
class Tracer : public Component
{
public:
	Tracer();

	void Awake();
	void LateUpdate();

	void SetTarget(Transform* target);

	void TracePositionX(bool value);
	void TracePositionY(bool value);
	void TracePositionZ(bool value);

	void SetClampPositionX(bool clampX = false, float minValue = 0.0f, float maxValue = 0.0f);
	void SetClampPositionY(bool clampY = false, float minValue = 0.0f, float maxValue = 0.0f);
	void SetClampPositionZ(bool clampZ = false, float minValue = 0.0f, float maxValue = 0.0f);

	// target���� offset��ŭ �̵��� ��ġ�� ����
	void SetOffSet(Vector3D value);

	// ������ �����ϴ� �Ÿ�����
	void SetSpace(Vector3D value);

	// 0���� �۰ų� ���� ��� �ٷ� �̵�
	void SetTraceSpeed(float speed);

private:
	Transform* ownTransform;

	Transform* targetTransform;

	Vector3D offset;
	Vector3D space;

	bool tracePosX;
	bool tracePosY;
	bool tracePosZ;

	bool isClampX;
	bool isClampY;
	bool isClampZ;

	float clampMinX;
	float clampMaxX;
	float clampMinY;
	float clampMaxY;
	float clampMinZ;
	float clampMaxZ;

	float traceSpeed;

private:
	// Component��(��) ���� ��ӵ�
	virtual void PreSerialize(json& jsonData) const override;
	virtual void PreDeserialize(const json& jsonData) override;
	virtual void PostSerialize(json& jsonData) const override;
	virtual void PostDeserialize(const json& jsonData) override;

};

