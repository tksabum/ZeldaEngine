#include "Tracer.h"

Tracer::Tracer() :
	ownTransform(nullptr),
	targetTransform(nullptr),
	offset(Vector3D::zero),
	space(Vector3D::zero),
	tracePosX(false),
	tracePosY(false),
	tracePosZ(false),
	traceSpeed(0.0f),
	isClampX(false),
	isClampY(false),
	isClampZ(false),
	clampMinX(0.0f),
	clampMaxX(0.0f),
	clampMinY(0.0f),
	clampMaxY(0.0f),
	clampMinZ(0.0f),
	clampMaxZ(0.0f)
{

}

void Tracer::Awake()
{
	ownTransform = GetGameObject()->GetTransform();
}

void Tracer::LateUpdate()
{
	if (targetTransform != nullptr)
	{
		float deltaTime = TimeController::GetInstance().GetDeltaTime();

		Vector3D targetPos = targetTransform->GetWorldPosition() + offset;
		Vector3D ownPos = ownTransform->GetWorldPosition();

		if (tracePosX)
		{
			if (Abs(targetPos.x - ownPos.x) >= space.x)
			{
				if (traceSpeed <= 0.0f || Abs(targetPos.x - ownPos.x) <= (traceSpeed * deltaTime))
				{
					ownPos.x = targetPos.x;
				}
				else
				{
					if (targetPos.x < ownPos.x)
					{
						ownPos.x = ownPos.x - (traceSpeed * deltaTime);
					}
					else
					{
						ownPos.x = ownPos.x + (traceSpeed * deltaTime);
					}
				}
			}
		}

		if (tracePosY)
		{
			if (Abs(targetPos.y - ownPos.y) >= space.y)
			{
				if (traceSpeed <= 0.0f || Abs(targetPos.y - ownPos.y) <= (traceSpeed * deltaTime))
				{
					ownPos.y = targetPos.y;
				}
				else
				{
					if (targetPos.y < ownPos.y)
					{
						ownPos.y = ownPos.y - (traceSpeed * deltaTime);
					}
					else
					{
						ownPos.y = ownPos.y + (traceSpeed * deltaTime);
					}
				}
			}
		}

		if (tracePosZ)
		{
			if (Abs(targetPos.z - ownPos.z) >= space.z)
			{
				if (traceSpeed <= 0.0f || Abs(targetPos.z - ownPos.z) <= (traceSpeed * deltaTime))
				{
					ownPos.z = targetPos.z;
				}
				else
				{
					if (targetPos.z < ownPos.z)
					{
						ownPos.z = ownPos.z - (traceSpeed * deltaTime);
					}
					else
					{
						ownPos.z = ownPos.z + (traceSpeed * deltaTime);
					}
				}
			}
		}

		if (isClampX) ownPos.x = max(clampMinX, min(clampMaxX, ownPos.x));
		if (isClampY) ownPos.y = max(clampMinY, min(clampMaxY, ownPos.y));
		if (isClampZ) ownPos.z = max(clampMinZ, min(clampMaxZ, ownPos.z));

		ownTransform->SetLocalPosition(ownPos);
	}
}

void Tracer::SetTarget(Transform* target)
{
	targetTransform = target;
}

void Tracer::TracePositionX(bool value)
{
	tracePosX = value;
}

void Tracer::TracePositionY(bool value)
{
	tracePosY = value;
}

void Tracer::TracePositionZ(bool value)
{
	tracePosZ = value;
}

void Tracer::SetClampPositionX(bool clampX, float minValue, float maxValue)
{
	isClampX = clampX;
	clampMinX = minValue;
	clampMaxX = maxValue;
}

void Tracer::SetClampPositionY(bool clampY, float minValue, float maxValue)
{
	isClampY = clampY;
	clampMinY = minValue;
	clampMaxY = maxValue;
}

void Tracer::SetClampPositionZ(bool clampZ, float minValue, float maxValue)
{
	isClampZ = clampZ;
	clampMinZ = minValue;
	clampMaxZ = maxValue;
}

void Tracer::SetOffSet(Vector3D value)
{
	offset = value;
}

void Tracer::SetSpace(Vector3D value)
{
	space = value;
}

void Tracer::SetTraceSpeed(float speed)
{
	traceSpeed = speed;
}

void Tracer::PreSerialize(json& jsonData) const
{
	PRESERIALIZE_BASE(Tracer);
	PRESERIALIZE_SERVALUE(offset);
	PRESERIALIZE_SERVALUE(space);
	PRESERIALIZE_VALUE(tracePosX);
	PRESERIALIZE_VALUE(tracePosY);
	PRESERIALIZE_VALUE(tracePosZ);
	PRESERIALIZE_VALUE(traceSpeed);

	PRESERIALIZE_VALUE(isClampX);
	PRESERIALIZE_VALUE(isClampY);
	PRESERIALIZE_VALUE(isClampZ);
	PRESERIALIZE_VALUE(clampMinX);
	PRESERIALIZE_VALUE(clampMaxX);
	PRESERIALIZE_VALUE(clampMinY);
	PRESERIALIZE_VALUE(clampMaxY);
	PRESERIALIZE_VALUE(clampMinZ);
	PRESERIALIZE_VALUE(clampMaxZ);
}

void Tracer::PreDeserialize(const json& jsonData)
{
	PREDESERIALIZE_BASE(Tracer);
	PREDESERIALIZE_SERVALUE(offset);
	PREDESERIALIZE_SERVALUE(space);
	PREDESERIALIZE_VALUE(tracePosX);
	PREDESERIALIZE_VALUE(tracePosY);
	PREDESERIALIZE_VALUE(tracePosZ);
	PREDESERIALIZE_VALUE(traceSpeed);

	PREDESERIALIZE_VALUE(isClampX);
	PREDESERIALIZE_VALUE(isClampY);
	PREDESERIALIZE_VALUE(isClampZ);
	PREDESERIALIZE_VALUE(clampMinX);
	PREDESERIALIZE_VALUE(clampMaxX);
	PREDESERIALIZE_VALUE(clampMinY);
	PREDESERIALIZE_VALUE(clampMaxY);
	PREDESERIALIZE_VALUE(clampMinZ);
	PREDESERIALIZE_VALUE(clampMaxZ);
}

void Tracer::PostSerialize(json& jsonData) const
{
	POSTSERIALIZE_BASE();
	POSTSERIALIZE_PTR(targetTransform);
	POSTSERIALIZE_SERVALUE(offset);
	POSTSERIALIZE_SERVALUE(space);
}

void Tracer::PostDeserialize(const json& jsonData)
{
	POSTDESERIALIZE_BASE();
	POSTDESERIALIZE_PTR(targetTransform);
	POSTDESERIALIZE_SERVALUE(offset);
	POSTDESERIALIZE_SERVALUE(space);
}