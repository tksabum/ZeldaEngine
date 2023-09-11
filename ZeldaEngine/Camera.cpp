#include "pch.h"
#include "Camera.h"

namespace ZeldaEngine
{
	unsigned int Camera::idCounter = 0;
	std::unordered_map<unsigned int, Camera*> Camera::cameraIDTable;

	Camera::Camera() : cameraNear(1.0f), cameraFar(1000.0f), fieldOfView(0.261799f), cameraScale(1.0f)
	{
		while (cameraIDTable.count(idCounter) != 0)
		{
			idCounter++;
		}

		cameraIDTable[idCounter] = this;
		cameraID = idCounter;
		idCounter++;
	}

	Camera::~Camera()
	{
		cameraIDTable.erase(cameraID);
	}

	unsigned int Camera::GetCameraID()
	{
		return cameraID;
	}

	float Camera::GetCameraNear()
	{
		return cameraNear;
	}

	float Camera::GetCameraFar()
	{
		return cameraFar;
	}

	float Camera::GetCameraFOV()
	{
		return fieldOfView;
	}

	float Camera::GetCameraScale()
	{
		return cameraScale;
	}

	void Camera::PreSerialize(json& jsonData) const
	{
		PRESERIALIZE_BASE(Camera);

		PRESERIALIZE_VALUE(cameraNear);
		PRESERIALIZE_VALUE(cameraFar);
		PRESERIALIZE_VALUE(fieldOfView);
		PRESERIALIZE_VALUE(cameraScale);
	}

	void Camera::PreDeserialize(const json& jsonData)
	{
		PREDESERIALIZE_BASE(Camera);

		PREDESERIALIZE_VALUE(cameraNear);
		PREDESERIALIZE_VALUE(cameraFar);
		PREDESERIALIZE_VALUE(fieldOfView);
		PREDESERIALIZE_VALUE(cameraScale);
	}

	void Camera::PostSerialize(json& jsonData) const
	{
		POSTSERIALIZE_BASE();
	}

	void Camera::PostDeserialize(const json& jsonData)
	{
		POSTDESERIALIZE_BASE();
	}
}