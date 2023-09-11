#pragma once

#include <vector>
#include <string>

#include "ZeldaEngineAPI.h"

#include "Serializable.h"

#include "ZeldaMath.h"

namespace ZeldaEngine
{
	class ZELDAENGINE_API EngineSetting : public Serializable
	{
	public:
		// 충돌시 겹쳐졌을 때의 처리
		enum class PhysicsOverlapMode
		{
			Depth,		// 충돌 깊이에 비례한 속도로 밀어낸다.
			Velocity	// 각각의 속도에 비례한 속도로 밀어낸다.
		};

		enum class GraphicsEngine
		{
			FF_GRAPHICS
		};

		GraphicsEngine graphicsEngine;

		// LayerID를 얻어온다. 실패시 -1을 리턴한다.
		int GetLayerID(std::wstring layername);

		void SetLayerName(int layerid, std::wstring layername);
		std::wstring GetLayerName(int layerid);

		void SetCollisionSetting(bool collision, int layerid1, int layerid2);
		bool GetCollisionSetting(int layerid1, int layerid2);

		void SetDebugToggleKeyAllowed(bool value);
		bool GetDebugToggleKeyAllowed();

		void SetShowDebugInfo(bool value);
		bool GetShowDebugInfo();

		void SetWindowSize(float sizex, float sizey);
		float GetWindowSizeX();
		float GetWindowSizeY();

		void SetGameName(std::wstring gamename);
		std::wstring GetGameName();

		void SetStartScene(std::wstring sceneName);
		std::wstring GetStartScene();

		void SetResourceFile(std::wstring resourceName);
		std::wstring GetResourceFile();

		void SetPhysicsOverlapMode(PhysicsOverlapMode mode);
		PhysicsOverlapMode GetPhysicsOverlapMode();

		void SetDefaultGravity(ZeldaMath::Vector3D gravity);
		ZeldaMath::Vector3D GetDefaultGravity();

		const static int LAYER_MAX_SIZE;
		const static bool SHOWDEBUGINFO_DEFAULT;
		const static bool DEBUGTOGGLEKEYALLOWED_DEFAULT;
		const static float WINDOWSIZEX_DEFAULT;
		const static float WINDOWSIZEY_DEFAULT;

		// Serializable을(를) 통해 상속됨
		virtual void PreSerialize(json& jsonData) const override;
		virtual void PreDeserialize(const json& jsonData) override;
		virtual void PostSerialize(json& jsonData) const override;
		virtual void PostDeserialize(const json& jsonData) override;

	private:
		float windowSizeX;
		float windowSizeY;
		bool showDebugInfo;
		bool debugToggleKeyAllowed;
		std::vector<std::wstring> layer;
		std::vector<std::vector<bool>> collisionSetting;
		std::wstring gameName;
		std::wstring startSceneName;
		std::wstring resourceFileName;

		PhysicsOverlapMode physicsOverlapMode;
		ZeldaMath::Vector3D defaultGravity;

		//singleton
	public:
		static EngineSetting& GetInstance();

	private:
		EngineSetting();
		~EngineSetting();
		EngineSetting(const EngineSetting& ref);
		EngineSetting& operator=(const EngineSetting& ref) = delete;
	};
}