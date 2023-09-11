#pragma once

#include "ZeldaEngineAPI.h"

#include "Component.h"

#include "Vector2D.h"
#include "Vector3D.h"
#include "Matrix4x4.h"

#include <unordered_map>

namespace ZeldaEngine
{
	class ZELDAENGINE_API Camera : public Component
	{
	public:
		Camera();
		virtual ~Camera();

		unsigned int GetCameraID();
		float GetCameraNear();
		float GetCameraFar();
		float GetCameraFOV();
		float GetCameraScale();

	private:
		float cameraNear;	// 최소 거리
		float cameraFar;	// 최대 거리
		float fieldOfView;	// 시야각 (보통 45 ~ 75를 사용한다고 함)
		float cameraScale;

		unsigned int cameraID; // 카메라 ID는 Serialize하지 않는다.

		static std::unordered_map<unsigned int, Camera*> cameraIDTable;
		static unsigned int idCounter;

	public:
		// Component을(를) 통해 상속됨
		virtual void PreSerialize(json& jsonData) const override;

		virtual void PreDeserialize(const json& jsonData) override;

		virtual void PostSerialize(json& jsonData) const override;

		virtual void PostDeserialize(const json& jsonData) override;
	};
}