#pragma once

#include "ZeldaEngine.h"

using namespace ZeldaEngine;
using namespace ZeldaEngine::ZeldaMath;

/// <summary>
/// rotation, scale을 변경하지 않은 오브젝트에 붙이는걸 전제로 만들었다...
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

	// target에서 offset만큼 이동한 위치를 추적
	void SetOffSet(Vector3D value);

	// 추적을 시작하는 거리차이
	void SetSpace(Vector3D value);

	// 0보다 작거나 같은 경우 바로 이동
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
	// Component을(를) 통해 상속됨
	virtual void PreSerialize(json& jsonData) const override;
	virtual void PreDeserialize(const json& jsonData) override;
	virtual void PostSerialize(json& jsonData) const override;
	virtual void PostDeserialize(const json& jsonData) override;

};

