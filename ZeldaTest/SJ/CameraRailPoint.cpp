#include "CameraRailPoint.h"


void CameraRailPoint::SetIndex(int index)
{
	this->index = index;
}

int CameraRailPoint::GetIndex() const
{
	return index;
}

void CameraRailPoint::PreSerialize(json& jsonData) const
{
	PRESERIALIZE_BASE(CameraRailPoint);

	PRESERIALIZE_VALUE(index);
}

void CameraRailPoint::PreDeserialize(const json& jsonData)
{
	PREDESERIALIZE_BASE(CameraRailPoint);

	PREDESERIALIZE_VALUE(index);
}

void CameraRailPoint::PostSerialize(json& jsonData) const
{
	POSTSERIALIZE_BASE();
}

void CameraRailPoint::PostDeserialize(const json& jsonData)
{
	POSTDESERIALIZE_BASE();
}
