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
		float cameraNear;	// �ּ� �Ÿ�
		float cameraFar;	// �ִ� �Ÿ�
		float fieldOfView;	// �þ߰� (���� 45 ~ 75�� ����Ѵٰ� ��)
		float cameraScale;

		unsigned int cameraID; // ī�޶� ID�� Serialize���� �ʴ´�.

		static std::unordered_map<unsigned int, Camera*> cameraIDTable;
		static unsigned int idCounter;

	public:
		// Component��(��) ���� ��ӵ�
		virtual void PreSerialize(json& jsonData) const override;

		virtual void PreDeserialize(const json& jsonData) override;

		virtual void PostSerialize(json& jsonData) const override;

		virtual void PostDeserialize(const json& jsonData) override;
	};
}